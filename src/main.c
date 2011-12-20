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
#include "queue.h"
#include "cmdli.h"
#include "usb.h"

int main(void)
{
	struct debug_t *debug;
	struct programs_t *progs;
	struct cmdli_t *cmdli;
	/* convenient pre-allcated structure */
	struct tm *tm_clock;
	char c;

	/* anti warning for non initialized variables */
	debug = NULL;
	progs = NULL;
	cmdli = NULL;
	tm_clock = NULL;

	/* Init sequence, turn on both led */
	led_init();
	usb_init();
	debug = debug_init(debug);
	progs = prog_init(progs);
	cmdli = cmdli_init(cmdli);
	tm_clock = date_init(tm_clock, debug);

	sei();
	date_hwclock_start();
	led_set(BOTH, OFF);

	while (1) {
		if (usb_connected) {
			debug->active = TRUE;
			c = uart_getchar(0, 0);

			if (c) {
				/* echo, shoud not be used */
				uart_putchar(0, c);
				cmdli_exec(c, cmdli, progs, debug);
			}
		} else {
			debug->active = FALSE;
		}

		if (date_timetorun(tm_clock, debug)) {
			led_set(GREEN, ON);

			if (progs->log) {
				debug_print_P(PSTR("Executing programs at "), debug);
				date(debug);
			}

			prog_run(progs, tm_clock, debug);

			if (prog_allarm(progs) && progs->log) {
				debug_print_P(PSTR("ALLARM! queue run skipped!\n"), debug);
			} else {
				if (progs->log) {
					debug_print_P(PSTR("Run queue at "), debug);
					date(debug);
				}

				queue_run(progs, tm_clock, debug);
			}

			/* print the temperature updated
			 * from the prog_run call
			 */
			if (progs->log)
				temperature_print(progs, debug);

			led_set(GREEN, OFF);
		}

		/*! \fixme not so good continuing call this */
		if (prog_allarm(progs) && progs->log)
			debug_print_P(PSTR("ALLARM! queue removed, I/O lines closed!\n"), debug);

	}

	date_hwclock_stop();
	cli();
	date_free(tm_clock);
	cmdli_free(cmdli);
	prog_free(progs);
	debug_free(debug);

	return(0);
}
