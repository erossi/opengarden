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
 * \brief Utility to handle struct parameters.
 */

#include <stdlib.h>
#include "ogstruct.h"

/*! Set flag value.
 *
 * This function will set or clear the correspondent bit
 * in the flags byte.
 *
 * ex. set the led flag is
 * \code {.c}
 * flag_set(progs, FL_LEDS, TRUE);
 * \endcode
 *
 * ex. clear the led flag value is
 * \code {.c}
 * flag_set(progs, FL_LEDS, FALSE);
 * \endcode
 *
 * \param *progs the ogstruct.
 * \param bit the bit of the flag's byte.
 * \param val TRUE or FALSE.
 */
void flag_set(struct programs_t *progs, const uint8_t bit,
		const uint8_t val)
{
	if (val)
		progs->flags |= _BV(bit);
	else
		progs->flags &= ~_BV(bit);
}

/*! Get flag value.
 *
 * This function will get the correspondent bit
 * in the flags byte.
 *
 * ex. check the led flag value is
 *  if (flag_get(progs, FL_LEDS)) ...
 *
 * \param *progs the ogstruct.
 * \param bit the bit of the flag's byte.
 * \return TRUE or FALSE
 */
uint8_t flag_get(struct programs_t *progs, const uint8_t bit)
{
	if (progs->flags & _BV(bit))
		return(TRUE);
	else
		return(FALSE);
}

/*! set or get the sunsite flag. */
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

/*! set or get the valve type flag. */
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
