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

/*! \file ogstruct.h
  \brief structures.
  */

#ifndef OGSTR_H
#define OGSTR_H

#include "date.h"
#include "debug.h"

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
#define CHECK_VALID_CODE 0x07
/*! \brief maximum number of programs */
#define MAX_PROGS 20
/*! Maximum increment factor for time increase. */
#define PROG_MAX_FACTOR 3.0
/*! Factor increment for tomorrow program reschedule. */
#define PROG_TOMORROW_FACTOR 2.0

/*! queue status NEW */
#define Q_NEW 0
/*! queue status OFF */
#define Q_OFF 1
/*! queue status RUN */
#define Q_RUN 2
/*! queue status DELAYED */
#define Q_DELAYED 3

/*! sunlight position. */
#define FULLSUN 0
/*! sunlight position. */
#define HALFSUN 1
/*! sunlight position. */
#define SHADOW 2

/*! valve type */
#define MONOSTABLE 1
/*! valve type */
#define BISTABLE 2

/*! flag sunsite 2 bit (0, 1) */
#define FL_SUNSITE 0
/*! flag valve type mono/bi-stable */
#define FL_VTYPE 2
/*! flag log enable/disable */
#define FL_LOG 3
/*! flag alarm level High or Low */
#define FL_LEVEL 4
/*! flag leds ON or OFF */
#define FL_LED 5
/*! flag alarm active */
#define FL_ALRM 6

/*! Macro FALSE */
#define FALSE 0
/*! Macro TRUE */
#define TRUE 1
/*! Macro LOW */
#define LOW 0
/*! Macro HIGH */
#define HIGH 1
/*! Macro OFF */
#define OFF 0
/*! Macro ON */
#define ON 1

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
	/*! queue programs 0..(MAX_PROGS - 1)*/
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
	/*! see FL_ definition for this bit mapped byte flag. */
	uint8_t flags;
	/*! \brief I/O line in use.
	 * In bistable valve type, we store the line in use
	 * so it can be possible to disable such line once opened
	 * in order to close only the correct line.
	 */
	uint8_t ioline;
};


void flag_set(struct programs_t *progs, const uint8_t bit,
		const uint8_t val);
uint8_t flag_get(struct programs_t *progs, const uint8_t bit);

#endif
