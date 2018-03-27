/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * spi.c: SPI functions shared between UC1701 and TSC2046e
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "spi.h"

#include <string.h>

#include "configs/config_uc1701.h"

#include "bricklib2/os/coop_task.h"
#include "bricklib2/utility/util_definitions.h"

#include "xmc_gpio.h"

SPI spi;

#define uc1701_rx_irq_handler IRQ_Hdlr_11
#define uc1701_tx_irq_handler IRQ_Hdlr_12

// Set pointers to read/write buffer
// With this the compiler can properly optimize the access!
uint8_t *spi_data_read = spi.data;
uint8_t *spi_data_write = spi.data;
uint8_t *spi_data_write_end = spi.data;

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) uc1701_rx_irq_handler(void) {
	// Always read exactly 8 bytes.
	// The FIFO is configured to call the IRQ only if at least 8 bytes are in it.
	// By doing it this way with -03 as RAM code, it only takes 2.98us per 64 bit of data.
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
	*spi_data_read = UC1701_USIC->OUTR;
	spi_data_read++;
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) uc1701_tx_irq_handler(void) {
	// Max fill level is 16.
	const uint8_t num = MIN(16-XMC_USIC_CH_TXFIFO_GetLevel(UC1701_USIC), spi_data_write_end - spi_data_write);
	switch(num) {
		case 16: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 15: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 14: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 13: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 12: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 11: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 10: UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 9:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 8:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 7:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 6:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 5:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 4:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 3:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 2:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
		case 1:  UC1701_USIC->IN[0] = *spi_data_write; spi_data_write++;
	}

	if(spi_data_write >= spi_data_write_end) {
		XMC_USIC_CH_TXFIFO_DisableEvent(UC1701_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
	}
}

void spi_task_transceive(const uint8_t *data, const uint32_t length, XMC_SPI_CH_SLAVE_SELECT_t slave) {
	while(spi.tranceive_ongoing) {
		coop_task_yield();
	}

	spi.tranceive_ongoing = true;

	memcpy(spi.data, data, length);
	spi.data_length = length;
	spi_data_read = spi.data;
	spi_data_write = spi.data;
	spi_data_write_end = spi.data + length;

	XMC_SPI_CH_ClearStatusFlag(UC1701_USIC, XMC_SPI_CH_STATUS_FLAG_MSLS);
	XMC_SPI_CH_EnableSlaveSelect(UC1701_USIC, slave);

	// If possible write and read data in this coop task.
	// Only if other tasks take too much time we go through the interrupts.
	// This may seem a bit redundant, but in profiling tests we found that this approach
	// is by far the most efficient.
	// Even if we transfer at a full 1.5MHz rate, the interrupt will only be called about once
	// per ms. The rest of the data is transferred through this loop, which will be called
	// by the scheduler anyway.
	while(spi_data_read < spi.data + spi.data_length) {
		// Disable TX IRQ to avoid race condition and write as much data by
		// hand as possible.
		XMC_USIC_CH_TXFIFO_DisableEvent(UC1701_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
		while((!XMC_USIC_CH_TXFIFO_IsFull(UC1701_USIC)) && (spi_data_write < spi_data_write_end)) {
			UC1701_USIC->IN[0] = *spi_data_write;
			spi_data_write++;
		}
		NVIC_ClearPendingIRQ(UC1701_IRQ_TX);
		if(spi_data_write < spi_data_write_end) {
			// If we have not yet written everything here, we turn the interrupt on.
			// If the bootloader or other tasks now takes too long the rest of the data can
			// be send through the interrupt.
			// Otherwise the we will run the code above again and send data in this loop.
			XMC_USIC_CH_TXFIFO_EnableEvent(UC1701_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
		}

		// Disable RX IRQ to avoid race condition and read as much data by
		// hand as possible.
		// If there are less then 8 bytes left in the FIFO and the SPI transfer
		// is finished, we read the rest of the bytes here (the RX IRQ is only called if 8 or
		// more bytes are in the FIFO).
		NVIC_DisableIRQ(UC1701_IRQ_RX);
		while(!XMC_USIC_CH_RXFIFO_IsEmpty(UC1701_USIC)) {
			*spi_data_read = UC1701_USIC->OUTR;
			spi_data_read++;
		}
		// We have to clear the IRQ here. There might have been an IRQ between the 
		// disable and the first read.
		NVIC_ClearPendingIRQ(UC1701_IRQ_RX);
		NVIC_EnableIRQ(UC1701_IRQ_RX);

		coop_task_yield();
	}

	XMC_SPI_CH_DisableSlaveSelect(UC1701_USIC);
	spi.tranceive_ongoing = false;
}

void spi_init(void) {
	memset(&spi, 0, sizeof(SPI));

	// UC1701 and TSC2046 use same SPI configuration, so we can use the defines from either of them.

	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = UC1701_SPI_BAUDRATE,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
		.mode             = UC1701_MOSI_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// MISO pin configuration
	const XMC_GPIO_CONFIG_t miso_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
		.mode             = UC1701_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure MISO pin
	XMC_GPIO_Init(UC1701_MISO_PIN, &miso_pin_config);

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(UC1701_USIC, &channel_config);
	UC1701_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0
//	UC1701_USIC->PCR_SSCMode &= ~USIC_CH_PCR_SSCMode_TIWEN_Msk; // Disable time between bytes

	XMC_SPI_CH_SetSlaveSelectPolarity(UC1701_USIC, XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS);
	XMC_SPI_CH_SetBitOrderMsbFirst(UC1701_USIC);

	XMC_SPI_CH_SetWordLength(UC1701_USIC, (uint8_t)8U);
	XMC_SPI_CH_SetFrameLength(UC1701_USIC, (uint8_t)64U);

	XMC_SPI_CH_SetTransmitMode(UC1701_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(UC1701_USIC,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
/*	XMC_SPI_CH_ConfigureShiftClockOutput(UC1701_USIC,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);*/
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(UC1701_USIC, 2);

	// Set input source path
	XMC_SPI_CH_SetInputSource(UC1701_USIC, UC1701_MISO_INPUT, UC1701_MISO_SOURCE);

	// SPI Mode: CPOL=1 and CPHA=1
	UC1701_USIC_CHANNEL->DX1CR |= USIC_CH_DX1CR_DPOL_Msk;

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(UC1701_USIC, 48, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Configure receive FIFO
	XMC_USIC_CH_RXFIFO_Configure(UC1701_USIC, 32, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Set service request for tx FIFO transmit interrupt
	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(UC1701_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, UC1701_SERVICE_REQUEST_TX);  // IRQ UC1701_IRQ_TX

	// Set service request for rx FIFO receive interrupt
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(UC1701_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD, UC1701_SERVICE_REQUEST_RX);  // IRQ UC1701_IRQ_RX
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(UC1701_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE, UC1701_SERVICE_REQUEST_RX); // IRQ UC1701_IRQ_RX

	//Set priority and enable NVIC node for transmit interrupt
	NVIC_SetPriority((IRQn_Type)UC1701_IRQ_TX, UC1701_IRQ_TX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)UC1701_IRQ_TX);

	// Set priority and enable NVIC node for receive interrupt
	NVIC_SetPriority((IRQn_Type)UC1701_IRQ_RX, UC1701_IRQ_RX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)UC1701_IRQ_RX);

	// Start SPI
	XMC_SPI_CH_Start(UC1701_USIC);

	// Configure SCLK pin
	XMC_GPIO_Init(UC1701_SCLK_PIN, &sclk_pin_config);

	// Configure MOSI pin
	XMC_GPIO_Init(UC1701_MOSI_PIN, &mosi_pin_config);

	XMC_USIC_CH_RXFIFO_Flush(UC1701_USIC);
	XMC_USIC_CH_RXFIFO_EnableEvent(UC1701_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
}
