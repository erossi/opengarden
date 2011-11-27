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

/*! \file date.c */

/*! Set the date from the command line in a human form.
 * \param cmd the string in the format YYYYMMDDhhmm.
 * \param debug printing and string stuff.
 */
void date_set(char *cmd, struct debug_t *debug)
{
	struct tm *date;
	time_t sec;

	date = malloc(sizeof(struct tm));

	/* Year */
	strlcpy(debug->string, cmd, 5);
	date->tm_year = atoi(debug->string) - 1900;
	/* Month */
	strlcpy(debug->string, cmd + 4, 3);
	date->tm_mon = atoi(debug->string) - 1;
	/* Day */
	strlcpy(debug->string, cmd + 6, 3);
	date->tm_mday = atoi(debug->string);
	/* Hour */
	strlcpy(debug->string, cmd + 8, 3);
	date->tm_hour = atoi(debug->string);
	/* Minute */
	strlcpy(debug->string, cmd + 10, 3);
	date->tm_min = atoi(debug->string);
	date->tm_sec = 0;

	sec = mktime(date);
	settimeofday(sec);

	free(date);
}

/*! Extract abstime from the command and set it to the RTC clock */
void date_setrtc(char *cmd)
{
	time_t sec;

	sec = strtoul(cmd, NULL, 10);
	settimeofday(sec);
}

/*! print the current rtc time */
void date_rtc(struct debug_t *debug)
{
	time_t clock;

	clock = time(NULL);
	debug->line = ultoa(clock, debug->line, 10);
	debug_print(debug);
	debug_print_P(PSTR("\n"), debug);
}

/*! print the current time */
void date(struct debug_t *debug)
{
	time_t clock;

	clock = time(NULL);
	strcpy(debug->line, ctime(&clock));
	debug_print(debug);
	debug_print_P(PSTR(" ("), debug);
	debug->line = ultoa(clock, debug->line, 10);
	debug_print(debug);
	debug_print_P(PSTR(")\n"), debug);
}

/*! adjust the internal clock */
struct tm *date_init(struct tm *tm_clock, struct debug_t *debug)
{
	time_t clock = 1318612980;

	/* this should set the pointer to the
	 * preallocated area of lastTime.
	 */
	tm_clock = gmtime(&clock);

	rtc_setup(); /* Prepare the HW clock counter */
	settimeofday(clock); /* set the clock to the current time */

        debug_print_P(PSTR("The date is now: "), debug);
	date(debug);

	return(tm_clock);
}

/*! free the allocated structure */
void date_free(struct tm *tm_clock)
{
	free(tm_clock);
}

/*! start the hardware clock now, high level call to rtc start. */
void date_hwclock_start(void)
{
	rtc_start();
}

/*! stop the hardware clock */
void date_hwclock_stop(void)
{
	rtc_stop();
}

/*!
 * \brief check if it is the time to run a program.
 *
 * Every time date_timetorun is called, it has stored in a
 * static variable the last time as minute we activate a check
 * trough all the programs to see if some has to start or to stop.
 * If the time (minutes) are the same stored, then for this minute we
 * are done and exit, else, if for this minute we did not check
 * the programs list, do it.
 * \param *tm_clock The time.
 * \param debug
 * \return true - Execute, false - don't
 * \note any call updates the struct tm to now.
 */
uint8_t date_timetorun(struct tm *tm_clock, struct debug_t *debug)
{
	time_t clock;
	/*! static store at which last time (minutes)
	 * we run programs, needed to avoid
	 * re-execution of programs in the same minute.
	 */
	static uint8_t flag = 99;

	clock = gettimeofday();
	tm_clock = gmtime(&clock);

	if (flag != tm_clock->tm_min) {
		flag = tm_clock->tm_min;
		return(1);
	} else {
		return(0);
	}
}
