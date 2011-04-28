/* Copyright (C) 2011 Enrico Rossi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "tcn75.h"

/*
PC0 SCL
PC1 SDA
PC2 IN ALLERT
PC3 - 5 ADDRESS
*/

void tcn75_init(void)
{
	i2c_init();
}

float tcn75_read_temperature(void)
{
	int16_t tempr;
	float temp;

	temp = -99;
	tempr = -99;

	if (i2c_master_send_b(ADDR_W, 0)) {
		/* error */
	} else {
		if (i2c_master_read_w(ADDR_R, &tempr)) {
			/* Error */
		} else {
			temp = tempr/256;
		}
	}

	return(temp);
}
