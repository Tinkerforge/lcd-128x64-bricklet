/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * spi.h: SPI functions shared between UC1701 and TSC2046e
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

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>

#include "xmc_spi.h"

#define SPI_BUFFER_SIZE 256

typedef struct {
	uint8_t data[SPI_BUFFER_SIZE];
	uint16_t data_length;

	bool tranceive_ongoing;
} SPI;

extern SPI spi;

void spi_task_transceive(const uint8_t *data, const uint32_t length, XMC_SPI_CH_SLAVE_SELECT_t slave);
void spi_init(void);

#endif
