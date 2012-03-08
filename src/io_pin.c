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

/*! set a valve to On, Off or a pulse of PULSE_MSEC msec. */
void onoff_pulse(const uint8_t status)
{
	switch (status) {
		case ON:
			/* OnOff pin to on */
			OUT_CMD_PORT |= _BV(OUT_CMD_ONOFF);
			led_set(RED, ON);
			break;
		case PULSE:
			_delay_ms(1);
			onoff_pulse(ON);
			_delay_ms(PULSE_MSEC);
			onoff_pulse(OFF);
			_delay_ms(1);
			break;
		default:
			/* OnOff pin to off */
			OUT_CMD_PORT &= ~_BV(OUT_CMD_ONOFF);
			led_set(RED, OFF);
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

void io_pin_shut(void)
{
	OUT_PORT = 0;
	OUT_DDR = 0;
	OUT_CMD_PORT &= ~(_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
	OUT_CMD_DDR &= ~(_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
}

/*! Set IO oline.
 *
 * \param oline the output line to be set or clear.
 * \param onoff set or clear.
 * \valvetype type of the valve in used.
 * \note online is in the range 1 to 8.
 */
void io_out_set(const uint8_t oline, const uint8_t onoff, const uint8_t valvetype)
{
	if (onoff) {
		OUT_PORT |= _BV(oline);
		valve_open(valvetype);
	} else {
		valve_close(valvetype);
		OUT_PORT &= ~_BV(oline);
	}
}

/*! \brief get allarm status */
uint8_t io_in_allarm(struct programs_t *progs)
{
	uint8_t err = 0;

	if (progs->flags & _BV(FL_ALRM)) {
		if (IN_PIN & _BV(IN_P0))
			err |= _BV(0);

		if (IN_PIN & _BV(IN_P1))
			err |= _BV(1);
	} else {
		if (!(IN_PIN & _BV(IN_P0)))
			err |= _BV(0);

		if (!(IN_PIN & _BV(IN_P1)))
			err |= _BV(1);
	}

	return(err);
}

/*! \brief Are there any IO out line in use?
 */
uint8_t io_line_in_use(void)
{
	return(OUT_PORT);
}

/*! Close all the output line.
 * \bug non compatibile with bistable valve, too quick.
 */
void io_out_off(struct programs_t *progs)
{
	uint8_t i;

	for (i=0; i<8; i++)
		io_out_set(i, OFF, progs->valve);
}
