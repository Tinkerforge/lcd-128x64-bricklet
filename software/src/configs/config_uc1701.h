/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_uc1701.h: Configuration for UC1701x matrix lcd controller
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

#ifndef CONFIG_UC1701_H
#define CONFIG_UC1701_H

#include "xmc_gpio.h"

#define UC1701_SPI_BAUDRATE         1500000
#define UC1701_USIC_CHANNEL         USIC0_CH0
#define UC1701_USIC                 XMC_SPI0_CH0

#define UC1701_SCLK_PIN             P0_14
#define UC1701_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_14_AF_U0C0_SCLKOUT)

#define UC1701_SELECT_PIN           P0_13
#define UC1701_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P0_13_AF_U0C0_SELO4)

#define UC1701_MOSI_PIN             P2_0
#define UC1701_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P2_0_AF_U0C0_DOUT0)

#define UC1701_MISO_PIN             P0_15
#define UC1701_MISO_INPUT           XMC_USIC_CH_INPUT_DX0
#define UC1701_MISO_SOURCE          0b001 // DX0B

#define UC1701_SERVICE_REQUEST_RX   2
#define UC1701_SERVICE_REQUEST_TX   3

#define UC1701_IRQ_RX               11
#define UC1701_IRQ_RX_PRIORITY      0
#define UC1701_IRQ_TX               12
#define UC1701_IRQ_TX_PRIORITY      1


#define UC1701_RST_PIN              P0_8
#define UC1701_CD_PIN               P0_5
#define UC1701_BACKLIGHT_PIN        P1_0

#endif
