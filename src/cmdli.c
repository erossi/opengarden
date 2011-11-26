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

void sunsite_print(struct programs_t *progs, struct debug_t *debug)
{
	debug_print_P(PSTR("Sunsite: "), debug);

	switch (progs->position) {
		case FULLSUN:
			debug_print_P(PSTR(" Full Sun (0)\n"), debug);
			break;
		case HALFSUN:
			debug_print_P(PSTR(" Half Sun (1)\n"), debug);
			break;
		default:
			debug_print_P(PSTR(" Shadow (2)\n"), debug);
	}

}

void sunsite_set(struct programs_t *progs, struct debug_t *debug, const char c)
{
	switch (c) {
		case '0': progs->position = FULLSUN;
			  break;
		case '1': progs->position = HALFSUN;
			  break;
		default: progs->position = SHADOW;
	}
}

/*! Clear the cli_t struct */
void cmdli_clear(struct cmdli_t *cmdli)
{
	*(cmdli->cmd) = 0;
	cmdli->idx = 0;
}

/*! initialize the struct cli and allocate memory. */
struct cmdli_t *cmdli_init(struct cmdli_t *cmdli)
{
	temperature_init();
	cmdli = malloc(sizeof(struct cmdli_t));
	cmdli->cmd = malloc(MAX_CMD_LENGHT);
	cmdli_clear(cmdli);
	return(cmdli);
}

/*! Free the struct from memory. */
void cmdli_free(struct cmdli_t *cmdli)
{
	free(cmdli->cmd);
	free(cmdli);
}

/*! Print the help. */
void cmdli_help(struct debug_t *debug)
{
	debug_print_P(PSTR("\nHelp command:\n"), debug);
	debug_print_P(PSTR("C - clear all programs from memory.\n"), debug);
	debug_print_P(PSTR("d[seconds] - print or set the absolute time. TimeZones not supported!\n"), debug);
	debug_print_P(PSTR("DNN - delete program number NN.\n"), debug);
	debug_print_P(PSTR("g - Print the temperature.\n"), debug);
	debug_print_P(PSTR("l - list programs.\n"), debug);
	debug_print_P(PSTR("pShSm,dtime,DD,OL\n"), debug);
	debug_print_P(PSTR(" where Sh [0..24], Sm [0..60], dtime [000-999], DD [0..FF] OL [0..7]\n"), debug);
	debug_print_P(PSTR("r - re-load programs from EEPROM.\n"), debug);
	debug_print_P(PSTR("s - save programs to EEPROM.\n"), debug);
	debug_print_P(PSTR("t[YYYYMMDDhhmm] - print or set the date.\n"), debug);
	debug_print_P(PSTR("v - version.\n"), debug);
	debug_print_P(PSTR("y[0..2] - print or set the sun site.\n"), debug);
	debug_print_P(PSTR("? - this help screen.\n\n"), debug);
}

/*! Execute an input command:
 * \param cmd char with the command,
 * \param progs the programs structure,
 * \param debug used to print things.
 */
void cmdli_run(char *cmd, struct programs_t *progs, struct debug_t *debug)
{
	uint8_t tmp;

	switch (*cmd) {
		case '?':
			cmdli_help(debug);
			break;
		case 'C':
			prog_clear(progs);
			debug_print_P(PSTR("All programs has been removed.\n"), debug);
			break;
		case 'd':
			/* strip the string from the 1st char */
			if (*(cmd + 1))
				date_setrtc(cmd + 1);
			else
				date(debug);
			break;
		case 'D':
			tmp = strtoul((cmd+1), 0, 10);
			prog_del(progs, tmp);
			sprintf_P(debug->line, PSTR("The program %d has been removed.\n"), tmp);
			debug_print(debug);
			break;
		case 'g':
			temperature_print(progs, debug);
			break;
		case 'l':
			prog_list(progs, debug);
			break;
		case 'p':
			prog_add(progs, cmd);
			break;
		case 'r':
			prog_load(progs);
			debug_print_P(PSTR("All programs has been restored.\n"), debug);
			break;
		case 's':
			prog_save(progs);
			debug_print_P(PSTR("All programs has been saved.\n"), debug);
			break;
		case 't':
			/* strip the string from the 1st char */
			if (*(cmd + 1))
				date_set(cmd + 1, debug);
			else
				date(debug);
			break;	
		case 'v':
			debug_version(debug);
			break;
		case 'y':
			if (*(cmd + 1))
				sunsite_set(progs, debug, *(cmd + 1));
			else
				sunsite_print(progs, debug);
			break;	
		case 0:
			break;
		default:
			debug_print_P(PSTR("Wrong command! Use '?' to get help\n"), debug);
			break;
	}
}

/*! Get a char from the users and echo it, compose the command line and,
 * if an \r is entered, execute the command.
 * \param c input char,
 * \param cmdli struct with the command line string,
 * \param progs the programs structure,
 * \param debug needed for printing things.
 */
void cmdli_exec(char c, struct cmdli_t *cmdli, struct programs_t *progs, struct debug_t *debug)
{
	if (c == '\r') {
		debug_print_P(PSTR("\n"), debug);
		*(cmdli->cmd + cmdli->idx) = 0;
		cmdli_run(cmdli->cmd, progs, debug);
		cmdli_clear(cmdli);
	} else {
		*(cmdli->cmd + cmdli->idx) = c;

		if ((cmdli->idx++) < MAX_CMD_LENGHT)
			*(cmdli->cmd + cmdli->idx) = 0;
		else
			cmdli_clear(cmdli);
	}
}
