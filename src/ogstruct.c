/* This file is part of OpenGarden
 * Copyright (C) 2012 Enrico Rossi
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

/*! \file ogstruct.c
  \brief Utility to handle struct parameters.
  */

#include <stdlib.h>
#include "ogstruct.h"

/*! Set or Get flag value.
 *
 * This function will set or clear the correspondent bit
 * in the flags byte.
 *
 * \example set the leds is
 * flags(progs, '1', FL_LEDS);
 *
 * \param *progs the ogstruct.
 * \param c the value to set or none to get.
 * \param bit the bit of the flag's byte.
 */
uint8_t flags_handle(struct programs_t *progs, const char c,
		const uint8_t bit)
{
	switch (c) {
		/* clear the bit */
		case '0': progs->flags &= ~_BV(bit);
			  break;
			  /* set the bit */
		case '1': progs->flags |= _BV(bit);
			  break;
	}

	if (progs->flags & _BV(bit))
		return(TRUE);
	else
		return(FALSE);
}

/*! Set or Get flag value with printing.
 *
 * This function will set or clear the correspondent bit
 * in the flags byte.
 *
 * \param *progs the ogstruct.
 * \param *debub for printing.
 * \param c the value to set or none to get.
 * \param bit the bit of the flag's byte.
 */
void flags_handle_p(struct programs_t *progs, struct debug_t *debug,
	       const char c, const uint8_t bit)
{
	if (c) {
		flags_handle(progs, c, bit);
		debug_print_P(PSTR("OK\n"), debug);
	} else {
		if (flags_handle(progs, 0, bit))
			debug_print_P(PSTR("ON\n"), debug);
		else
			debug_print_P(PSTR("OFF\n"), debug);
	}
}

/* Incomplete */
void flag_sunsite(struct programs_t *progs, const char c)
{
	switch (c) {
		case '0': progs->position = FULLSUN;
			  break;
		case '1': progs->position = HALFSUN;
			  break;
		case '2': progs->position = SHADOW;
			  break;
		case 0:
			 switch (progs->position) {
				 case FULLSUN:
					 break;
				 case HALFSUN:
					 break;
			 }

			 break;
	}
}

void flag_valve(struct programs_t *progs, const char c)
{
	switch (c) {
		case '1':
			progs->valve = MONOSTABLE;
			break;
		case '2':
			progs->valve = BISTABLE;
			break;
		case 0:
			break;
	}
}
