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

/*! \file temperature.h
  \brief temperature functions 
  */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "ogstruct.h"
#include "tcn75.h"
#include "debug.h"

/*! \brief temperature media at boot time */
#define TMEDIA_INIT 15
#define TNOW_INIT -99
#define DFACTOR_INIT 0

/*! \brief temperature media weight.
 *
 * 24h = 1440 minutes
 * wsingle = 1.0 / 1440;
 * wmedia = 1.0 - wsingle;
 */
#define TMEDIA_WSING 0.0007
#define TMEDIA_WALL 0.9993

/*! The formula is:
 * dfactor = (temperature media - TMEDIA_BASE)/TMEDIA_RATIO + 1
 * \note keep this float
 */
#define TMEDIA_RATIO_FS 5.0
#define TMEDIA_RATIO_HS 6.0
#define TMEDIA_RATIO_SW 5.0
/*! \brief temperature at which time is considered ok */
#define TMEDIA_BASE_FS 24.0
#define TMEDIA_BASE_HS 20.0
#define TMEDIA_BASE_SW 15.0

void temperature_update(struct programs_t *progs);
void temperature_print(struct programs_t *progs, struct debug_t *debug);
void temperature_init(void);

#endif
