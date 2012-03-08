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

/*! setup program's struct to sane defaults. */
void setup_defaults(struct programs_t *progs)
{
	progs->check = CHECK_VALID_CODE;
	progs->number = 0; /* 0 valid program */
	progs->qc = 0; /* no element in the queue */
	progs->tnow = TNOW_INIT;
	progs->tmedia = TMEDIA_INIT;
	progs->dfactor = DFACTOR_INIT;
	progs->position = FULLSUN;
	progs->valve = BISTABLE;
	progs->log = FALSE;
	/* flags setup */
	progs->flags = FULLSUN; /* sunsite */
	progs->flags |= (_BV(FL_ALRM) & HIGH);
}

void print_program_details(const uint8_t i, struct programs_t *progs, struct debug_t *debug)
{
	sprintf_P(debug->line, PSTR(" %02d,%02d%02d,%03d,%2x,%1x\n"), i, \
			progs->p[i].hstart, progs->p[i].mstart, \
			progs->p[i].dmin, progs->p[i].dow, \
			progs->p[i].oline);
	debug_print(debug);
}

/*! \brief Load or re-load the programs from the eeprom.
 *
 * There are 2 case scenario, one on boot or after a reset,
 * the other after an 'C' command.
 * In the first case we do not have many valid information
 * like all the temperature informations and log status.
 * On the other case instead we have all the information
 * updated.
 */
void prog_load(struct programs_t *progs)
{
	float tnow, tmedia, dfact;
	uint8_t log;

	/* keep this values after the load */
	tnow = progs->tnow;
	tmedia = progs->tmedia;
	dfact = progs->dfactor;
	log = progs->log;

	eeprom_read_block(progs, &EE_progs, sizeof(struct programs_t));

	/* initialize a new programs struct.
	 * if the program stored in flash is not a valid
	 * program then do some defaults.
	 */
	if (progs->check != CHECK_VALID_CODE) {
		progs->check = CHECK_VALID_CODE;
		progs->number = 0; /* 0 valid program */
		progs->position = FULLSUN;
		progs->valve = BISTABLE;
	}

	progs->qc = 0; /* no element in the queue */
	/* recover the valid data preserved. */
	progs->tnow = tnow;
	progs->tmedia = tmedia;
	progs->dfactor = dfact;
	progs->log = log;
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
	/* default temperature infos and log status. */
	setup_defaults(progs);
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
	/* update temperature and dfactor */
	temperature_update(progs);

	/* remember you must not change the temperature or dfactor */
	for (i=0; i<progs->number; i++) {
		if ((progs->p[i].dow & _BV(tm_clock->tm_wday)) &&
				(progs->p[i].hstart == tm_clock->tm_hour) &&
				(progs->p[i].mstart == tm_clock->tm_min)) {

			if (progs->log)
				print_program_details(i, progs, debug);

			q_push(progs, tm_clock, i);
			tm_clock = gmtime(&tnow);
		}
	}
}

/*! list all valid programs */
void prog_list(struct programs_t *progs, struct debug_t *debug)
{
	uint8_t i;

	sprintf_P(debug->line, PSTR("Programs [%02d]\n"), progs->number);
	debug_print(debug);

	for (i = 0; i < progs->number; i++)
		print_program_details(i, progs, debug);
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
		/* get OL */
		strlcpy(substr, s + 13, 2);
		progs->p[progs->number].oline = strtoul(substr, 0, 10);
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

/*! test allarm lines and act accordingly.
 *
 * \bug Bistable valve non-compatible.
 */
uint8_t prog_allarm(struct programs_t *progs)
{
	/* if there is an allarm */
	if (io_in_allarm(progs)) {
		io_out_off(progs); /* close all the lines */
		progs->qc = 0; /* remove all progs in the queue */
		return(1);
	} else {
		return(0);
	}
}
