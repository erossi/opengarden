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

/*! \file program.c
  \brief Function to load/unload and exec programs
  */

/*! add a stack ? add the 1 time ram only flag? */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "program.h"

/*! global EEPROM variable */
struct programs_t EEMEM EE_progs;

void prog_load(struct programs_t *progs)
{
	float tmedia;

	/* keep the Tmedia after the load */
	tmedia = progs->tmedia;

	eeprom_read_block(progs, &EE_progs, sizeof(struct programs_t));

	/* initialize a new programs struct */
	if (progs->check != CHECK_VALID_CODE) {
		progs->check = CHECK_VALID_CODE;
		progs->number = 0; /* 0 valid program */
	}

	progs->qc = 0; /* no element in the queue */
	progs->tnow = -99;
	progs->tmedia = tmedia;
	progs->dfactor = 1;
}

/*! \brief Store the programs into the eeprom area */
void prog_save(struct programs_t *progs)
{
	eeprom_update_block(progs, &EE_progs, sizeof(struct programs_t));
}

/*! \brief initialize the program area and IO lines */
struct programs_t *prog_init(struct programs_t *progs)
{
	progs = malloc(sizeof(struct programs_t));
	progs->tmedia = TMEDIA_INIT; /* initalize the Tmedia */
	prog_load(progs);
	io_pin_init();
	return(progs);
}

/*! Free the allocated memory and close the IO lines */
void prog_free(struct programs_t *progs)
{
	/*! \todo shut IO down, to be implemented but useless. */
	free(progs);
}

/*! Check which program to exec.
 * \param progs
 * \param tm_clock time now.
 * \param debug
 */
void prog_run(struct programs_t *progs, struct tm *tm_clock, struct debug_t *debug)
{
	uint8_t i;
	time_t tnow;

	tnow = mktime(tm_clock);
	temperature_update(progs);

	for (i=0; i<progs->number; i++) {
		if ((progs->p[i].dow & _BV(tm_clock->tm_wday)) &&
				(progs->p[i].hstart == tm_clock->tm_hour) &&
				(progs->p[i].mstart == tm_clock->tm_min)) {
			sprintf_P(debug->line, PSTR(" p%02d added to queue\n"), i);
			debug_print(debug);
			q_push(progs, tm_clock, i);
			tm_clock = gmtime(&tnow);
		}
	}
}

/*! list all valid programs */
void prog_list(struct programs_t *progs, struct debug_t *debug)
{
	uint8_t i;

	sprintf_P(debug->line, PSTR("Programs list [%02d]:\n"), progs->number);
	debug_print(debug);
	debug_print_P(PSTR(" p<number>,<start>,<duration>,<DoW>,<out line>\n"), debug);

	for (i = 0; i < progs->number; i++) {
		sprintf_P(debug->line, PSTR(" p%02d,%02d%02d,%03d,%2x,%2x\n"),i ,progs->p[i].hstart, progs->p[i].mstart, progs->p[i].dmin, progs->p[i].dow, progs->p[i].oline);
		debug_print(debug);
	}

	queue_list(progs, debug);
}

/*! remove all programs from the memory */
void prog_clear(struct programs_t *progs)
{
	progs->number = 0;
}

/*! add a program into memory
  \param s string in the form pShSm,shsm,DD,OO
  where
  Sh Start hour in the form 0..24.
  Sm Start minutes.
  sh Stop hour.
  sm Stop minutes.
  DD Day of the week sun..sat bit for day (HEX number).
  OO output line 0..7 bit for line 0 to 7 (HEX number).
 */
void prog_add(struct programs_t *progs, const char *s)
{
	char *substr;

	if (progs->number + 1 < MAX_PROGS) {
		substr = malloc(3);
		/* get Sh, copy from s char 1..2 into substr */
		strlcpy(substr, s + 1, 3);
		progs->p[progs->number].hstart = strtoul(substr, 0, 10);
		/* get Sm, copy from s char 3..4 into substr */
		strlcpy(substr, s + 3, 3);
		progs->p[progs->number].mstart = strtoul(substr, 0, 10);
		/* get duration */
		strlcpy(substr, s + 6, 4);
		progs->p[progs->number].dmin = strtoul(substr, 0, 10);
		/* get DD */
		strlcpy(substr, s + 10, 3);
		progs->p[progs->number].dow = strtoul(substr, 0, 16);
		/* get OO */
		strlcpy(substr, s + 13, 3);
		progs->p[progs->number].oline = strtoul(substr, 0, 16);
		free(substr);
		progs->number++;
	}
}

/*! \brief remove a program from the memory */
uint8_t prog_del(struct programs_t *progs, const uint8_t n)
{
	uint8_t i;

	if (n < progs->number) {
		for (i=n+1; i < progs->number; i++)
			progs->p[i-1] = progs->p[i];

		progs->number--;
		return(1);
	} else {
		return(0);
	}
}

uint8_t prog_allarm(struct programs_t *progs)
{
	/* if there is an allarm */
	if (!io_in_allarm()) {
		io_out_change_line(0xff, 0); /* close all the lines */
		progs->qc = 0; /* remove all progs in the queue */
		return(1);
	} else {
		return(0);
	}
}
