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

/*! \file queue.c
  \brief queue functions
  */

/*! add a stack ? add the 1 time ram only flag? */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "program.h"

/* \brief queue a program to be executed. */
void q_push(struct programs_t *progs, struct tm *tm_clock, const uint8_t i)
{
	time_t tnow, tend;
	uint8_t tomorrow;

	/* now in seconds */
	tnow = mktime(tm_clock);

	/* read temperature, calculate drift factor
	 * based on temperature.
	 * if tdelta * drift > max_irrigation_time, then
	 * schedule for tomorrow,
	 * else apply drift factor and store the new
	 * end of the program.
	 */
	if (progs->dfactor > PROG_MAX_FACTOR) {
		progs->dfactor = PROG_MAX_FACTOR;
		/* set tomorrow */
		tomorrow = _BV(tm_clock->tm_wday) << 1;

		/* Saturday case */
		if (tomorrow > 32)
			tomorrow = 1;

		/* if the program does not run tomorrow */
		if (!(progs->p[i].dow & tomorrow)) {
			tend = tnow + (int)(progs->p[i].dmin * 60.0 * PROG_TOMORROW_FACTOR);

			if (progs->qc < (MAX_PROGS - 1)) {
				progs->q[progs->qc].time = tnow + 86400l;
				progs->q[progs->qc].oline = progs->p[i].oline;
				progs->q[progs->qc].flag = 1;
				progs->qc++;
				progs->q[progs->qc].time = tend + 86400l;
				progs->q[progs->qc].oline = progs->p[i].oline;
				progs->q[progs->qc].flag = 0;
				progs->qc++;
			}
		}
	}

	if (progs->dfactor > 0) {
		tend = tnow + (int)(progs->p[i].dmin * 60.0 * progs->dfactor);

		if (progs->qc < (MAX_PROGS - 1)) {
			progs->q[progs->qc].time = 0;
			progs->q[progs->qc].oline = progs->p[i].oline;
			progs->q[progs->qc].flag = 1;
			progs->qc++;
			progs->q[progs->qc].time = tend;
			progs->q[progs->qc].oline = progs->p[i].oline;
			progs->q[progs->qc].flag = 0;
			progs->qc++;
		}
	}
}

/*! \brief pop the next program from the queue */
void q_pop(struct programs_t *progs, const uint8_t i)
{
	uint8_t j;

	for (j=i+1; j<progs->qc; j++)
		progs->q[j-1] = progs->q[j];

	progs->qc--;
}

/*! Check which program in the queue to exec.
 * \param progs
 * \param tm_clock time now.
 * \param debug
 */
void queue_run(struct programs_t *progs, struct tm *tm_clock, struct debug_t *debug)
{
	uint8_t i;
	time_t tnow;

	tnow = mktime(tm_clock);
	i = 0;

	while (i<progs->qc) {
		sprintf_P(debug->line, PSTR(" time: %10lu, oline: %2x, status: "), progs->q[i].time, progs->q[i].oline);
		debug_print(debug);

		if (progs->q[i].time <= tnow) {
			switch (progs->q[i].flag) {
				case 0:
					debug_print_P(PSTR("off\n"), debug);
					io_out_change_line(progs->q[i].oline, 0);
					break;
				case 1:
					debug_print_P(PSTR("on\n"), debug);
					io_out_change_line(progs->q[i].oline, 1);
					break;
				default:
					debug_print_P(PSTR("none\n"), debug);
			}

			q_pop(progs, i);
		} else {
			debug_print_P(PSTR("skip\n"), debug);
			i++;
		}
	}
}

/*! list all valid programs */
void queue_list(struct programs_t *progs, struct debug_t *debug)
{
	uint8_t i;

	sprintf_P(debug->line, PSTR("Queue list [%02d]:\n"), progs->qc);
	debug_print(debug);
	debug_print_P(PSTR(" q<number>,<Tstart>,<out line>,<flag>\n"), debug);

	for (i = 0; i < progs->qc; i++) {
		sprintf_P(debug->line, PSTR(" q%02d,%lu,%2x,%2x\n"),i ,progs->q[i].time, progs->q[i].oline, progs->q[i].flag);
		debug_print(debug);
	}
}
