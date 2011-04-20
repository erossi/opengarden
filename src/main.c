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

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "debug.h"
#include "led.h"
#include "date.h"
#include "program.h"
#include "cmdli.h"

/*! Global EEPROM storage */
struct programms_t EEMEM EE_progs;

int main(void)
{
	struct debug_t *debug;
	struct programms_t *progs;
	struct cmdli_t *cmdli;
	char c;

	/* Init sequence, turn on both led */
	led_init();
	debug = debug_init();
	progs = prog_init();
	cmdli = cmdli_init(debug);
	date_init(debug);

	sei();
	date_hwclock_start();
	led_set(BOTH, OFF);

	while (1) {
		/*
		if (io_in_get(IN_P0)) {
			io_out_set(OUT_P0, 1);
			io_out_set(OUT_P1, 1);
		} else {
			io_out_set(OUT_P0, 0);
			io_out_set(OUT_P1, 0);
		}
		*/

		c = uart_getchar(0, 0);

		if (c)
			cmdli_exec(c, cmdli, debug);

		led_set(GREEN, BLINK);
		_delay_ms(500);
		date(debug);
	}

	date_hwclock_stop();
	cli();
	cmdli_free(cmdli);
	prog_free(progs);
	debug_free(debug);

	return(0);
}
