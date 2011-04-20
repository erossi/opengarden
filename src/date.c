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

        debug_print_P(PSTR("The date is now: "), debug);
	strcpy(debug->line, asctime(date));
	debug_print(debug);
	uart_putchar(0, '\n');

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
}

/*! adjust the internal clock */
void date_init(struct debug_t *debug)
{
	struct tm tm_clock;
	time_t clock = 1299764113;

	rtc_setup(); /* Prepare the HW clock counter */
	date_set(&tm_clock, debug); /* Input the current time. */
	clock = mktime(&tm_clock); /* convert the time into sec. */
	settimeofday(clock); /* set the clock to the current time */
}

/*! print the current time */
void date(struct debug_t *debug)
{
	time_t clock;

	clock = time(NULL);
	strcpy(debug->line, ctime(&clock));
	debug_print(debug);
}

void date_hwclock_start(void)
{
	rtc_start();
}

void date_hwclock_stop(void)
{
	rtc_stop();
}
