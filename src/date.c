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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "date.h"

void date_set(struct tm *date, struct debug_t *debug)
{
        debug_print_P(PSTR("Enter date [YYYYMMDDhhmm]: "), debug);
	debug_get_str(debug->line);
	uart_putchar(0, '\n');
	/* Year */
	strlcpy(debug->string, debug->line, 5);
	date->tm_year = atoi(debug->string) - 1900;
	/* Month */
	strlcpy(debug->string, debug->line + 4, 3);
	date->tm_mon = atoi(debug->string) - 1;
	/* Day */
	strlcpy(debug->string, debug->line + 6, 3);
	date->tm_mday = atoi(debug->string);
	/* Hour */
	strlcpy(debug->string, debug->line + 8, 3);
	date->tm_hour = atoi(debug->string);
	/* Minute */
	strlcpy(debug->string, debug->line + 10, 3);
	date->tm_min = atoi(debug->string);
	date->tm_sec = 0;
}

/*! print the current time */
void date(struct debug_t *debug)
{
	time_t clock;

	clock = time(NULL);
	strcpy(debug->line, ctime(&clock));
	debug_print(debug);
	uart_putchar(0, '\n');
}

/*! adjust the internal clock */
struct tm *date_init(struct tm *tm_clock, struct debug_t *debug)
{
	time_t clock = 1299764113;

	/* this should set the pointer to the
	 * preallocated area of lastTime.
	 */
	tm_clock = gmtime(&clock);

	rtc_setup(); /* Prepare the HW clock counter */
	date_set(tm_clock, debug); /* Input the current time. */
	clock = mktime(tm_clock); /* convert the time into sec. */
	settimeofday(clock); /* set the clock to the current time */

        debug_print_P(PSTR("The date is now: "), debug);
	date(debug);

	/*
	debug->line = itoa(date->tm_year, debug->line, 10);
	debug_print(debug);
	uart_putchar(0, '\n');
	debug->line = itoa(date->tm_mon, debug->line, 10);
	debug_print(debug);
	uart_putchar(0, '\n');
	debug->line = itoa(date->tm_mday, debug->line, 10);
	debug_print(debug);
	uart_putchar(0, '\n');
	debug->line = itoa(date->tm_hour, debug->line, 10);
	debug_print(debug);
	uart_putchar(0, '\n');
	debug->line = itoa(date->tm_min, debug->line, 10);
	debug_print(debug);
	uart_putchar(0, '\n');
	debug->line = itoa(date->tm_sec, debug->line, 10);
	debug_print(debug);
	uart_putchar(0, '\n');
	*/

	return(tm_clock);
}

void date_free(struct tm *tm_clock)
{
	free(tm_clock);
}

void date_hwclock_start(void)
{
	rtc_start();
}

void date_hwclock_stop(void)
{
	rtc_stop();
}

uint8_t date_timetorun(struct tm *tm_clock, struct debug_t *debug)
{
	time_t clock;
	/*! min at which last time we executed, needed to avoid
	 * re-execution of programs in the same minute.
	 */
	static uint8_t flag = 99;

	clock = gettimeofday();
	tm_clock = gmtime(&clock);

	if (flag != tm_clock->tm_min)
		if ((tm_clock->tm_min == 0) || (tm_clock->tm_min == 30)) {
			debug_print_P(PSTR("Executing programms at "), debug);
			date(debug);
			flag = tm_clock->tm_min;
			return(1);
		}

	return(0);
}
