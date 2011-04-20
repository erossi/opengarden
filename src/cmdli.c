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
#include <stdio.h>
#include "cmdli.h"

/*! Clear the cli_t struct */
void cmdli_clear(struct cmdli_t *cmdli)
{
	*(cmdli->cmd) = 0;
	cmdli->idx = 0;
}

/*! initialize the struct cli and allocate memory. */
struct cmdli_t *cmdli_init(struct cmdli_t *cmdli, struct debug_t *debug)
{
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
	debug_print_P(PSTR("pHH,SS,DD,OO - HH[0..48] SS[0..48] DD[n/a] OO[oline].\n"), debug);
	debug_print_P(PSTR("r - re-load programms from EEPROM.\n"), debug);
	debug_print_P(PSTR("s - save programms to EEPROM.\n"), debug);
	debug_print_P(PSTR("t - time status.\n"), debug);
	debug_print_P(PSTR("? - this help screen.\n\n"), debug);
}

void exec_command(char *cmd, struct programms_t *progs, struct debug_t *debug)
{
	uint8_t tmp;

	switch (*cmd) {
		case '?':
			cmdli_help(debug);
			break;
		case 'c':
			prog_clear(progs);
			debug_print_P(PSTR("All programms has been removed.\n"), debug);
			break;
		case 'd':
			tmp = strtoul((cmd+1), 0, 10);
			prog_del(progs, tmp);
			sprintf_P(debug->line, PSTR("The program %d has been removed.\n"), tmp);
			debug_print(debug);
			break;
		case 'l':
			prog_list(progs, debug);
			break;
		case 'p':
			prog_add(progs, cmd);
			break;
		case 'r':
			prog_load(progs);
			debug_print_P(PSTR("All programms has been restored.\n"), debug);
			break;
		case 's':
			prog_save(progs);
			debug_print_P(PSTR("All programms has been saved.\n"), debug);
			break;
		case 't':
			debug_print_P(PSTR("The time is: "), debug);
			date(debug);
			break;	
		default:
			debug_print_P(PSTR("Wrong command! Use '?' to get help\n"), debug);
			break;
	}
}

void cmdli_exec(char c, struct cmdli_t *cmdli, struct programms_t *progs, struct debug_t *debug)
{
	if (c == '\r') {
		debug_print_P(PSTR("\n"), debug);
		*(cmdli->cmd + cmdli->idx) = 0;
		exec_command(cmdli->cmd, progs, debug);
		cmdli_clear(cmdli);
	} else {
		*(cmdli->cmd + cmdli->idx) = c;

		if ((cmdli->idx++) < MAX_CMD_LENGHT)
			*(cmdli->cmd + cmdli->idx) = 0;
		else
			cmdli_clear(cmdli);
	}
}

