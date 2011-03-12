/* This file is part of OpenGarden
 * Copyright (C) 2011 Enrico Rossi
 *
 * OpenGarden is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenGarden is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \file io_out.c
  \brief Main.
 */

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "io_out.h"

void io_out_init(void)
{
	OUT_DDR = 0xff; /* all output */
	OUT_PORT = 0;
}

void io_out_set(const uint8_t pin, const uint8_t val)
{
	if (val)
		OUT_PORT |= _BV(pin);
	else
		OUT_PORT &= ~_BV(pin);
}

uint8_t io_out_get(const uint8_t pin)
{
	if (OUT_PORT & pin)
		return(1);
	else
		return(0);
}
