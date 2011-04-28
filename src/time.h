/* time.h - stdlib time conversion routines

  Written By - Johan Knol, johan.knol@iduna.nl

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2, or (at your option) any
  later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

  In other words, you are welcome to use, share and improve this program.
  You are forbidden to forbid anyone else to use, share and improve
  what you give them.   Help stamp out software-hoarding!  
  */

/* Modified by Enrico Rossi */

/*! \file time.h
 * \brief standard libc time handling functions,
 * reduced and adapted to atmega project. */

#ifndef TIME_H
#define TIME_H

#include "rtc.h"

/*!
   A leap year is ((((year%4)==0) && ((year%100)!=0)) || ((year%400)==0)) 
   but since we have no fancy years between 1970 and 2038 we can do:
 */
#define LEAP_YEAR(year) ((year%4)==0)

/*! The standard struct tm. */
struct tm {
	/*! Seconds. [0-60] */
	unsigned char tm_sec;
	/*! Minutes [0-59] */
	unsigned char tm_min;
	/*! Hours [0-23] */
	unsigned char tm_hour;
	/*! Day [1-31] */
	unsigned char tm_mday;
	/*! Month [0-11] */
	unsigned char tm_mon;
	/*! Year since 1900 */
	int tm_year;
	/*! Day of week [0-6] */
	unsigned char tm_wday;
	/*! Days in year [0-365] */
	int tm_yday;
};

/*! seconds since the epoch */
typedef unsigned long time_t;

void settimeofday(const time_t seconds);
time_t gettimeofday(void);
time_t time(time_t *t);
struct tm *gmtime(time_t *timep);
time_t mktime(struct tm *timeptr);
char *asctime(struct tm *timeptr);
char *ctime(time_t *timep);

#endif
