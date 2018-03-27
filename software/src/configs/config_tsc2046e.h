/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_tsc2046e.h: Configuration for TSC2046E touch screen controller
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

#ifndef CONFIG_TSC2046E_H
#define CONFIG_TSC2046E_H

#include "xmc_gpio.h"

#define TSC2046E_SPI_BAUDRATE         1500000
#define TSC2046E_USIC_CHANNEL         USIC0_CH0
#define TSC2046E_USIC                 XMC_SPI0_CH0

#define TSC2046E_SCLK_PIN             P0_14
#define TSC2046E_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_14_AF_U0C0_SCLKOUT)

#define TSC2046E_SELECT_PIN           P0_12
#define TSC2046E_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P0_12_AF_U0C0_SELO3)

#define TSC2046E_MOSI_PIN             P2_0
#define TSC2046E_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P2_0_AF_U0C0_DOUT0)

#define TSC2046E_MISO_PIN             P0_15
#define TSC2046E_MISO_INPUT           XMC_USIC_CH_INPUT_DX0
#define TSC2046E_MISO_SOURCE          0b001 // DX0B

#define TSC2046E_SERVICE_REQUEST_RX   2
#define TSC2046E_SERVICE_REQUEST_TX   3

#define TSC2046E_IRQ_RX               11
#define TSC2046E_IRQ_RX_PRIORITY      2
#define TSC2046E_IRQ_TX               12
#define TSC2046E_IRQ_TX_PRIORITY      3


#define TSC2046E_PENIRQ_PIN           P1_1
#define TSC2046E_BUSY_PIN             P0_6
#define TSC2046E_LED_PIN              P2_1

#endif
