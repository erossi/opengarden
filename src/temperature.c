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

/*! \file temperature.c
  */

#include <stdlib.h>
#include "temperature.h"

/*! \brief update the temperature and the media */
void temperature_update(struct programs_t *progs)
{
	progs->tnow = tcn75_read_temperature();
	progs->tmedia = (progs->tmedia * TMEDIA_WALL) + (progs->tnow * TMEDIA_WSING);

	switch (progs->position) {
		case FULLSUN:
			progs->dfactor = (progs->tmedia - TMEDIA_BASE_FS)/TMEDIA_RATIO_FS + 1.0;
			break;
		case HALFSUN:
			progs->dfactor = (progs->tmedia - TMEDIA_BASE_HS)/TMEDIA_RATIO_HS + 1.0;
			break;
		default:
			progs->dfactor = (progs->tmedia - TMEDIA_BASE_SW)/TMEDIA_RATIO_SW + 1.0;
			break;
	}
}

void temperature_print(struct programs_t *progs, struct debug_t *debug)
{
	if (progs->tnow == -99) {
		debug_print_P(PSTR("Not available!\n"), debug);
	} else {
		debug_print_P(PSTR("Temperature: "), debug);
		debug->line = dtostrf(progs->tnow, 3, 5, debug->line);
		debug_print(debug);
		debug_print_P(PSTR(","), debug);
		debug->line = dtostrf(progs->tmedia, 3, 5, debug->line);
		debug_print(debug);
		debug_print_P(PSTR(","), debug);
		debug->line = dtostrf(progs->dfactor, 3, 5, debug->line);
		debug_print(debug);
		debug_print_P(PSTR("\n"), debug);
	}
}

void temperature_init(void)
{
	tcn75_init();
}
