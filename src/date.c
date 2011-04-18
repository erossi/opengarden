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

void setup_date(struct tm *date, struct debug_t *debug) {
        debug_print_P(PSTR("Enter date [YYYYMMDDhhmm]: "), debug);
	debug_get_str(debug->line);
	uart_putchar(0, '\n');
	/* Year */
	strlcpy(debug->string, debug->line, 5);
	date->tm_year = atoi(debug->string) - 1900;
	/* Month */
	strcpy(debug->line, debug->line + 4);
	debug_print(debug);
	uart_putchar(0, '\n');
	strlcpy(debug->string, debug->line, 3);
	date->tm_mon = atoi(debug->string) - 1;
	/* Day */
	strcpy(debug->line, debug->line + 2);
	debug_print(debug);
	uart_putchar(0, '\n');
	strlcpy(debug->string, debug->line, 3);
	date->tm_mday = atoi(debug->string);
	/* Hour */
	strcpy(debug->line, debug->line + 2);
	debug_print(debug);
	uart_putchar(0, '\n');
	strlcpy(debug->string, debug->line, 3);
	date->tm_hour = atoi(debug->string);
	/* Minute */
	strcpy(debug->line, debug->line + 2);
	debug_print(debug);
	uart_putchar(0, '\n');
	strlcpy(debug->string, debug->line, 3);
	date->tm_min = atoi(debug->string);
	date->tm_sec = 0;

	debug->line = asctime(date);
	debug_print(debug);
	uart_putchar(0, '\n');

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
}
