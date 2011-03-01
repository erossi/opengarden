/* This file is part of 
 * Copyright (C) 2011 Enrico Rossi
 *
 * Chpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \file main.c
  \brief Main.
 */

#define AU_PORT PORTA
#define AU_DDR DDRA
#define IN_PORT PORTD
#define IN_PIN PIND
#define IN_DDR DDRD

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

int main(void)
{
	int i;

	/* Init sequence, turn on both led */
	led_init();
	AU_DDR = 0xff; /* all output */
	AU_PORT = 0;
	IN_DDR = 0;
	IN_PORT = 0;
	led_set(BOTH, OFF);

	while (1) {
		if (IN_PIN & _BV(4))
			AU_PORT |= _BV(0) | _BV(1);
		else
			AU_PORT &= ~(_BV(0) | _BV(1));

		if (IN_PIN & _BV(5))
			AU_PORT |= _BV(2) | _BV(3);
		else
			AU_PORT &= ~(_BV(2) | _BV(3));

		if (IN_PIN & _BV(6))
			AU_PORT |= _BV(4) | _BV(5);
		else
			AU_PORT &= ~(_BV(4) | _BV(5));

		if (IN_PIN & _BV(7))
			AU_PORT |= _BV(6) | _BV(7);
		else
			AU_PORT &= ~(_BV(6) | _BV(7));


		led_set(GREEN, ON);
		_delay_ms(1000);
		led_set(BOTH, OFF);
	}

	return(0);
}
