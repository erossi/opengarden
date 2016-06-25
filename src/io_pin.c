/* This file is part of OpenGarden
 * Copyright (C) 2011-2013 Enrico Rossi
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

/*! send an On, Off or a pulse of PULSE_MSEC msec. on the 
 * OUT_CMD_ONOFF pin.
 */
void onoff_pulse(const uint8_t status)
{
	switch (status) {
		case ON:
			OUT_CMD_PORT |= _BV(OUT_CMD_ONOFF);
			break;
		case PULSE:
			_delay_ms(1);
			onoff_pulse(ON);
			_delay_ms(PULSE_MSEC);
			onoff_pulse(OFF);
			_delay_ms(1);
			break;
		default:
			OUT_CMD_PORT &= ~_BV(OUT_CMD_ONOFF);
	}
}

/*! Send the command to open the pre-selected valve.
 *
 * \param valvetype MONOSTABLE or BISTABLE.
 * \note Open a valve meaning water flows.
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

/*! Inverse of the valve_open().
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

/*! setup the I/O port.
 *
 * \param status I/O port status to be activated.
 * \bug the status of the IO lines should not be used.
 */
void io_init(void)
{
	IN_DDR &= ~(_BV(IN_P0) | _BV(IN_P1));
	IN_PORT &= ~(_BV(IN_P0) | _BV(IN_P1));
	OUT_PORT = 0;
	OUT_DDR = 0xff; /* all output */
	OUT_CMD_DDR |= (_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
	OUT_CMD_PORT &= ~(_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
}

/*! \brief Shutdown all I/O pin.
 *
 * Not only make them 0, but also release the IO lines.
 */
void io_shut(void)
{
	OUT_PORT = 0;
	OUT_DDR = 0;
	OUT_CMD_PORT &= ~(_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
	OUT_CMD_DDR &= ~(_BV(OUT_CMD_ONOFF) | _BV(OUT_CMD_PN));
}

/*! Set I/O oline.
 *
 * open or close the I/O line, the OUT_PORT is supposed to
 * represent the correct status of the I/O lines only if the
 * valve type is MONOSTABLE.
 * In the open case, we set the port to the corresponding oline,
 * then we also set the oline into the progs structure in order to
 * remember which ioline we are using, because this info will be lost
 * during the stand-by period. Then, after opened the valve, if the valve
 * is BISTABLE we can clear the oline from the port, no need to keep
 * the line up; instead if the valve is MONOSTABLE then the PORT
 * represent the ioline in use and it must not be cleared.
 * In the close case and bistable valve, the port must be set to
 * the correct ioline before the close operation or it will not
 * affect any line.
 *
 * \param oline the output line to be set.
 * \param onoff set or clear.
 * \param progs ptr to the parameters.
 * \note online is in the range 0 to 7.
 * \note no more than 1 line can be used at the same time.
 * \note if OFF, the oline param is ignored, there should be only 1 oline
 * in use to be closed.
 */
void io_set(const uint8_t oline, const uint8_t onoff, struct programs_t *progs)
{

#ifdef GTP_HARDWARE
	if (onoff) {
		/* store the ioline in use into the progs struct. */
		progs->ioline = _BV(oline);
		/* set the ioline to the port. */
		OUT_PORT = _BV(oline);

		if (progs->valve == BISTABLE) {
			onoff_pulse(ON);
			_delay_ms(1);
			OUT_PORT = 0;
			_delay_ms(60);
			onoff_pulse(OFF);
		} else {
			valve_open(progs->valve);
		}
	} else {
		if (progs->valve == BISTABLE) {
			OUT_PORT = progs->ioline;
			OUT_CMD_PORT |= _BV(OUT_CMD_PN);
			_delay_ms(1);
			OUT_PORT = 0;
			_delay_ms(60);
			OUT_CMD_PORT &= ~_BV(OUT_CMD_PN);
		} else {
			valve_close(progs->valve);
		}

		OUT_PORT = 0;
		progs->ioline = 0;
	}
#else
	if (onoff) {
		/* store the ioline in use into the progs struct. */
		progs->ioline = _BV(oline);
		/* set the ioline to the port. */
		OUT_PORT = _BV(oline);
		valve_open(progs->valve);

		if (progs->valve == BISTABLE)
			OUT_PORT = 0;
	} else {
		if (progs->valve == BISTABLE)
			OUT_PORT = progs->ioline;

		valve_close(progs->valve);
		OUT_PORT = 0;
		progs->ioline = 0;
	}
#endif

}

/*! Are there any IO out line in use?
 *
 */
uint8_t io_get(struct programs_t *progs)
{
	if (progs->valve == MONOSTABLE)
		return(OUT_PORT);
	else
		return(progs->ioline);
}

/*! Close all the output line.
 *
 * \note the oline is ignored since there must be only one oline
 * in use.
 */
void io_off(struct programs_t *progs)
{
	io_set(0, OFF, progs);
}

/*! \brief get alarm status.
 * It return if one or both the alarm line are set, based on
 * the alarm level trigger.
 *
 * \param progs the struct to look for the alarm level setup.
 * \return 0 - 3 the alarm line active in binary mapping.
 */
uint8_t io_alarm(struct programs_t *progs)
{
	uint8_t err = 0;

	if (flag_get(progs, FL_LEVEL)) {
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

	if (err)
		flag_set(progs, FL_ALRM, TRUE);
	else
		flag_set(progs, FL_ALRM, FALSE);

	return(err);
}
