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

/*! \file ogstruct.h
  \brief structures.
  */

#ifndef OGSTR_H
#define OGSTR_H

#include "date.h"

/*! \brief check code to control if a valid program is in memory.
 *
 * Change this code any time the struct program_t has been changed
 * when reprogram the micro.
 * During boot the program read from flash this code, and, if it is
 * valid, retrive from flash the stored programs.
 * If the structs are changed, we should change this code too, to avoid
 * retrival of wrong data.
 * If, during programming, you nuke the flash memory too, this check
 * code is useless.
 */
#define CHECK_VALID_CODE 0x03
/*! \brief maximum number of programs */
#define MAX_PROGS 20
#define PROG_MAX_FACTOR 3.0
#define PROG_TOMORROW_FACTOR 2.0

/*! queue status */
#define Q_NEW 0
#define Q_OFF 1
#define Q_RUN 2
#define Q_DELAYED 3

/*! sunlight position of the circuit */
#define FULLSUN 0
#define HALFSUN 1
#define SHADOW 2

/*! valve type */
#define MONOSTABLE 1
#define BISTABLE 2

/*! A single program event structure */
struct program_t {
	/*! \brief Days of the week
	 *
	 * equal to _BV(struct tm->tm_wday)
	 * 1 Sunday
	 * 2 Monday
	 * 4 Tuesday
	 * 8 Wednesday
	 * 16 Thursday
	 * 32 Friday
	 * 64 Saturday
	 */
	uint8_t dow;

	/*! \brief Output line from 0 to 7. */
	uint8_t oline;

	/*! \brief start time (hours) */
	uint8_t hstart;
	/*! start time (minutes) */
	uint8_t mstart;
	/*! duration (minutes) */
	uint16_t dmin;
};

/*! \brief the queue buffer */
struct queue_t {
	/*! absolute time to open the oline */
	time_t start;
	/*! absolute time to close the oline */
	time_t stop;
	/*! the output line */
	uint8_t oline;
	/*! the status of the queue */
	uint8_t status;
};

/*! Single structure to keep all the programs */
struct programs_t {
	/*! control code, should be == CHECK_VALID_CODE.
	 *
	 * at any new software release this code is used to
	 * know if the program stored in the flash memory is still
	 * compatibile with the new software.
	 */
	uint8_t check;
	/*! number of valid programs 0..MAX_PROGS */
	uint8_t number;
	/*! number of valid queue elements */
	uint8_t qc;
	/*! array of the programs 0..(MAX_PROGS - 1)*/
	struct program_t p[MAX_PROGS];
	struct queue_t q[MAX_PROGS];
	/*! temperature realtime */
	float tnow;
	/*! temperature media */
	float tmedia;
	/*! Drifting factor */
	float dfactor;
	/*! sunlight position */
	uint8_t position;
	/*! valve type */
	uint8_t valve;
};

#endif
