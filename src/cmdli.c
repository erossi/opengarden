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

/*! \file cmdli.c
  \brief Command Line Interface
  */

#include <stdlib.h>
#include <stdio.h>
#include "cmdli.h"

void sunsite_cmd(struct programs_t *progs, struct debug_t *debug, const char c)
{
	switch (c) {
		case '0': progs->position = FULLSUN;
			  debug_print_P(PSTR("OK\n"), debug);
			  break;
		case '1': progs->position = HALFSUN;
			  debug_print_P(PSTR("OK\n"), debug);
			  break;
		case '2': progs->position = SHADOW;
			  debug_print_P(PSTR("OK\n"), debug);
			  break;
		case 0:
			 switch (progs->position) {
				 case FULLSUN:
					 debug_print_P(PSTR("0 - Full Sun\n"), debug);
					 break;
				 case HALFSUN:
					 debug_print_P(PSTR("1 - Half Sun\n"), debug);
					 break;
				 default:
					 debug_print_P(PSTR("2 - Shadow\n"), debug);
			 }

			 break;
		default:
			 debug_print_P(PSTR("ERROR\n"), debug);
	}
}

void valve_cmd(struct programs_t *progs, struct debug_t *debug, const char c)
{
	switch (c) {
		case '1':
			progs->valve = MONOSTABLE;
			debug_print_P(PSTR("OK\n"), debug);
			break;
		case '2':
			progs->valve = BISTABLE;
			debug_print_P(PSTR("OK\n"), debug);
			break;
		case 0:
			if (progs->valve == MONOSTABLE)
				debug_print_P(PSTR("monostable (1)\n"), debug);
			else
				debug_print_P(PSTR("bistable (2)\n"), debug);
			break;
		default:
			debug_print_P(PSTR("ERROR\n"), debug);
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
	debug_print_P(PSTR("L[0 | 1] - logs OFF/ON\n"), debug);
	debug_print_P(PSTR("pShSm,dtime,DD,OL\n"), debug);
	debug_print_P(PSTR(" where Sh [0..24], Sm [0..60], dtime [000-999], DD [0..FF] OL [0..7]\n"), debug);
	debug_print_P(PSTR("q - queue list.\n"), debug);
	debug_print_P(PSTR("r - load programs from EEPROM.\n"), debug);
	debug_print_P(PSTR("s - save programs to EEPROM.\n"), debug);
	debug_print_P(PSTR("t[YYYYMMDDhhmm] - print or set the date.\n"), debug);
	debug_print_P(PSTR("v - version.\n"), debug);
	debug_print_P(PSTR("V[1 | 2] - Valve type: 1 Monostable, 2 Bistable.\n"), debug);
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
			debug_print_P(PSTR("OK\n"), debug);
			break;
		case 'd':
			/* strip the string from the 1st char */
			if (*(cmd + 1)) {
				date_setrtc(cmd + 1);
				debug_print_P(PSTR("OK\n"), debug);
			} else {
				date_rtc(debug);
			}

			break;
		case 'D':
			tmp = strtoul((cmd+1), 0, 10);

			if (prog_del(progs, tmp))
				debug_print_P(PSTR("OK\n"), debug);
			else
				debug_print_P(PSTR("ERROR\n"), debug);

			break;
		case 'g':
			temperature_print(progs, debug);
			break;
		case 'l':
			prog_list(progs, debug);
			break;
		case 'L':
			switch (*(cmd + 1)) {
				case '0':
					progs->log = FALSE;
					debug_print_P(PSTR("OK\n"), debug);
					break;
				case '1':
					progs->log = TRUE;
					debug_print_P(PSTR("OK\n"), debug);
					break;
				default:
					if (progs->log)
						debug_print_P(PSTR("ON\n"), debug);
					else
						debug_print_P(PSTR("OFF\n"), debug);
			}

			break;
		case 'p':
			prog_add(progs, cmd);
			break;
		case 'q':
			queue_list(progs, debug);
			break;
		case 'r':
			prog_load(progs);
			debug_print_P(PSTR("OK\n"), debug);
			break;
		case 's':
			prog_save(progs);
			debug_print_P(PSTR("OK\n"), debug);
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
		case 'V':
			valve_cmd(progs, debug, *(cmd + 1));
			break;
		case 'y':
			sunsite_cmd(progs, debug, *(cmd + 1));
			break;	
		case 0:
			break;
		default:
			debug_print_P(PSTR("ERROR\n"), debug);
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
	if (c == '\n') {
		/* debug_print_P(PSTR("\n"), debug); */
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
