/**
 * @file op_popt.h
 * Wrapper for libpopt - always use this rather
 * than popt.h
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_POPT_H
#define OP_POPT_H

#include <popt.h>

#ifndef POPT_TABLEEND
#define POPT_TABLEEND { NULL, '\0', 0, 0, 0, NULL, NULL }
#endif

#ifdef __cplusplus
extern "C" {
#endif

poptContext op_poptGetContext(char const * name,
                int argc, char const ** argv,
                struct poptOption const * options, int flags);

#ifdef __cplusplus
}
#endif

#endif 
