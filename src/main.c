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

/*! \file main.c
  \brief Main.
 */

#define AU_PORT PORTA
#define AU_DDR DDRA

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "debug.h"
#include "led.h"
#include "time.h"
#include "io_input.h"

int main(void)
{
	struct debug_t *debug;
	time_t clock = 1299764113;

	/* Init sequence, turn on both led */
	led_init();
	io_in_init();
	debug = debug_init();
	AU_DDR = 0xff; /* all output */
	AU_PORT = 0;
	led_set(BOTH, OFF);
	rtc_setup();
	settimeofday(clock);

	sei();
	rtc_start();

	while (1) {
		if (io_in_get(IN_P0))
			AU_PORT |= _BV(0) | _BV(1);
		else
			AU_PORT &= ~(_BV(0) | _BV(1));

		if (io_in_get(IN_P1))
			AU_PORT |= _BV(2) | _BV(3);
		else
			AU_PORT &= ~(_BV(2) | _BV(3));

		if (io_in_get(IN_P2))
			AU_PORT |= _BV(4) | _BV(5);
		else
			AU_PORT &= ~(_BV(4) | _BV(5));

		if (io_in_get(IN_P3))
			AU_PORT |= _BV(6) | _BV(7);
		else
			AU_PORT &= ~(_BV(6) | _BV(7));


		led_set(GREEN, BLINK);
		_delay_ms(500);
		clock = time(NULL);
		debug->line = ctime(&clock);
		debug_print(debug);
	}

	rtc_stop();
	cli();
	debug_free(debug);

	return(0);
}
