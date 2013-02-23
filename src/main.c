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

/*! \file main.c
 * \brief Main.
 */

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "i2c.h"
#include "debug.h"
#include "led.h"
#include "date.h"
#include "program.h"
#include "queue.h"
#include "cmdli.h"
#include "usb.h"

/*! The main function. */
void job_on_the_field(struct programs_t *progs, struct debug_t *debug, struct tm *tm_clock)
{
	if (flag_get(progs, FL_LED))
		led_set(GREEN, ON);

	if (flag_get(progs, FL_LOG)) {
		debug_print_P(PSTR("Executing programs at "), debug);
		date(debug);
	}

	prog_run(progs, tm_clock, debug);

	if (prog_alarm(progs)) {
		if (flag_get(progs, FL_LED))
			led_set(RED, BLINK);

		if (flag_get(progs, FL_LOG))
			debug_print_P(PSTR("ALARM! queue run skipped!\n"), debug);
	} else {
		if (flag_get(progs, FL_LOG)) {
			debug_print_P(PSTR("Run queue at "), debug);
			date(debug);
		}

		queue_run(progs, tm_clock, debug);
	}

	/* print the temperature updated
	 * from the prog_run call
	 */
	if (flag_get(progs, FL_LOG))
		temperature_print(progs, debug);

	led_set(GREEN, OFF);
}

/*! Sleep function.
 *
 * Which IO line is in use is recorded in the progs struct.
 *
 * \note Incompatible with MONOSTABLE valve.
 */
void go_to_sleep(struct debug_t *debug)
{
	/* shut down everything */
	i2c_shut();
	debug_stop(debug);
	io_shut();
	led_shut();
	/* start sleep procedure */
	sleep_enable();
	sleep_bod_disable();
	sleep_cpu();
	sleep_disable();
	/* restart everything */
	led_init();
	io_init();
	debug_start(debug);
	debug->active = FALSE;
	i2c_init();
}

/*! main */
int main(void)
{
	struct debug_t *debug;
	struct programs_t *progs;
	struct cmdli_t *cmdli;
	/* convenient pre-allocated structure */
	struct tm *tm_clock;
	char c;

	/* anti warning for non initialized variables */
	debug = NULL;
	progs = NULL;
	cmdli = NULL;
	tm_clock = NULL;

	/* Init sequence, turn on both led */
	led_init();
	led_set(BOTH, ON);

	io_init();
	usb_init();
	debug = debug_init(debug);
	progs = prog_init(progs);
	cmdli = cmdli_init(cmdli);
	tm_clock = date_init(tm_clock, debug);

        set_sleep_mode(SLEEP_MODE_PWR_SAVE);

	sei();
	date_hwclock_start();
	led_set(BOTH, OFF);

	while (1) {
		/* If PC is connected or the valves are monostable
		 * then check for a command sent from the user
		 * and execute it.
		 * Anyway do NOT go to sleep.
		 */
		if (usb_connected || (progs->valve == MONOSTABLE)) {
			debug->active = TRUE;
			c = uart_getchar(0, 0);

			if (c) {
				/* echo */
				uart_putchar(0, c);
				cmdli_exec(c, cmdli, progs, debug);
			}
		} else {
			go_to_sleep(debug);
		}

		/* if there is a job to do (open, close valves).
		 */
		if (date_timetorun(tm_clock, debug))
			job_on_the_field(progs, debug, tm_clock);

		/*! \bug Calling the check for alarm at any
		 * cycle is not so good, especially with
		 * monostable valve the alarm trigger is continuous.
		 */
		if (prog_alarm(progs)) {
			if (flag_get(progs, FL_LED))
				led_set(RED, BLINK);

			if (flag_get(progs, FL_LOG))
				debug_print_P(PSTR("ALARM! queue removed, I/O lines closed!\n"), debug);
		}
	}

	/* This part should never be reached */
	date_hwclock_stop();
	cli();
	date_free(tm_clock);
	cmdli_free(cmdli);
	prog_free(progs);
	debug_free(debug);

	return(0);
}
