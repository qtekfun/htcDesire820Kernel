/**
 * @file daemon/opd_mangling.h
 * Mangling and opening of sample files
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_MANGLING_H
#define OPD_MANGLING_H

#include "odb.h"

struct sfile;

int opd_open_sample_file(odb_t *file, struct sfile *last,
                         struct sfile * sf, int counter, int cg);

#endif 
