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

/*! \file io_pin.c
  \brief IO lines low level API.
 */

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "io_pin.h"

/*! Return how many bits are 1 in a byte
 */
uint8_t bit_set(const uint8_t byte)
{
	uint8_t i, bit;

	bit = 0;

	for (i=0; i<8; i++)
		if (byte & _BV(i))
			bit++;

	return(bit);
}

/*! set a valve to On, Off or a pulse of 100msec */
void onoff_pulse(const uint8_t status)
{
	switch (status) {
		case ON:
			/* OnOff pin to on */
			OUT_CMD_PORT |= _BV(OUT_CMD_ONOFF);
			break;
		case PULSE:
			_delay_ms(1);
			OUT_CMD_PORT |= _BV(OUT_CMD_ONOFF);
			_delay_ms(100);
			OUT_CMD_PORT &= ~_BV(OUT_CMD_ONOFF);
			_delay_ms(1);
			break;
		default:
			/* OnOff pin to off */
			OUT_CMD_PORT &= ~_BV(OUT_CMD_ONOFF);
	}
}

/*! Open a valve meaning water flows.
 *
 * \param valvetype MONOSTABLE or BISTABLE.
 */
void valve_open(const uint8_t valvetype)
{
	if (valvetype == BISTABLE) {
		/* PN high */
		OUT_CMD_PORT |= _BV(OUT_CMD_PN);
		onoff_pulse(PULSE);
		/* back to default set on standby */
		OUT_CMD_PORT &= ~_BV(OUT_CMD_PN);
	} else {
		/* OnOff pin to on */
		onoff_pulse(ON);
	}
}

/*! Close a valve meaning water stop.
 *
 * \param valvetype MONOSTABLE or BISTABLE.
 */
void valve_close(const uint8_t valvetype)
{
	if (valvetype == BISTABLE) {
		/* PN low */
		OUT_CMD_PORT &= ~_BV(OUT_CMD_PN);
		onoff_pulse(PULSE);
		/* back to default set on standby */
		OUT_CMD_PORT &= ~_BV(OUT_CMD_PN);
	} else {
		/* OnOff pin to off */
		onoff_pulse(OFF);
	}
}

/*! setup the io port */
void io_pin_init(void)
{
	IN_DDR &= ~(_BV(IN_P0) | _BV(IN_P1));
	IN_PORT &= ~(_BV(IN_P0) | _BV(IN_P1));
	OUT_PORT = 0;
	OUT_DDR = 0xff; /* all output */
	OUT_CMD_DDR |= (_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
	OUT_CMD_PORT &= ~(_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
}

uint8_t io_in_get(const uint8_t port)
{
	if (IN_PIN & _BV(port))
		return(1);
	else
		return(0);
}

/*! Set IO oline.
 *
 * \param oline the output line to be set or clear.
 * \param onoff set or clear.
 * \note online is in the range 1 to 8.
 */
void io_out_set(const uint8_t oline, const uint8_t onoff)
{
	if (onoff) {
		OUT_PORT |= _BV(oline);
		valve_open(MONOSTABLE);
	} else {
		valve_close(MONOSTABLE);
		OUT_PORT &= ~_BV(oline);
	}
}

uint8_t io_out_get(const uint8_t pin)
{
	if (OUT_PORT & _BV(pin))
		return(1);
	else
		return(0);
}

uint8_t io_in_allarm(void)
{
	uint8_t err = 0;

	if (!(IN_PIN & _BV(IN_P0)))
		err = _BV(0);

	if (!(IN_PIN & _BV(IN_P1)))
		err |= _BV(1);

	return(err);
}

/*! \brief How many IO out line are in use?
 */
uint8_t io_line_in_use(void)
{
	return(bit_set(OUT_PORT));
}

void io_out_off(void)
{
	uint8_t i;

	for (i=0; i<8; i++)
		io_out_set(_BV(i), OFF);
}
