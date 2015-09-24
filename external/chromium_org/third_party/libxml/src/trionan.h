/*************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 2001 Bjorn Reese <breese@users.sourceforge.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
 * CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
 *
 ************************************************************************/

#ifndef TRIO_NAN_H
#define TRIO_NAN_H

#include "triodef.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
  TRIO_FP_INFINITE,
  TRIO_FP_NAN,
  TRIO_FP_NORMAL,
  TRIO_FP_SUBNORMAL,
  TRIO_FP_ZERO
};

TRIO_PUBLIC double trio_nan TRIO_PROTO((void));

TRIO_PUBLIC double trio_pinf TRIO_PROTO((void));

TRIO_PUBLIC double trio_ninf TRIO_PROTO((void));
  
TRIO_PUBLIC double trio_nzero TRIO_PROTO((TRIO_NOARGS));

TRIO_PUBLIC int trio_isnan TRIO_PROTO((double number));

TRIO_PUBLIC int trio_isinf TRIO_PROTO((double number));

#if 0
	
TRIO_PUBLIC int trio_isfinite TRIO_PROTO((double number));

TRIO_PUBLIC int trio_fpclassify TRIO_PROTO((double number));
#endif

TRIO_PUBLIC int trio_signbit TRIO_PROTO((double number));

TRIO_PUBLIC int trio_fpclassify_and_signbit TRIO_PROTO((double number, int *is_negative));

#ifdef __cplusplus
}
#endif

#endif 
