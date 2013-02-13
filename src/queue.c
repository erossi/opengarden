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

/*! \file queue.c
 * \brief queue functions.
 */

/*! add a stack ? add the 1 time ram only flag? */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "program.h"

/*! print the status of a queue element.
 *
 * \param progs
 * \param debug
 * \param index the indexed element of the queue.
 */
void print_qstatus(struct programs_t *progs, struct debug_t *debug, const uint8_t index)
{
	switch (progs->q[index].status) {
		case Q_NEW:
			debug_print_P(PSTR("new"), debug);
			break;
		case Q_OFF:
			debug_print_P(PSTR("off"), debug);
			break;
		case Q_RUN:
			debug_print_P(PSTR("run"), debug);
			break;
		case Q_DELAYED:
			debug_print_P(PSTR("delayed"), debug);
			break;
		default:
			debug_print_P(PSTR("ERROR"), debug);
	}
}

/*! print a single queue line.
 */
void print_qline(struct programs_t *progs, struct debug_t *debug, const uint8_t index)
{
	if (flag_get(progs, FL_LOG)) {
		sprintf_P(debug->line, PSTR(" %10lu,%10lu,%1x,"), \
				progs->q[index].start, \
				progs->q[index].stop, \
				progs->q[index].oline);
		debug_print(debug);
		print_qstatus(progs, debug, index);
		debug_print_P(PSTR(" -> "), debug);
	}
}

/*! \brief queue a program to be executed.
 * \param progs the programs struct.
 * \param tm_clock the time.
 * \param i the program number to be pushed into the queue.
 */
void q_push(struct programs_t *progs, struct tm *tm_clock, const uint8_t i)
{
	time_t tnow, tend;
	uint8_t tomorrow;
	float dfactor;

	/* now in seconds */
	tnow = mktime(tm_clock);

	/* save dfactor value */
	dfactor = progs->dfactor;

	/* read temperature, calculate drift factor
	 * based on temperature.
	 * if tdelta * drift > max_irrigation_time, then
	 * schedule for tomorrow,
	 * else apply drift factor and store the new
	 * end of the program.
	 */
	if (dfactor > PROG_MAX_FACTOR) {
		dfactor = PROG_MAX_FACTOR;
		/* set tomorrow */
		tomorrow = _BV(tm_clock->tm_wday) << 1;

		/* Saturday case */
		if (tomorrow > 32)
			tomorrow = 1;

		/* if the program does not run tomorrow */
		if (!(progs->p[i].dow & tomorrow)) {
			tend = tnow + (unsigned long int)(progs->p[i].dmin * 60.0 * PROG_TOMORROW_FACTOR);

			if (progs->qc < (MAX_PROGS - 1)) {
				progs->q[progs->qc].start = tnow + 86400l;
				progs->q[progs->qc].stop = tend + 86400l;
				progs->q[progs->qc].oline = progs->p[i].oline;
				progs->q[progs->qc].status = Q_NEW;
				progs->qc++;
			}
		}
	}

	if (dfactor > 0) {
		tend = tnow + (unsigned long int)(progs->p[i].dmin * 60.0 * dfactor);

		if (progs->qc < (MAX_PROGS - 1)) {
			progs->q[progs->qc].start = tnow;
			progs->q[progs->qc].stop = tend;
			progs->q[progs->qc].oline = progs->p[i].oline;
			progs->q[progs->qc].status = Q_NEW;
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

/*! Remove Q_OFF element from the queue.
 */
void q_purge(struct programs_t *progs)
{
	uint8_t i = 0;

	while (i<progs->qc)
		if (progs->q[i].status == Q_OFF)
			q_pop(progs, i);
		else
			i++;
}

/*! Open the oline of the indexed queue element.
 *
 * Function to recalculate timing and open the oline relative
 * to the indexed element in the queue.
 *
 * \param progs
 * \param tnow the time
 * \param index the indexed element.
 */
void run(struct programs_t *progs, const time_t tnow, const uint8_t index)
{
	/* recalculate tstart and tend */
	progs->q[index].stop += tnow - progs->q[index].start;
	progs->q[index].start = tnow;
	progs->q[index].status = Q_RUN;
	io_out_set(progs->q[index].oline, ON, progs->valve);
}

/*! Check which program in the queue to exec.
 * \param progs
 * \param tm_clock time now.
 * \param debug
 * \bug With bistable valve opening and closing oline cannot be done at the same time, any io-action must be done on separate queue-run.
 * \note the printed infos rappresent the status of a queue before.
 */
void queue_run(struct programs_t *progs, struct tm *tm_clock, struct debug_t *debug)
{
	uint8_t i, exit;
	time_t tnow;

	tnow = mktime(tm_clock);
	i = 0;
	exit = FALSE;

	while ((i<progs->qc) && (exit == FALSE)) {
		print_qline(progs, debug, i);

		if (progs->q[i].start <= tnow) {
			switch (progs->q[i].status) {
				case Q_NEW:
					if (io_line_in_use()) {
						progs->q[i].status = Q_DELAYED;
					} else {
						run(progs, tnow, i);
						/* force exit */
						exit = TRUE;
					}

					break;
				case Q_RUN:
					if (progs->q[i].stop <= tnow) {
						progs->q[i].status = Q_OFF;
						io_out_set(progs->q[i].oline, OFF, progs->valve);
						/* force exit */
						exit = TRUE;
					}

					break;
				case Q_DELAYED:
					if (!io_line_in_use()) {
						run(progs, tnow, i);
						/* force exit */
						exit = TRUE;
					}

					break;
				case Q_OFF:
				default:
					break;
			}
		}

		if (flag_get(progs, FL_LOG)) {
			print_qstatus(progs, debug, i);
			debug_print_P(PSTR("\n"), debug);
		}

		i++;
	}

	/* purge all Q_OFF elements */
	q_purge(progs);
}

/*! list all valid programs */
void queue_list(struct programs_t *progs, struct debug_t *debug)
{
	uint8_t i;

	sprintf_P(debug->line, PSTR("Queue [%02d]\n"), progs->qc);
	debug_print(debug);

	for (i = 0; i < progs->qc; i++) {
		print_qline(progs, debug, i);
		debug_print_P(PSTR("\n"), debug);
	}
}
