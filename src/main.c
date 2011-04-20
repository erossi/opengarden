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
	/*! convenient pre-allcated structure */
	struct tm *tm_clock;
	uint8_t tmp;
	char c;

	/* anti warning for non initialized variables */
	debug = NULL;
	progs = NULL;
	cmdli = NULL;
	tm_clock = NULL;

	/* Init sequence, turn on both led */
	led_init();
	debug = debug_init(debug);
	progs = prog_init(progs);
	cmdli = cmdli_init(cmdli, debug);
	tm_clock = date_init(tm_clock, debug);

	sei();
	date_hwclock_start();
	led_set(BOTH, OFF);

	while (1) {
		c = uart_getchar(0, 0);

		if (c)
			cmdli_exec(c, cmdli, debug);

		if (date_timetorun(tm_clock, debug)) {
			tmp = tm_clock->tm_hour * 2;

			if (tm_clock->tm_min > 29)
				tmp++;

			prog_run(progs, tmp, debug);
		}

		led_set(GREEN, BLINK);
		_delay_ms(500);
		date(debug);
	}

	date_hwclock_stop();
	cli();
	date_free(tm_clock);
	cmdli_free(cmdli);
	prog_free(progs);
	debug_free(debug);

	return(0);
}
