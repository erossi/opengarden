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
#include "io_pin.h"

void io_pin_init(void)
{
	IN_DDR = 0;
	IN_PORT = 0;
	OUT_DDR = 0xff; /* all output */
	OUT_PORT = 0;
	/*! \bug fix either _BV(PB0) */
	OUT_CMD_DDR = 3; /* pb0 and pb1 out */
	OUT_CMD_PORT = 0; /* bug should use only pb0 and pb1 lines */
}

uint8_t io_in_get(const uint8_t port)
{
	if (IN_PIN & _BV(port))
		return(1);
	else
		return(0);
}

void io_out_set(const uint8_t pin, const uint8_t val)
{
	/* \bug insert 1ms delay from ONOFF and PN switch */
	/* \bug limit the IO to only 1 line at a time to save
	 * hardware from fried */
	if (val) {
		OUT_PORT |= _BV(pin);
		OUT_CMD_PORT |= _BV(OUT_CMD_ONOFF);
	} else {
		OUT_CMD_PORT &= ~_BV(OUT_CMD_ONOFF);
		OUT_PORT &= ~_BV(pin);
	}
}

uint8_t io_out_get(const uint8_t pin)
{
	if (OUT_PORT & _BV(pin))
		return(1);
	else
		return(0);
}

void io_out_change_line(const uint8_t oline, const uint8_t onoff)
{
	if (oline & _BV(0))
		io_out_set(OUT_P0, onoff);
	if (oline & _BV(1))
		io_out_set(OUT_P1, onoff);
	if (oline & _BV(2))
		io_out_set(OUT_P2, onoff);
	if (oline & _BV(3))
		io_out_set(OUT_P3, onoff);
	if (oline & _BV(4))
		io_out_set(OUT_P4, onoff);
	if (oline & _BV(5))
		io_out_set(OUT_P5, onoff);
	if (oline & _BV(6))
		io_out_set(OUT_P6, onoff);
	if (oline & _BV(7))
		io_out_set(OUT_P7, onoff);
}

uint8_t io_in_allarm(void)
{
	uint8_t err = 0;

	if (!(IN_PIN & _BV(IN_P0)))
		err = _BV(0);

	if (!(IN_PIN & _BV(IN_P1)))
		err |= _BV(1);

	/* reverse allarm */
	/*
	if (IN_PIN & _BV(IN_P2))
		err |= _BV(2);

	if (IN_PIN & _BV(IN_P3))
		err |= _BV(3);
		*/

	return(err);
}

/*! \brief How many IO out line are in use?
 */
uint8_t io_line_in_use(void)
{
	uint8_t i, j;

	j = 0;

	for (i=0; i<8; i++)
		if (OUT_PORT & _BV(i))
			j++;

	return(j);
}
