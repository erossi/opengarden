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

/*! \file program.h
  \brief Function to load/unload and exec programms
  */

#ifndef PROGS_H
#define PROGS_H

#include <avr/eeprom.h>
#include "io_out.h"
#include "debug.h"

/*! check code to control if a valid program is in memeory */
#define CHECK_VALID_CODE 0x7c
/*! maximum number of programms in memory */
#define MAX_PROGS 20

/*! A single program event structure */
struct program_t {
	/*! Days of the week */
	uint8_t dow;
	/*! Output line */
	uint8_t oline;
	/*! start time */
	uint8_t hstart;
	/*! stop time */
	uint8_t hstop;
};

/*! Single structure to keep all the programms */
struct programms_t {
	/*! control code, should be == CHECK_VALID_CODE */
	uint8_t check;
	/*! number of valid programms 0..MAX_PROGS */
	uint8_t number;
	/*! array of the programms 0..(MAX_PROGS - 1)*/
	struct program_t p[MAX_PROGS];
};

/*! global EEPROM variable */
struct programms_t EEMEM *EE_progs;

struct programms_t *prog_init(void);
void prog_save(struct programms_t *progs);
void prog_list(struct programms_t *progs, struct debug_t *debug);
void prog_clear(struct programms_t *progs);
void prog_add(struct programms_t *progs, const char *s);
uint8_t prog_del(struct programms_t *progs, const uint8_t n);

#endif
