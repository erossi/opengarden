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

/*!
  \file rtc.c
  \brief Counters handler and IRQ routine.
  */

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "rtc.h"

/*!
  IRQ wakes up on the timer oveflow and increment the global seconds.
 */
ISR(TIMER2_OVF_vect)
{
	rtc_seconds++;
}

/*! setup timer/counter on 32Khz external clock. */
void rtc_setup(void)
{
	ASSR = _BV(AS2);
	TCCR2A = 0;
	TCCR2B = 0;

	/*! enable interrupt on timer 2 overflow */
	TIMSK2 = _BV(TOIE2);
}

/*! setup prescaler to scale to 1 sec counter and start counter. */
void rtc_start(void)
{
	/*! counter prescaler 128 */
	TCCR2B = _BV(CS22) | _BV(CS20);
	loop_until_bit_is_clear(ASSR, TCR2BUB);
}

/*! stop the counter. */
void rtc_stop(void)
{
	TCCR2B = 0;
	loop_until_bit_is_clear(ASSR, TCR2BUB);
}
