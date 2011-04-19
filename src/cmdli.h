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

/*! \file cmdli.h
  \brief Command line interface.
  */

#ifndef CMDLI_H
#define CMDLI_H

#include "debug.h"

/*! maximum chars a command is made of */
#define MAX_CMD_LENGHT 20

/*! CLI command and flag */
struct cmdli_t {
	/*! received command */
	char *cmd;
	/*! index to store the next char in the string cmd */
	uint8_t idx;
};

struct cmdli_t *cmdli_init(struct debug_t *debug);
void cmdli_free(struct cmdli_t *cmdli);
void cmdli_help(struct debug_t *debug);
void cmdli_exec(char c, struct cmdli_t *cmdli, struct debug_t *debug);

#endif
