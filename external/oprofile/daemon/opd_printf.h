/**
 * @file daemon/opd_printf.h
 * Output routines
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_PRINTF_H
#define OPD_PRINTF_H

extern int vsfile;
extern int vsamples;
extern int varcs;
extern int vmodule;
extern int vext;
extern int vmisc;

#define verbprintf(x, args...) \
	do { \
		 \
		if (x == 1) \
			printf(args); \
	} while (0)

#endif 
