/* This file is part of OpenGarden
 * Copyright (C) 2011, 2012 Enrico Rossi
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
  \file usb.c
  \brief usb connection cheching functions
  */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usb.h"

/*! IRQ wakes up when PC/usb connect or disconnect. */
ISR(INT0_vect)
{
	if (PORTD & PD2)
		usb_connected = TRUE;
	else
		usb_connected = FALSE;
}

/*! enable irq int0 */
void usb_init(void)
{
	/* PD2 input */
	DDRD &= ~_BV(PD2);
	/* Trigger irq on any edge of the INT0 pin */
	EICRA |= _BV(ISC00);
	/* Enable irq0 */
	EIMSK |= _BV(INT0);
}

/*! disable INT0 irq */
void usb_shutdown(void)
{
	/* disable irq0 */
	EIMSK &= ~_BV(INT0);
	/* disable triggers */
	EICRA &= ~(_BV(ISC01) | _BV(ISC00));
}
