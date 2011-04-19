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

/*! \file cmdli.c
  \brief Command Line Interface
  */

#include <stdlib.h>
#include "cmdli.h"

/*! Clear the cli_t struct */
void cmdli_clear(struct cmdli_t *cmdli)
{
	*(cmdli->cmd) = 0;
	cmdli->idx = 0;
}

/*! initialize the struct cli and allocate memory. */
struct cmdli_t *cmdli_init(struct debug_t *debug)
{
	struct cmdli_t *cmdli;

	cmdli = malloc(sizeof(struct cmdli_t));
	cmdli->cmd = malloc(MAX_CMD_LENGHT);
	cmdli_clear(cmdli);
	cmdli_help(debug);
	return(cmdli);
}

void cmdli_free(struct cmdli_t *cmdli)
{
	free(cmdli->cmd);
	free(cmdli);
}

void cmdli_help(struct debug_t *debug)
{
	debug_print_P(PSTR("\nHelp command:\n"), debug);
	debug_print_P(PSTR("c - clear all programms from memory.\n"), debug);
	debug_print_P(PSTR("dNN - delete program number NN.\n"), debug);
	debug_print_P(PSTR("l - list programms.\n"), debug);
	debug_print_P(PSTR("pHHSSDDOO - HH[0..48] SS[0..48] DD[n/a] OO[oline].\n"), debug);
	debug_print_P(PSTR("s - save programms to EEPROM.\n"), debug);
	debug_print_P(PSTR("t - time status.\n"), debug);
	debug_print_P(PSTR("? - this help screen.\n\n"), debug);
}

void exec_command(char *cmd, struct debug_t *debug)
{
	switch (*cmd) {
		case '?':
			cmdli_help(debug);
			break;
		case 't':
			break;	
		default:
			break;
	}
}

void cmdli_exec(char c, struct cmdli_t *cmdli, struct debug_t *debug)
{
	if (c == '\r') {
		*(cmdli->cmd + cmdli->idx) = 0;
		exec_command(cmdli->cmd, debug);
		cmdli_clear(cmdli);
	} else {
		*(cmdli->cmd + cmdli->idx) = c;

		if ((cmdli->idx++) < MAX_CMD_LENGHT)
			*(cmdli->cmd + cmdli->idx) = 0;
		else
			cmdli_clear(cmdli);
	}
}

