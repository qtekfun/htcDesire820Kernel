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
 ************************************************************************
 *
 * Functions to handle special quantities in floating-point numbers
 * (that is, NaNs and infinity). They provide the capability to detect
 * and fabricate special quantities.
 *
 * Although written to be as portable as possible, it can never be
 * guaranteed to work on all platforms, as not all hardware supports
 * special quantities.
 *
 * The approach used here (approximately) is to:
 *
 *   1. Use C99 functionality when available.
 *   2. Use IEEE 754 bit-patterns if possible.
 *   3. Use platform-specific techniques.
 *
 ************************************************************************/


#include "triodef.h"
#include "trionan.h"

#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#if defined(TRIO_PLATFORM_UNIX)
# include <signal.h>
#endif
#if defined(TRIO_COMPILER_DECC)
#  if defined(__linux__)
#   include <cpml.h>
#  else
#   include <fp_class.h>
#  endif
#endif
#include <assert.h>

#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_nan.h"
#endif


#define TRIO_TRUE (1 == 1)
#define TRIO_FALSE (0 == 1)

#if defined(__alpha) && !defined(_IEEE_FP)
# if defined(TRIO_COMPILER_DECC)
#  if defined(TRIO_PLATFORM_VMS)
#   error "Must be compiled with option /IEEE_MODE=UNDERFLOW_TO_ZERO/FLOAT=IEEE"
#  else
#   if !defined(_CFE)
#    error "Must be compiled with option -ieee"
#   endif
#  endif
# elif defined(TRIO_COMPILER_GCC) && (defined(__osf__) || defined(__linux__))
#  error "Must be compiled with option -mieee"
# endif
#endif 

#if (FLT_RADIX == 2) && (DBL_MAX_EXP == 1024) && (DBL_MANT_DIG == 53)
# define USE_IEEE_754
#endif



static TRIO_CONST char rcsid[] = "@(#)$Id$";

#if defined(USE_IEEE_754)

#define TRIO_DOUBLE_INDEX(x) (((unsigned char *)&internalEndianMagic)[7-(x)])

#if (defined(__BORLANDC__) && __BORLANDC__ >= 0x0590)
static TRIO_CONST double internalEndianMagic = 7.949928895127362e-275;
#else
static TRIO_CONST double internalEndianMagic = 7.949928895127363e-275;
#endif

static TRIO_CONST unsigned char ieee_754_exponent_mask[] = {
  0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static TRIO_CONST unsigned char ieee_754_mantissa_mask[] = {
  0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static TRIO_CONST unsigned char ieee_754_sign_mask[] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static TRIO_CONST unsigned char ieee_754_negzero_array[] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static TRIO_CONST unsigned char ieee_754_infinity_array[] = {
  0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static TRIO_CONST unsigned char ieee_754_qnan_array[] = {
  0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



TRIO_PRIVATE double
trio_make_double
TRIO_ARGS1((values),
	   TRIO_CONST unsigned char *values)
{
  TRIO_VOLATILE double result;
  int i;

  for (i = 0; i < (int)sizeof(double); i++) {
    ((TRIO_VOLATILE unsigned char *)&result)[TRIO_DOUBLE_INDEX(i)] = values[i];
  }
  return result;
}

TRIO_PRIVATE int
trio_is_special_quantity
TRIO_ARGS2((number, has_mantissa),
	   double number,
	   int *has_mantissa)
{
  unsigned int i;
  unsigned char current;
  int is_special_quantity = TRIO_TRUE;

  *has_mantissa = 0;

  for (i = 0; i < (unsigned int)sizeof(double); i++) {
    current = ((unsigned char *)&number)[TRIO_DOUBLE_INDEX(i)];
    is_special_quantity
      &= ((current & ieee_754_exponent_mask[i]) == ieee_754_exponent_mask[i]);
    *has_mantissa |= (current & ieee_754_mantissa_mask[i]);
  }
  return is_special_quantity;
}

TRIO_PRIVATE int
trio_is_negative
TRIO_ARGS1((number),
	   double number)
{
  unsigned int i;
  int is_negative = TRIO_FALSE;

  for (i = 0; i < (unsigned int)sizeof(double); i++) {
    is_negative |= (((unsigned char *)&number)[TRIO_DOUBLE_INDEX(i)]
		    & ieee_754_sign_mask[i]);
  }
  return is_negative;
}

#endif 


TRIO_PUBLIC double
trio_nzero(TRIO_NOARGS)
{
#if defined(USE_IEEE_754)
  return trio_make_double(ieee_754_negzero_array);
#else
  TRIO_VOLATILE double zero = 0.0;

  return -zero;
#endif
}

TRIO_PUBLIC double
trio_pinf(TRIO_NOARGS)
{
  
  static double result = 0.0;

  if (result == 0.0) {
    
#if defined(INFINITY) && defined(__STDC_IEC_559__)
    result = (double)INFINITY;

#elif defined(USE_IEEE_754)
    result = trio_make_double(ieee_754_infinity_array);

#else
# if defined(TRIO_PLATFORM_UNIX)
    void (*signal_handler)(int) = signal(SIGFPE, SIG_IGN);
# endif

    result = HUGE_VAL;
    if (HUGE_VAL == DBL_MAX) {
      
      result += HUGE_VAL;
    }
    
# if defined(TRIO_PLATFORM_UNIX)
    signal(SIGFPE, signal_handler);
# endif

#endif
  }
  return result;
}

TRIO_PUBLIC double
trio_ninf(TRIO_NOARGS)
{
  static double result = 0.0;

  if (result == 0.0) {
    result = -trio_pinf();
  }
  return result;
}

TRIO_PUBLIC double
trio_nan(TRIO_NOARGS)
{
  
  static double result = 0.0;

  if (result == 0.0) {
    
#if defined(TRIO_COMPILER_SUPPORTS_C99)
    result = nan("");

#elif defined(NAN) && defined(__STDC_IEC_559__)
    result = (double)NAN;
  
#elif defined(USE_IEEE_754)
    result = trio_make_double(ieee_754_qnan_array);

#else
# if defined(TRIO_PLATFORM_UNIX)
    void (*signal_handler)(int) = signal(SIGFPE, SIG_IGN);
# endif
    
    result = trio_pinf() / trio_pinf();
    
# if defined(TRIO_PLATFORM_UNIX)
    signal(SIGFPE, signal_handler);
# endif
    
#endif
  }
  return result;
}

TRIO_PUBLIC int
trio_isnan
TRIO_ARGS1((number),
	   double number)
{
#if (defined(TRIO_COMPILER_SUPPORTS_C99) && defined(isnan)) \
 || defined(TRIO_COMPILER_SUPPORTS_UNIX95)
  return isnan(number);
  
#elif defined(TRIO_COMPILER_MSVC) || defined(TRIO_COMPILER_BCB)
  return _isnan(number) ? TRIO_TRUE : TRIO_FALSE;

#elif defined(USE_IEEE_754)
  int has_mantissa;
  int is_special_quantity;

  is_special_quantity = trio_is_special_quantity(number, &has_mantissa);
  
  return (is_special_quantity && has_mantissa);
  
#else
  int status;
  double integral, fraction;
  
# if defined(TRIO_PLATFORM_UNIX)
  void (*signal_handler)(int) = signal(SIGFPE, SIG_IGN);
# endif
  
  status = (
	    ((TRIO_VOLATILE double)number != (TRIO_VOLATILE double)number) ||
	    ((number != 0.0) &&
	     (fraction = modf(number, &integral),
	      integral == fraction)));
  
# if defined(TRIO_PLATFORM_UNIX)
  signal(SIGFPE, signal_handler);
# endif
  
  return status;
  
#endif
}

TRIO_PUBLIC int
trio_isinf
TRIO_ARGS1((number),
	   double number)
{
#if defined(TRIO_COMPILER_DECC) && !defined(__linux__)
  return ((fp_class(number) == FP_POS_INF)
	  ? 1
	  : ((fp_class(number) == FP_NEG_INF) ? -1 : 0));

#elif defined(isinf)
  return isinf(number)
    ? ((number > 0.0) ? 1 : -1)
    : 0;
  
#elif defined(TRIO_COMPILER_MSVC) || defined(TRIO_COMPILER_BCB)
  return ((_fpclass(number) == _FPCLASS_PINF)
	  ? 1
	  : ((_fpclass(number) == _FPCLASS_NINF) ? -1 : 0));

#elif defined(USE_IEEE_754)
  int has_mantissa;
  int is_special_quantity;

  is_special_quantity = trio_is_special_quantity(number, &has_mantissa);
  
  return (is_special_quantity && !has_mantissa)
    ? ((number < 0.0) ? -1 : 1)
    : 0;

#else
  int status;
  
# if defined(TRIO_PLATFORM_UNIX)
  void (*signal_handler)(int) = signal(SIGFPE, SIG_IGN);
# endif
  
  double infinity = trio_pinf();
  
  status = ((number == infinity)
	    ? 1
	    : ((number == -infinity) ? -1 : 0));
  
# if defined(TRIO_PLATFORM_UNIX)
  signal(SIGFPE, signal_handler);
# endif
  
  return status;
  
#endif
}

#if 0
	
TRIO_PUBLIC int
trio_isfinite
TRIO_ARGS1((number),
	   double number)
{
#if defined(TRIO_COMPILER_SUPPORTS_C99) && defined(isfinite)
  return isfinite(number);
  
#elif defined(TRIO_COMPILER_MSVC) || defined(TRIO_COMPILER_BCB)
  return _finite(number);

#elif defined(USE_IEEE_754)
  int dummy;

  return (! trio_is_special_quantity(number, &dummy));

#else
  return ((trio_isinf(number) == 0) && (trio_isnan(number) == 0));
  
#endif
}

#endif

TRIO_PUBLIC int
trio_fpclassify_and_signbit
TRIO_ARGS2((number, is_negative),
	   double number,
	   int *is_negative)
{
#if defined(fpclassify) && defined(signbit)
  *is_negative = signbit(number);
  switch (fpclassify(number)) {
  case FP_NAN:
    return TRIO_FP_NAN;
  case FP_INFINITE:
    return TRIO_FP_INFINITE;
  case FP_SUBNORMAL:
    return TRIO_FP_SUBNORMAL;
  case FP_ZERO:
    return TRIO_FP_ZERO;
  default:
    return TRIO_FP_NORMAL;
  }

#else
# if defined(TRIO_COMPILER_DECC)
#  define TRIO_FPCLASSIFY(n) fp_class(n)
#  define TRIO_QUIET_NAN FP_QNAN
#  define TRIO_SIGNALLING_NAN FP_SNAN
#  define TRIO_POSITIVE_INFINITY FP_POS_INF
#  define TRIO_NEGATIVE_INFINITY FP_NEG_INF
#  define TRIO_POSITIVE_SUBNORMAL FP_POS_DENORM
#  define TRIO_NEGATIVE_SUBNORMAL FP_NEG_DENORM
#  define TRIO_POSITIVE_ZERO FP_POS_ZERO
#  define TRIO_NEGATIVE_ZERO FP_NEG_ZERO
#  define TRIO_POSITIVE_NORMAL FP_POS_NORM
#  define TRIO_NEGATIVE_NORMAL FP_NEG_NORM
  
# elif defined(TRIO_COMPILER_MSVC) || defined(TRIO_COMPILER_BCB)
#  define TRIO_FPCLASSIFY(n) _fpclass(n)
#  define TRIO_QUIET_NAN _FPCLASS_QNAN
#  define TRIO_SIGNALLING_NAN _FPCLASS_SNAN
#  define TRIO_POSITIVE_INFINITY _FPCLASS_PINF
#  define TRIO_NEGATIVE_INFINITY _FPCLASS_NINF
#  define TRIO_POSITIVE_SUBNORMAL _FPCLASS_PD
#  define TRIO_NEGATIVE_SUBNORMAL _FPCLASS_ND
#  define TRIO_POSITIVE_ZERO _FPCLASS_PZ
#  define TRIO_NEGATIVE_ZERO _FPCLASS_NZ
#  define TRIO_POSITIVE_NORMAL _FPCLASS_PN
#  define TRIO_NEGATIVE_NORMAL _FPCLASS_NN
  
# elif defined(FP_PLUS_NORM)
#  if defined(TRIO_PLATFORM_AIX)
#   if defined(__cplusplus)
#    define TRIO_FPCLASSIFY(n) _class(n)
#   else
#    define TRIO_FPCLASSIFY(n) class(n)
#   endif
#  else
#   define TRIO_FPCLASSIFY(n) fpclassify(n)
#  endif
#  define TRIO_QUIET_NAN FP_QNAN
#  define TRIO_SIGNALLING_NAN FP_SNAN
#  define TRIO_POSITIVE_INFINITY FP_PLUS_INF
#  define TRIO_NEGATIVE_INFINITY FP_MINUS_INF
#  define TRIO_POSITIVE_SUBNORMAL FP_PLUS_DENORM
#  define TRIO_NEGATIVE_SUBNORMAL FP_MINUS_DENORM
#  define TRIO_POSITIVE_ZERO FP_PLUS_ZERO
#  define TRIO_NEGATIVE_ZERO FP_MINUS_ZERO
#  define TRIO_POSITIVE_NORMAL FP_PLUS_NORM
#  define TRIO_NEGATIVE_NORMAL FP_MINUS_NORM
# endif

# if defined(TRIO_FPCLASSIFY)
  switch (TRIO_FPCLASSIFY(number)) {
  case TRIO_QUIET_NAN:
  case TRIO_SIGNALLING_NAN:
    *is_negative = TRIO_FALSE; 
    return TRIO_FP_NAN;
  case TRIO_POSITIVE_INFINITY:
    *is_negative = TRIO_FALSE;
    return TRIO_FP_INFINITE;
  case TRIO_NEGATIVE_INFINITY:
    *is_negative = TRIO_TRUE;
    return TRIO_FP_INFINITE;
  case TRIO_POSITIVE_SUBNORMAL:
    *is_negative = TRIO_FALSE;
    return TRIO_FP_SUBNORMAL;
  case TRIO_NEGATIVE_SUBNORMAL:
    *is_negative = TRIO_TRUE;
    return TRIO_FP_SUBNORMAL;
  case TRIO_POSITIVE_ZERO:
    *is_negative = TRIO_FALSE;
    return TRIO_FP_ZERO;
  case TRIO_NEGATIVE_ZERO:
    *is_negative = TRIO_TRUE;
    return TRIO_FP_ZERO;
  case TRIO_POSITIVE_NORMAL:
    *is_negative = TRIO_FALSE;
    return TRIO_FP_NORMAL;
  case TRIO_NEGATIVE_NORMAL:
    *is_negative = TRIO_TRUE;
    return TRIO_FP_NORMAL;
  default:
    
    *is_negative = (number < 0.0);
    return TRIO_FP_NORMAL;
  }
  
# else
  int rc;
  
  if (number == 0.0) {
#  if defined(USE_IEEE_754)
    *is_negative = trio_is_negative(number);
#  else
    *is_negative = TRIO_FALSE; 
#  endif
    return TRIO_FP_ZERO;
  }
  if (trio_isnan(number)) {
    *is_negative = TRIO_FALSE;
    return TRIO_FP_NAN;
  }
  if ((rc = trio_isinf(number))) {
    *is_negative = (rc == -1);
    return TRIO_FP_INFINITE;
  }
  if ((number > 0.0) && (number < DBL_MIN)) {
    *is_negative = TRIO_FALSE;
    return TRIO_FP_SUBNORMAL;
  }
  if ((number < 0.0) && (number > -DBL_MIN)) {
    *is_negative = TRIO_TRUE;
    return TRIO_FP_SUBNORMAL;
  }
  *is_negative = (number < 0.0);
  return TRIO_FP_NORMAL;
  
# endif
#endif
}

TRIO_PUBLIC int
trio_signbit
TRIO_ARGS1((number),
	   double number)
{
  int is_negative;
  
  (void)trio_fpclassify_and_signbit(number, &is_negative);
  return is_negative;
}

#if 0
	
TRIO_PUBLIC int
trio_fpclassify
TRIO_ARGS1((number),
	   double number)
{
  int dummy;
  
  return trio_fpclassify_and_signbit(number, &dummy);
}

#endif


#if defined(STANDALONE)
# include <stdio.h>

static TRIO_CONST char *
getClassification
TRIO_ARGS1((type),
	   int type)
{
  switch (type) {
  case TRIO_FP_INFINITE:
    return "FP_INFINITE";
  case TRIO_FP_NAN:
    return "FP_NAN";
  case TRIO_FP_NORMAL:
    return "FP_NORMAL";
  case TRIO_FP_SUBNORMAL:
    return "FP_SUBNORMAL";
  case TRIO_FP_ZERO:
    return "FP_ZERO";
  default:
    return "FP_UNKNOWN";
  }
}

static void
print_class
TRIO_ARGS2((prefix, number),
	   TRIO_CONST char *prefix,
	   double number)
{
  printf("%-6s: %s %-15s %g\n",
	 prefix,
	 trio_signbit(number) ? "-" : "+",
	 getClassification(TRIO_FPCLASSIFY(number)),
	 number);
}

int main(TRIO_NOARGS)
{
  double my_nan;
  double my_pinf;
  double my_ninf;
# if defined(TRIO_PLATFORM_UNIX)
  void (*signal_handler) TRIO_PROTO((int));
# endif

  my_nan = trio_nan();
  my_pinf = trio_pinf();
  my_ninf = trio_ninf();

  print_class("Nan", my_nan);
  print_class("PInf", my_pinf);
  print_class("NInf", my_ninf);
  print_class("PZero", 0.0);
  print_class("NZero", -0.0);
  print_class("PNorm", 1.0);
  print_class("NNorm", -1.0);
  print_class("PSub", 1.01e-307 - 1.00e-307);
  print_class("NSub", 1.00e-307 - 1.01e-307);
  
  printf("NaN : %4g 0x%02x%02x%02x%02x%02x%02x%02x%02x (%2d, %2d)\n",
	 my_nan,
	 ((unsigned char *)&my_nan)[0],
	 ((unsigned char *)&my_nan)[1],
	 ((unsigned char *)&my_nan)[2],
	 ((unsigned char *)&my_nan)[3],
	 ((unsigned char *)&my_nan)[4],
	 ((unsigned char *)&my_nan)[5],
	 ((unsigned char *)&my_nan)[6],
	 ((unsigned char *)&my_nan)[7],
	 trio_isnan(my_nan), trio_isinf(my_nan));
  printf("PInf: %4g 0x%02x%02x%02x%02x%02x%02x%02x%02x (%2d, %2d)\n",
	 my_pinf,
	 ((unsigned char *)&my_pinf)[0],
	 ((unsigned char *)&my_pinf)[1],
	 ((unsigned char *)&my_pinf)[2],
	 ((unsigned char *)&my_pinf)[3],
	 ((unsigned char *)&my_pinf)[4],
	 ((unsigned char *)&my_pinf)[5],
	 ((unsigned char *)&my_pinf)[6],
	 ((unsigned char *)&my_pinf)[7],
	 trio_isnan(my_pinf), trio_isinf(my_pinf));
  printf("NInf: %4g 0x%02x%02x%02x%02x%02x%02x%02x%02x (%2d, %2d)\n",
	 my_ninf,
	 ((unsigned char *)&my_ninf)[0],
	 ((unsigned char *)&my_ninf)[1],
	 ((unsigned char *)&my_ninf)[2],
	 ((unsigned char *)&my_ninf)[3],
	 ((unsigned char *)&my_ninf)[4],
	 ((unsigned char *)&my_ninf)[5],
	 ((unsigned char *)&my_ninf)[6],
	 ((unsigned char *)&my_ninf)[7],
	 trio_isnan(my_ninf), trio_isinf(my_ninf));
  
# if defined(TRIO_PLATFORM_UNIX)
  signal_handler = signal(SIGFPE, SIG_IGN);
# endif
  
  my_pinf = DBL_MAX + DBL_MAX;
  my_ninf = -my_pinf;
  my_nan = my_pinf / my_pinf;

# if defined(TRIO_PLATFORM_UNIX)
  signal(SIGFPE, signal_handler);
# endif
  
  printf("NaN : %4g 0x%02x%02x%02x%02x%02x%02x%02x%02x (%2d, %2d)\n",
	 my_nan,
	 ((unsigned char *)&my_nan)[0],
	 ((unsigned char *)&my_nan)[1],
	 ((unsigned char *)&my_nan)[2],
	 ((unsigned char *)&my_nan)[3],
	 ((unsigned char *)&my_nan)[4],
	 ((unsigned char *)&my_nan)[5],
	 ((unsigned char *)&my_nan)[6],
	 ((unsigned char *)&my_nan)[7],
	 trio_isnan(my_nan), trio_isinf(my_nan));
  printf("PInf: %4g 0x%02x%02x%02x%02x%02x%02x%02x%02x (%2d, %2d)\n",
	 my_pinf,
	 ((unsigned char *)&my_pinf)[0],
	 ((unsigned char *)&my_pinf)[1],
	 ((unsigned char *)&my_pinf)[2],
	 ((unsigned char *)&my_pinf)[3],
	 ((unsigned char *)&my_pinf)[4],
	 ((unsigned char *)&my_pinf)[5],
	 ((unsigned char *)&my_pinf)[6],
	 ((unsigned char *)&my_pinf)[7],
	 trio_isnan(my_pinf), trio_isinf(my_pinf));
  printf("NInf: %4g 0x%02x%02x%02x%02x%02x%02x%02x%02x (%2d, %2d)\n",
	 my_ninf,
	 ((unsigned char *)&my_ninf)[0],
	 ((unsigned char *)&my_ninf)[1],
	 ((unsigned char *)&my_ninf)[2],
	 ((unsigned char *)&my_ninf)[3],
	 ((unsigned char *)&my_ninf)[4],
	 ((unsigned char *)&my_ninf)[5],
	 ((unsigned char *)&my_ninf)[6],
	 ((unsigned char *)&my_ninf)[7],
	 trio_isnan(my_ninf), trio_isinf(my_ninf));
  
  return 0;
}
#endif
