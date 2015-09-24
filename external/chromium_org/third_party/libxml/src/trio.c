/*************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 1998 Bjorn Reese and Daniel Stenberg.
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
 *************************************************************************
 *
 * A note to trio contributors:
 *
 * Avoid heap allocation at all costs to ensure that the trio functions
 * are async-safe. The exceptions are the printf/fprintf functions, which
 * uses fputc, and the asprintf functions and the <alloc> modifier, which
 * by design are required to allocate form the heap.
 *
 ************************************************************************/


#include "triodef.h"
#include "trio.h"
#include "triop.h"
#include "trionan.h"
#if !defined(TRIO_MINIMAL)
# include "triostr.h"
#endif


#include <math.h>
#include <limits.h>
#include <float.h>

#if (defined(__STDC_ISO_10646__) || defined(MB_LEN_MAX) \
     || defined(USE_MULTIBYTE) || TRIO_WIDECHAR) \
    && !defined(_WIN32_WCE)
# define TRIO_COMPILER_SUPPORTS_MULTIBYTE
# if !defined(MB_LEN_MAX)
#  define MB_LEN_MAX 6
# endif
#endif

#if (defined(TRIO_COMPILER_MSVC) && (_MSC_VER >= 1100)) || defined(TRIO_COMPILER_BCB)
# define TRIO_COMPILER_SUPPORTS_MSVC_INT
#endif

#if defined(_WIN32_WCE)
#include <wincecompat.h>
#endif


#if !(defined(DEBUG) || defined(NDEBUG))
# define NDEBUG
#endif

#include <assert.h>
#include <ctype.h>
#if !defined(TRIO_COMPILER_SUPPORTS_C99)
# define isblank(x) (((x)==32) || ((x)==9))
#endif
#if defined(TRIO_COMPILER_ANCIENT)
# include <varargs.h>
#else
# include <stdarg.h>
#endif
#include <stddef.h>

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifndef NULL
# define NULL 0
#endif
#define NIL ((char)0)
#ifndef FALSE
# define FALSE (1 == 0)
# define TRUE (! FALSE)
#endif
#define BOOLEAN_T int

#define VALID(x) (NULL != (x))

#if TRIO_ERRORS
# define TRIO_ERROR_RETURN(x,y) (- ((x) + ((y) << 8)))
#else
# define TRIO_ERROR_RETURN(x,y) (-1)
#endif

typedef unsigned long trio_flags_t;


#if defined(TRIO_PLATFORM_UNIX)
# include <unistd.h>
# include <signal.h>
# include <locale.h>
# define USE_LOCALE
#endif 
#if defined(TRIO_PLATFORM_VMS)
# include <unistd.h>
#endif
#if defined(TRIO_PLATFORM_WIN32)
# if defined(_WIN32_WCE)
#  include <wincecompat.h>
# else
#  include <io.h>
#  define read _read
#  define write _write
# endif
#endif 

#if TRIO_WIDECHAR
# if defined(TRIO_COMPILER_SUPPORTS_ISO94)
#  include <wchar.h>
#  include <wctype.h>
typedef wchar_t trio_wchar_t;
typedef wint_t trio_wint_t;
# else
typedef char trio_wchar_t;
typedef int trio_wint_t;
#  define WCONST(x) L ## x
#  define WEOF EOF
#  define iswalnum(x) isalnum(x)
#  define iswalpha(x) isalpha(x)
#  define iswblank(x) isblank(x)
#  define iswcntrl(x) iscntrl(x)
#  define iswdigit(x) isdigit(x)
#  define iswgraph(x) isgraph(x)
#  define iswlower(x) islower(x)
#  define iswprint(x) isprint(x)
#  define iswpunct(x) ispunct(x)
#  define iswspace(x) isspace(x)
#  define iswupper(x) isupper(x)
#  define iswxdigit(x) isxdigit(x)
# endif
#endif



#ifndef __cplusplus
# if !defined(USE_LONGLONG)
#  if defined(TRIO_COMPILER_GCC) && !defined(__STRICT_ANSI__)
#   define USE_LONGLONG
#  elif defined(TRIO_COMPILER_SUNPRO)
#   define USE_LONGLONG
#  elif defined(_LONG_LONG) || defined(_LONGLONG)
#   define USE_LONGLONG
#  endif
# endif
#endif

#if defined(USE_LONGLONG)
typedef signed long long int trio_longlong_t;
typedef unsigned long long int trio_ulonglong_t;
#elif defined(TRIO_COMPILER_SUPPORTS_MSVC_INT)
typedef signed __int64 trio_longlong_t;
typedef unsigned __int64 trio_ulonglong_t;
#else
typedef TRIO_SIGNED long int trio_longlong_t;
typedef unsigned long int trio_ulonglong_t;
#endif

#if defined(TRIO_COMPILER_SUPPORTS_C99)
# include <stdint.h>
typedef intmax_t trio_intmax_t;
typedef uintmax_t trio_uintmax_t;
typedef int8_t trio_int8_t;
typedef int16_t trio_int16_t;
typedef int32_t trio_int32_t;
typedef int64_t trio_int64_t;
#elif defined(TRIO_COMPILER_SUPPORTS_UNIX98)
# include <inttypes.h>
typedef intmax_t trio_intmax_t;
typedef uintmax_t trio_uintmax_t;
typedef int8_t trio_int8_t;
typedef int16_t trio_int16_t;
typedef int32_t trio_int32_t;
typedef int64_t trio_int64_t;
#elif defined(TRIO_COMPILER_SUPPORTS_MSVC_INT)
typedef trio_longlong_t trio_intmax_t;
typedef trio_ulonglong_t trio_uintmax_t;
typedef __int8 trio_int8_t;
typedef __int16 trio_int16_t;
typedef __int32 trio_int32_t;
typedef __int64 trio_int64_t;
#else
typedef trio_longlong_t trio_intmax_t;
typedef trio_ulonglong_t trio_uintmax_t;
# if defined(TRIO_INT8_T)
typedef TRIO_INT8_T trio_int8_t;
# else
typedef TRIO_SIGNED char trio_int8_t;
# endif
# if defined(TRIO_INT16_T)
typedef TRIO_INT16_T trio_int16_t;
# else
typedef TRIO_SIGNED short trio_int16_t;
# endif
# if defined(TRIO_INT32_T)
typedef TRIO_INT32_T trio_int32_t;
# else
typedef TRIO_SIGNED int trio_int32_t;
# endif
# if defined(TRIO_INT64_T)
typedef TRIO_INT64_T trio_int64_t;
# else
typedef trio_longlong_t trio_int64_t;
# endif
#endif

#if (!(defined(TRIO_COMPILER_SUPPORTS_C99) \
 || defined(TRIO_COMPILER_SUPPORTS_UNIX01))) \
 && !defined(_WIN32_WCE)
# define floorl(x) floor((double)(x))
# define fmodl(x,y) fmod((double)(x),(double)(y))
# define powl(x,y) pow((double)(x),(double)(y))
#endif

#define TRIO_FABS(x) (((x) < 0.0) ? -(x) : (x))


#ifndef DECIMAL_DIG
# define DECIMAL_DIG DBL_DIG
#endif

#ifdef LDBL_DIG
# define MAX_MANTISSA_DIGITS LDBL_DIG
# define MAX_EXPONENT_DIGITS 4
# define MAX_DOUBLE_DIGITS LDBL_MAX_10_EXP
#else
# define MAX_MANTISSA_DIGITS DECIMAL_DIG
# define MAX_EXPONENT_DIGITS 3
# define MAX_DOUBLE_DIGITS DBL_MAX_10_EXP
#endif

#if defined(TRIO_COMPILER_ANCIENT) || !defined(LDBL_DIG)
# undef LDBL_DIG
# undef LDBL_MANT_DIG
# undef LDBL_EPSILON
# define LDBL_DIG DBL_DIG
# define LDBL_MANT_DIG DBL_MANT_DIG
# define LDBL_EPSILON DBL_EPSILON
#endif

#define MAX_CHARS_IN(x) (sizeof(x) * CHAR_BIT)
#define POINTER_WIDTH ((sizeof("0x") - 1) + sizeof(trio_pointer_t) * CHAR_BIT / 4)

#define INFINITE_LOWER "inf"
#define INFINITE_UPPER "INF"
#define LONG_INFINITE_LOWER "infinite"
#define LONG_INFINITE_UPPER "INFINITE"
#define NAN_LOWER "nan"
#define NAN_UPPER "NAN"

enum {
  TYPE_PRINT = 1,
  TYPE_SCAN  = 2,

  
  FLAGS_NEW                 = 0,
  FLAGS_STICKY              = 1,
  FLAGS_SPACE               = 2 * FLAGS_STICKY,
  FLAGS_SHOWSIGN            = 2 * FLAGS_SPACE,
  FLAGS_LEFTADJUST          = 2 * FLAGS_SHOWSIGN,
  FLAGS_ALTERNATIVE         = 2 * FLAGS_LEFTADJUST,
  FLAGS_SHORT               = 2 * FLAGS_ALTERNATIVE,
  FLAGS_SHORTSHORT          = 2 * FLAGS_SHORT,
  FLAGS_LONG                = 2 * FLAGS_SHORTSHORT,
  FLAGS_QUAD                = 2 * FLAGS_LONG,
  FLAGS_LONGDOUBLE          = 2 * FLAGS_QUAD,
  FLAGS_SIZE_T              = 2 * FLAGS_LONGDOUBLE,
  FLAGS_PTRDIFF_T           = 2 * FLAGS_SIZE_T,
  FLAGS_INTMAX_T            = 2 * FLAGS_PTRDIFF_T,
  FLAGS_NILPADDING          = 2 * FLAGS_INTMAX_T,
  FLAGS_UNSIGNED            = 2 * FLAGS_NILPADDING,
  FLAGS_UPPER               = 2 * FLAGS_UNSIGNED,
  FLAGS_WIDTH               = 2 * FLAGS_UPPER,
  FLAGS_WIDTH_PARAMETER     = 2 * FLAGS_WIDTH,
  FLAGS_PRECISION           = 2 * FLAGS_WIDTH_PARAMETER,
  FLAGS_PRECISION_PARAMETER = 2 * FLAGS_PRECISION,
  FLAGS_BASE                = 2 * FLAGS_PRECISION_PARAMETER,
  FLAGS_BASE_PARAMETER      = 2 * FLAGS_BASE,
  FLAGS_FLOAT_E             = 2 * FLAGS_BASE_PARAMETER,
  FLAGS_FLOAT_G             = 2 * FLAGS_FLOAT_E,
  FLAGS_QUOTE               = 2 * FLAGS_FLOAT_G,
  FLAGS_WIDECHAR            = 2 * FLAGS_QUOTE,
  FLAGS_ALLOC               = 2 * FLAGS_WIDECHAR,
  FLAGS_IGNORE              = 2 * FLAGS_ALLOC,
  FLAGS_IGNORE_PARAMETER    = 2 * FLAGS_IGNORE,
  FLAGS_VARSIZE_PARAMETER   = 2 * FLAGS_IGNORE_PARAMETER,
  FLAGS_FIXED_SIZE          = 2 * FLAGS_VARSIZE_PARAMETER,
  FLAGS_LAST                = FLAGS_FIXED_SIZE,
  
  FLAGS_EXCLUDE             = FLAGS_SHORT,
  FLAGS_USER_DEFINED        = FLAGS_IGNORE,
  FLAGS_ROUNDING            = FLAGS_INTMAX_T,
  
  FLAGS_ALL_VARSIZES        = FLAGS_LONG | FLAGS_QUAD | FLAGS_INTMAX_T | FLAGS_PTRDIFF_T | FLAGS_SIZE_T,
  FLAGS_ALL_SIZES           = FLAGS_ALL_VARSIZES | FLAGS_SHORTSHORT | FLAGS_SHORT,

  NO_POSITION  = -1,
  NO_WIDTH     =  0,
  NO_PRECISION = -1,
  NO_SIZE      = -1,

  
  NO_BASE      = -1,
  MIN_BASE     =  2,
  MAX_BASE     = 36,
  BASE_BINARY  =  2,
  BASE_OCTAL   =  8,
  BASE_DECIMAL = 10,
  BASE_HEX     = 16,

  
  MAX_PARAMETERS = 64,
  
  MAX_CHARACTER_CLASS = UCHAR_MAX + 1,

  
  MAX_USER_NAME = 64,
  MAX_USER_DATA = 256,
  
  
  MAX_LOCALE_SEPARATOR_LENGTH = MB_LEN_MAX,
  
  MAX_LOCALE_GROUPS = 64,

  
  DYNAMIC_START_SIZE = 32
};

#define NO_GROUPING ((int)CHAR_MAX)

#define FORMAT_UNKNOWN   0
#define FORMAT_INT       1
#define FORMAT_DOUBLE    2
#define FORMAT_CHAR      3
#define FORMAT_STRING    4
#define FORMAT_POINTER   5
#define FORMAT_COUNT     6
#define FORMAT_PARAMETER 7
#define FORMAT_GROUP     8
#if TRIO_GNU
# define FORMAT_ERRNO    9
#endif
#if TRIO_EXTENSION
# define FORMAT_USER_DEFINED 10
#endif

#define CHAR_IDENTIFIER '%'
#define CHAR_BACKSLASH '\\'
#define CHAR_QUOTE '\"'
#define CHAR_ADJUST ' '

#define CLASS_ALNUM "[:alnum:]"
#define CLASS_ALPHA "[:alpha:]"
#define CLASS_BLANK "[:blank:]"
#define CLASS_CNTRL "[:cntrl:]"
#define CLASS_DIGIT "[:digit:]"
#define CLASS_GRAPH "[:graph:]"
#define CLASS_LOWER "[:lower:]"
#define CLASS_PRINT "[:print:]"
#define CLASS_PUNCT "[:punct:]"
#define CLASS_SPACE "[:space:]"
#define CLASS_UPPER "[:upper:]"
#define CLASS_XDIGIT "[:xdigit:]"

#define SPECIFIER_CHAR 'c'
#define SPECIFIER_STRING 's'
#define SPECIFIER_DECIMAL 'd'
#define SPECIFIER_INTEGER 'i'
#define SPECIFIER_UNSIGNED 'u'
#define SPECIFIER_OCTAL 'o'
#define SPECIFIER_HEX 'x'
#define SPECIFIER_HEX_UPPER 'X'
#define SPECIFIER_FLOAT_E 'e'
#define SPECIFIER_FLOAT_E_UPPER 'E'
#define SPECIFIER_FLOAT_F 'f'
#define SPECIFIER_FLOAT_F_UPPER 'F'
#define SPECIFIER_FLOAT_G 'g'
#define SPECIFIER_FLOAT_G_UPPER 'G'
#define SPECIFIER_POINTER 'p'
#define SPECIFIER_GROUP '['
#define SPECIFIER_UNGROUP ']'
#define SPECIFIER_COUNT 'n'
#if TRIO_UNIX98
# define SPECIFIER_CHAR_UPPER 'C'
# define SPECIFIER_STRING_UPPER 'S'
#endif
#if TRIO_C99
# define SPECIFIER_HEXFLOAT 'a'
# define SPECIFIER_HEXFLOAT_UPPER 'A'
#endif
#if TRIO_GNU
# define SPECIFIER_ERRNO 'm'
#endif
#if TRIO_EXTENSION
# define SPECIFIER_BINARY 'b'
# define SPECIFIER_BINARY_UPPER 'B'
# define SPECIFIER_USER_DEFINED_BEGIN '<'
# define SPECIFIER_USER_DEFINED_END '>'
# define SPECIFIER_USER_DEFINED_SEPARATOR ':'
#endif

#define QUALIFIER_POSITION '$'
#define QUALIFIER_SHORT 'h'
#define QUALIFIER_LONG 'l'
#define QUALIFIER_LONG_UPPER 'L'
#define QUALIFIER_ALTERNATIVE '#'
#define QUALIFIER_SPACE ' '
#define QUALIFIER_PLUS '+'
#define QUALIFIER_MINUS '-'
#define QUALIFIER_DOT '.'
#define QUALIFIER_STAR '*'
#define QUALIFIER_CIRCUMFLEX '^' 
#if TRIO_C99
# define QUALIFIER_SIZE_T 'z'
# define QUALIFIER_PTRDIFF_T 't'
# define QUALIFIER_INTMAX_T 'j'
#endif
#if TRIO_BSD || TRIO_GNU
# define QUALIFIER_QUAD 'q'
#endif
#if TRIO_GNU
# define QUALIFIER_SIZE_T_UPPER 'Z'
#endif
#if TRIO_MISC
# define QUALIFIER_WIDECHAR 'w'
#endif
#if TRIO_MICROSOFT
# define QUALIFIER_FIXED_SIZE 'I'
#endif
#if TRIO_EXTENSION
# define QUALIFIER_QUOTE '\''
# define QUALIFIER_STICKY '!'
# define QUALIFIER_VARSIZE '&' 
# define QUALIFIER_PARAM '@' 
# define QUALIFIER_COLON ':' 
# define QUALIFIER_EQUAL '=' 
# define QUALIFIER_ROUNDING_UPPER 'R'
#endif



typedef struct {
  
  int type;
  
  trio_flags_t flags;
  
  int width;
  
  int precision;
  
  int base;
  
  int varsize;
  
  int indexAfterSpecifier;
  
  union {
    char *string;
#if TRIO_WIDECHAR
    trio_wchar_t *wstring;
#endif
    trio_pointer_t pointer;
    union {
      trio_intmax_t as_signed;
      trio_uintmax_t as_unsigned;
    } number;
    double doubleNumber;
    double *doublePointer;
    trio_long_double_t longdoubleNumber;
    trio_long_double_t *longdoublePointer;
    int errorNumber;
  } data;
  
  char user_name[MAX_USER_NAME];
  char user_data[MAX_USER_DATA];
} trio_parameter_t;

typedef struct {
  union {
    trio_outstream_t out;
    trio_instream_t in;
  } stream;
  trio_pointer_t closure;
} trio_custom_t;

typedef struct _trio_class_t {
  void (*OutStream) TRIO_PROTO((struct _trio_class_t *, int));
  void (*InStream) TRIO_PROTO((struct _trio_class_t *, int *));
  trio_pointer_t location;
  int current;
  /*
   * The number of characters that would have been written/read
   * if there had been sufficient space.
   */
  int processed;
  /*
   * The number of characters that are actually written/read.
   * Processed and committed will only differ for the *nprintf
   * and *nscanf functions.
   */
  int committed;
  /*
   * The upper limit of characters that may be written/read.
   */
  int max;
  int error;
} trio_class_t;

typedef struct _trio_reference_t {
  trio_class_t *data;
  trio_parameter_t *parameter;
} trio_reference_t;

typedef struct _trio_userdef_t {
  struct _trio_userdef_t *next;
  trio_callback_t callback;
  char *name;
} trio_userdef_t;


static TRIO_CONST char rcsid[] = "@(#)$Id$";

#if defined(TRIO_PLATFORM_MPEIX)
static TRIO_CONST trio_long_double_t ___dummy_long_double = 0;
#endif

static TRIO_CONST char internalNullString[] = "(nil)";

#if defined(USE_LOCALE)
static struct lconv *internalLocaleValues = NULL;
#endif

static int internalDecimalPointLength = 1;
static int internalThousandSeparatorLength = 1;
static char internalDecimalPoint = '.';
static char internalDecimalPointString[MAX_LOCALE_SEPARATOR_LENGTH + 1] = ".";
static char internalThousandSeparator[MAX_LOCALE_SEPARATOR_LENGTH + 1] = ",";
static char internalGrouping[MAX_LOCALE_GROUPS] = { (char)NO_GROUPING };

static TRIO_CONST char internalDigitsLower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static TRIO_CONST char internalDigitsUpper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static BOOLEAN_T internalDigitsUnconverted = TRUE;
static int internalDigitArray[128];
#if TRIO_EXTENSION
static BOOLEAN_T internalCollationUnconverted = TRUE;
static char internalCollationArray[MAX_CHARACTER_CLASS][MAX_CHARACTER_CLASS];
#endif

#if TRIO_EXTENSION
static TRIO_VOLATILE trio_callback_t internalEnterCriticalRegion = NULL;
static TRIO_VOLATILE trio_callback_t internalLeaveCriticalRegion = NULL;
static trio_userdef_t *internalUserDef = NULL;
#endif



#if defined(TRIO_MINIMAL)
# define TRIO_STRING_PUBLIC static
# include "triostr.c"
#endif 

TRIO_PRIVATE BOOLEAN_T
TrioIsQualifier
TRIO_ARGS1((character),
	   TRIO_CONST char character)
{
  
  switch (character)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case QUALIFIER_PLUS:
    case QUALIFIER_MINUS:
    case QUALIFIER_SPACE:
    case QUALIFIER_DOT:
    case QUALIFIER_STAR:
    case QUALIFIER_ALTERNATIVE:
    case QUALIFIER_SHORT:
    case QUALIFIER_LONG:
    case QUALIFIER_LONG_UPPER:
    case QUALIFIER_CIRCUMFLEX:
#if defined(QUALIFIER_SIZE_T)
    case QUALIFIER_SIZE_T:
#endif
#if defined(QUALIFIER_PTRDIFF_T)
    case QUALIFIER_PTRDIFF_T:
#endif
#if defined(QUALIFIER_INTMAX_T)
    case QUALIFIER_INTMAX_T:
#endif
#if defined(QUALIFIER_QUAD)
    case QUALIFIER_QUAD:
#endif
#if defined(QUALIFIER_SIZE_T_UPPER)
    case QUALIFIER_SIZE_T_UPPER:
#endif
#if defined(QUALIFIER_WIDECHAR)
    case QUALIFIER_WIDECHAR:
#endif
#if defined(QUALIFIER_QUOTE)
    case QUALIFIER_QUOTE:
#endif
#if defined(QUALIFIER_STICKY)
    case QUALIFIER_STICKY:
#endif
#if defined(QUALIFIER_VARSIZE)
    case QUALIFIER_VARSIZE:
#endif
#if defined(QUALIFIER_PARAM)
    case QUALIFIER_PARAM:
#endif
#if defined(QUALIFIER_FIXED_SIZE)
    case QUALIFIER_FIXED_SIZE:
#endif
#if defined(QUALIFIER_ROUNDING_UPPER)
    case QUALIFIER_ROUNDING_UPPER:
#endif
      return TRUE;
    default:
      return FALSE;
    }
}

#if defined(USE_LOCALE)
TRIO_PRIVATE void
TrioSetLocale(TRIO_NOARGS)
{
  internalLocaleValues = (struct lconv *)localeconv();
  if (internalLocaleValues)
    {
      if ((internalLocaleValues->decimal_point) &&
	  (internalLocaleValues->decimal_point[0] != NIL))
	{
	  internalDecimalPointLength = trio_length(internalLocaleValues->decimal_point);
	  if (internalDecimalPointLength == 1)
	    {
	      internalDecimalPoint = internalLocaleValues->decimal_point[0];
	    }
	  else
	    {
	      internalDecimalPoint = NIL;
	      trio_copy_max(internalDecimalPointString,
			    sizeof(internalDecimalPointString),
			    internalLocaleValues->decimal_point);
	    }
	}
      if ((internalLocaleValues->thousands_sep) &&
	  (internalLocaleValues->thousands_sep[0] != NIL))
	{
	  trio_copy_max(internalThousandSeparator,
			sizeof(internalThousandSeparator),
			internalLocaleValues->thousands_sep);
	  internalThousandSeparatorLength = trio_length(internalThousandSeparator);
	}
      if ((internalLocaleValues->grouping) &&
	  (internalLocaleValues->grouping[0] != NIL))
	{
	  trio_copy_max(internalGrouping,
			sizeof(internalGrouping),
			internalLocaleValues->grouping);
	}
    }
}
#endif 

TRIO_PRIVATE int
TrioCalcThousandSeparatorLength
TRIO_ARGS1((digits),
	   int digits)
{
#if TRIO_EXTENSION
  int count = 0;
  int step = NO_GROUPING;
  char *groupingPointer = internalGrouping;

  while (digits > 0)
    {
      if (*groupingPointer == CHAR_MAX)
	{
	  
	  break; 
	}
      else if (*groupingPointer == 0)
	{
	  
	  if (step == NO_GROUPING)
	    {
	      
	      break; 
	    }
	}
      else
	{
	  step = *groupingPointer++;
	}
      if (digits > step)
	count += internalThousandSeparatorLength;
      digits -= step;
    }
  return count;
#else
  return 0;
#endif
}

TRIO_PRIVATE BOOLEAN_T
TrioFollowedBySeparator
TRIO_ARGS1((position),
	   int position)
{
#if TRIO_EXTENSION
  int step = 0;
  char *groupingPointer = internalGrouping;

  position--;
  if (position == 0)
    return FALSE;
  while (position > 0)
    {
      if (*groupingPointer == CHAR_MAX)
	{
	  
	  break; 
	}
      else if (*groupingPointer != 0)
	{
	  step = *groupingPointer++;
	}
      if (step == 0)
	break;
      position -= step;
    }
  return (position == 0);
#else
  return FALSE;
#endif
}

TRIO_PRIVATE int
TrioGetPosition
TRIO_ARGS2((format, indexPointer),
	   TRIO_CONST char *format,
	   int *indexPointer)
{
#if TRIO_UNIX98
  char *tmpformat;
  int number = 0;
  int index = *indexPointer;

  number = (int)trio_to_long(&format[index], &tmpformat, BASE_DECIMAL);
  index = (int)(tmpformat - format);
  if ((number != 0) && (QUALIFIER_POSITION == format[index++]))
    {
      *indexPointer = index;
      return number - 1;
    }
#endif
  return NO_POSITION;
}

#if TRIO_EXTENSION
TRIO_PRIVATE trio_userdef_t *
TrioFindNamespace
TRIO_ARGS2((name, prev),
	   TRIO_CONST char *name,
	   trio_userdef_t **prev)
{
  trio_userdef_t *def;
  
  if (internalEnterCriticalRegion)
    (void)internalEnterCriticalRegion(NULL);
  
  for (def = internalUserDef; def; def = def->next)
    {
      
      if (trio_equal_case(def->name, name))
	break;
      
      if (prev)
	*prev = def;
    }
  
  if (internalLeaveCriticalRegion)
    (void)internalLeaveCriticalRegion(NULL);
  
  return def;
}
#endif

TRIO_PRIVATE trio_long_double_t
TrioPower
TRIO_ARGS2((number, exponent),
	   int number,
	   int exponent)
{
  trio_long_double_t result;

  if (number == 10)
    {
      switch (exponent)
	{
	  
	case 0:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E-1);
	  break;
	case 1:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+0);
	  break;
	case 2:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+1);
	  break;
	case 3:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+2);
	  break;
	case 4:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+3);
	  break;
	case 5:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+4);
	  break;
	case 6:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+5);
	  break;
	case 7:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+6);
	  break;
	case 8:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+7);
	  break;
	case 9:
	  result = (trio_long_double_t)number * TRIO_SUFFIX_LONG(1E+8);
	  break;
	default:
	  result = powl((trio_long_double_t)number,
			(trio_long_double_t)exponent);
	  break;
	}
    }
  else
    {
      return powl((trio_long_double_t)number, (trio_long_double_t)exponent);
    }
  return result;
}

TRIO_PRIVATE double
TrioLogarithm
TRIO_ARGS2((number, base),
	   double number,
	   int base)
{
  double result;

  if (number <= 0.0)
    {
      
      result = (number == 0.0) ? trio_ninf() : trio_nan();
    }
  else
    {
      if (base == 10)
	{
	  result = log10(number);
	}
      else
	{
	  result = log10(number) / log10((double)base);
	}
    }
  return result;
}

TRIO_PRIVATE double
TrioLogarithmBase
TRIO_ARGS1((base),
	   int base)
{
  switch (base)
    {
    case BASE_BINARY : return 1.0;
    case BASE_OCTAL  : return 3.0;
    case BASE_DECIMAL: return 3.321928094887362345;
    case BASE_HEX    : return 4.0;
    default          : return TrioLogarithm((double)base, 2);
    }
}

TRIO_PRIVATE int
TrioParse
TRIO_ARGS5((type, format, parameters, arglist, argarray),
	   int type,
	   TRIO_CONST char *format,
	   trio_parameter_t *parameters,
	   va_list *arglist,
	   trio_pointer_t *argarray)
{
  
  unsigned short usedEntries[MAX_PARAMETERS];
  
  int parameterPosition;
  int currentParam;
  int maxParam = -1;
  
  trio_flags_t flags;
  int width;
  int precision;
  int varsize;
  int base;
  int index;  
  int dots;  
  BOOLEAN_T positional;  
  BOOLEAN_T gotSticky = FALSE;  
  int indices[MAX_PARAMETERS];
  int pos = 0;
  
  char ch;
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  int charlen;
#endif
  int save_errno;
  int i = -1;
  int num;
  char *tmpformat;

  
  assert((arglist != NULL) ^ (argarray != NULL));
  
  memset(usedEntries, 0, sizeof(usedEntries));

  save_errno = errno;
  index = 0;
  parameterPosition = 0;
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  (void)mblen(NULL, 0);
#endif
  
  while (format[index])
    {
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
      if (! isascii(format[index]))
	{
	  charlen = mblen(&format[index], MB_LEN_MAX);
	  index += (charlen > 0) ? charlen : 1;
	  continue; 
	}
#endif 
      if (CHAR_IDENTIFIER == format[index++])
	{
	  if (CHAR_IDENTIFIER == format[index])
	    {
	      index++;
	      continue; 
	    }

	  flags = FLAGS_NEW;
	  dots = 0;
	  currentParam = TrioGetPosition(format, &index);
	  positional = (NO_POSITION != currentParam);
	  if (!positional)
	    {
	      
	      currentParam = parameterPosition;
	    }
          if(currentParam >= MAX_PARAMETERS)
	    {
	      
	      return TRIO_ERROR_RETURN(TRIO_ETOOMANY, index);
	    }

	  if (currentParam > maxParam)
	    maxParam = currentParam;

	  
	  width = NO_WIDTH;
	  precision = NO_PRECISION;
	  base = NO_BASE;
	  varsize = NO_SIZE;

	  while (TrioIsQualifier(format[index]))
	    {
	      ch = format[index++];

	      switch (ch)
		{
		case QUALIFIER_SPACE:
		  flags |= FLAGS_SPACE;
		  break;

		case QUALIFIER_PLUS:
		  flags |= FLAGS_SHOWSIGN;
		  break;

		case QUALIFIER_MINUS:
		  flags |= FLAGS_LEFTADJUST;
		  flags &= ~FLAGS_NILPADDING;
		  break;

		case QUALIFIER_ALTERNATIVE:
		  flags |= FLAGS_ALTERNATIVE;
		  break;

		case QUALIFIER_DOT:
		  if (dots == 0) 
		    {
		      dots++;

		      
		      if (QUALIFIER_DOT == format[index])
			break;
		      
		      
		      flags |= FLAGS_PRECISION;
		      if ((QUALIFIER_STAR == format[index])
#if defined(QUALIFIER_PARAM)
			  || (QUALIFIER_PARAM == format[index])
#endif
			  )
			{
			  index++;
			  flags |= FLAGS_PRECISION_PARAMETER;

			  precision = TrioGetPosition(format, &index);
			  if (precision == NO_POSITION)
			    {
			      parameterPosition++;
			      if (positional)
				precision = parameterPosition;
			      else
				{
				  precision = currentParam;
				  currentParam = precision + 1;
				}
			    }
			  else
			    {
			      if (! positional)
				currentParam = precision + 1;
			      if (width > maxParam)
				maxParam = precision;
			    }
			  if (currentParam > maxParam)
			    maxParam = currentParam;
			}
		      else
			{
			  precision = trio_to_long(&format[index],
						   &tmpformat,
						   BASE_DECIMAL);
			  index = (int)(tmpformat - format);
			}
		    }
		  else if (dots == 1) 
		    {
		      dots++;
		      
		      
		      flags |= FLAGS_BASE;
		      if ((QUALIFIER_STAR == format[index])
#if defined(QUALIFIER_PARAM)
			  || (QUALIFIER_PARAM == format[index])
#endif
			  )
			{
			  index++;
			  flags |= FLAGS_BASE_PARAMETER;
			  base = TrioGetPosition(format, &index);
			  if (base == NO_POSITION)
			    {
			      parameterPosition++;
			      if (positional)
				base = parameterPosition;
			      else
				{
				  base = currentParam;
				  currentParam = base + 1;
				}
			    }
			  else
			    {
			      if (! positional)
				currentParam = base + 1;
			      if (base > maxParam)
				maxParam = base;
			    }
			  if (currentParam > maxParam)
			    maxParam = currentParam;
			}
		      else
			{
			  base = trio_to_long(&format[index],
					      &tmpformat,
					      BASE_DECIMAL);
			  if (base > MAX_BASE)
			    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
			  index = (int)(tmpformat - format);
			}
		    }
		  else
		    {
		      return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
		    }
		  break; 

#if defined(QUALIFIER_PARAM)
		case QUALIFIER_PARAM:
		  type = TYPE_PRINT;
		  
#endif
		case QUALIFIER_STAR:
		  
		  if (TYPE_PRINT == type)
		    {
		      
		      flags |= (FLAGS_WIDTH | FLAGS_WIDTH_PARAMETER);
		      width = TrioGetPosition(format, &index);
		      if (width == NO_POSITION)
			{
			  parameterPosition++;
			  if (positional)
			    width = parameterPosition;
			  else
			    {
			      width = currentParam;
			      currentParam = width + 1;
			    }
			}
		      else
			{
			  if (! positional)
			    currentParam = width + 1;
			  if (width > maxParam)
			    maxParam = width;
			}
		      if (currentParam > maxParam)
			maxParam = currentParam;
		    }
		  else
		    {
		      
		      flags |= FLAGS_IGNORE;
		    }

		  break; 

		case '0':
		  if (! (flags & FLAGS_LEFTADJUST))
		    flags |= FLAGS_NILPADDING;
		  
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		  flags |= FLAGS_WIDTH;
		  width = trio_to_long(&format[index - 1],
				       &tmpformat,
				       BASE_DECIMAL);
		  index = (int)(tmpformat - format);
		  break;

		case QUALIFIER_SHORT:
		  if (flags & FLAGS_SHORTSHORT)
		    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
		  else if (flags & FLAGS_SHORT)
		    flags |= FLAGS_SHORTSHORT;
		  else
		    flags |= FLAGS_SHORT;
		  break;

		case QUALIFIER_LONG:
		  if (flags & FLAGS_QUAD)
		    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
		  else if (flags & FLAGS_LONG)
		    flags |= FLAGS_QUAD;
		  else
		    flags |= FLAGS_LONG;
		  break;

		case QUALIFIER_LONG_UPPER:
		  flags |= FLAGS_LONGDOUBLE;
		  break;

#if defined(QUALIFIER_SIZE_T)
		case QUALIFIER_SIZE_T:
		  flags |= FLAGS_SIZE_T;
		  
		  if (sizeof(size_t) == sizeof(trio_ulonglong_t))
		    flags |= FLAGS_QUAD;
		  else if (sizeof(size_t) == sizeof(long))
		    flags |= FLAGS_LONG;
		  break;
#endif

#if defined(QUALIFIER_PTRDIFF_T)
		case QUALIFIER_PTRDIFF_T:
		  flags |= FLAGS_PTRDIFF_T;
		  if (sizeof(ptrdiff_t) == sizeof(trio_ulonglong_t))
		    flags |= FLAGS_QUAD;
		  else if (sizeof(ptrdiff_t) == sizeof(long))
		    flags |= FLAGS_LONG;
		  break;
#endif

#if defined(QUALIFIER_INTMAX_T)
		case QUALIFIER_INTMAX_T:
		  flags |= FLAGS_INTMAX_T;
		  if (sizeof(trio_intmax_t) == sizeof(trio_ulonglong_t))
		    flags |= FLAGS_QUAD;
		  else if (sizeof(trio_intmax_t) == sizeof(long))
		    flags |= FLAGS_LONG;
		  break;
#endif

#if defined(QUALIFIER_QUAD)
		case QUALIFIER_QUAD:
		  flags |= FLAGS_QUAD;
		  break;
#endif

#if defined(QUALIFIER_FIXED_SIZE)
		case QUALIFIER_FIXED_SIZE:
		  if (flags & FLAGS_FIXED_SIZE)
		    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);

		  if (flags & (FLAGS_ALL_SIZES | FLAGS_LONGDOUBLE |
			       FLAGS_WIDECHAR | FLAGS_VARSIZE_PARAMETER))
		    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);

		  if ((format[index] == '6') &&
		      (format[index + 1] == '4'))
		    {
		      varsize = sizeof(trio_int64_t);
		      index += 2;
		    }
		  else if ((format[index] == '3') &&
			   (format[index + 1] == '2'))
		    {
		      varsize = sizeof(trio_int32_t);
		      index += 2;
		    }
		  else if ((format[index] == '1') &&
			   (format[index + 1] == '6'))
		    {
		      varsize = sizeof(trio_int16_t);
		      index += 2;
		    }
		  else if (format[index] == '8')
		    {
		      varsize = sizeof(trio_int8_t);
		      index++;
		    }
		  else
		    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
		  
		  flags |= FLAGS_FIXED_SIZE;
		  break;
#endif

#if defined(QUALIFIER_WIDECHAR)
		case QUALIFIER_WIDECHAR:
		  flags |= FLAGS_WIDECHAR;
		  break;
#endif

#if defined(QUALIFIER_SIZE_T_UPPER)
		case QUALIFIER_SIZE_T_UPPER:
		  break;
#endif

#if defined(QUALIFIER_QUOTE)
		case QUALIFIER_QUOTE:
		  flags |= FLAGS_QUOTE;
		  break;
#endif

#if defined(QUALIFIER_STICKY)
		case QUALIFIER_STICKY:
		  flags |= FLAGS_STICKY;
		  gotSticky = TRUE;
		  break;
#endif
		  
#if defined(QUALIFIER_VARSIZE)
		case QUALIFIER_VARSIZE:
		  flags |= FLAGS_VARSIZE_PARAMETER;
		  parameterPosition++;
		  if (positional)
		    varsize = parameterPosition;
		  else
		    {
		      varsize = currentParam;
		      currentParam = varsize + 1;
		    }
		  if (currentParam > maxParam)
		    maxParam = currentParam;
		  break;
#endif

#if defined(QUALIFIER_ROUNDING_UPPER)
		case QUALIFIER_ROUNDING_UPPER:
		  flags |= FLAGS_ROUNDING;
		  break;
#endif

		default:
		  
                  return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
		}
	    } 

	  if (flags & FLAGS_WIDTH_PARAMETER)
	    {
	      usedEntries[width] += 1;
	      parameters[pos].type = FORMAT_PARAMETER;
	      parameters[pos].flags = 0;
	      indices[width] = pos;
	      width = pos++;
	    }
	  if (flags & FLAGS_PRECISION_PARAMETER)
	    {
	      usedEntries[precision] += 1;
	      parameters[pos].type = FORMAT_PARAMETER;
	      parameters[pos].flags = 0;
	      indices[precision] = pos;
	      precision = pos++;
	    }
	  if (flags & FLAGS_BASE_PARAMETER)
	    {
	      usedEntries[base] += 1;
	      parameters[pos].type = FORMAT_PARAMETER;
	      parameters[pos].flags = 0;
	      indices[base] = pos;
	      base = pos++;
	    }
	  if (flags & FLAGS_VARSIZE_PARAMETER)
	    {
	      usedEntries[varsize] += 1;
	      parameters[pos].type = FORMAT_PARAMETER;
	      parameters[pos].flags = 0;
	      indices[varsize] = pos;
	      varsize = pos++;
	    }
	  
	  indices[currentParam] = pos;
	  
	  switch (format[index++])
	    {
#if defined(SPECIFIER_CHAR_UPPER)
	    case SPECIFIER_CHAR_UPPER:
	      flags |= FLAGS_WIDECHAR;
	      
#endif
	    case SPECIFIER_CHAR:
	      if (flags & FLAGS_LONG)
		flags |= FLAGS_WIDECHAR;
	      else if (flags & FLAGS_SHORT)
		flags &= ~FLAGS_WIDECHAR;
	      parameters[pos].type = FORMAT_CHAR;
	      break;

#if defined(SPECIFIER_STRING_UPPER)
	    case SPECIFIER_STRING_UPPER:
	      flags |= FLAGS_WIDECHAR;
	      
#endif
	    case SPECIFIER_STRING:
	      if (flags & FLAGS_LONG)
		flags |= FLAGS_WIDECHAR;
	      else if (flags & FLAGS_SHORT)
		flags &= ~FLAGS_WIDECHAR;
	      parameters[pos].type = FORMAT_STRING;
	      break;

	    case SPECIFIER_GROUP:
	      if (TYPE_SCAN == type)
		{
		  int depth = 1;
		  parameters[pos].type = FORMAT_GROUP;
		  if (format[index] == QUALIFIER_CIRCUMFLEX)
		    index++;
		  if (format[index] == SPECIFIER_UNGROUP)
		    index++;
		  if (format[index] == QUALIFIER_MINUS)
		    index++;
		  
		  while (format[index] != NIL)
		    {
		      if (format[index] == SPECIFIER_GROUP)
			{
			  depth++;
			}
		      else if (format[index] == SPECIFIER_UNGROUP)
			{
			  if (--depth <= 0)
			    {
			      index++;
			      break;
			    }
			}
		      index++;
		    }
		}
	      break;
	      
	    case SPECIFIER_INTEGER:
	      parameters[pos].type = FORMAT_INT;
	      break;
	      
	    case SPECIFIER_UNSIGNED:
	      flags |= FLAGS_UNSIGNED;
	      parameters[pos].type = FORMAT_INT;
	      break;

	    case SPECIFIER_DECIMAL:
	      
	      flags &= ~FLAGS_BASE_PARAMETER;
	      base = BASE_DECIMAL;
	      parameters[pos].type = FORMAT_INT;
	      break;

	    case SPECIFIER_OCTAL:
	      flags |= FLAGS_UNSIGNED;
	      flags &= ~FLAGS_BASE_PARAMETER;
	      base = BASE_OCTAL;
	      parameters[pos].type = FORMAT_INT;
	      break;

#if defined(SPECIFIER_BINARY)
	    case SPECIFIER_BINARY_UPPER:
	      flags |= FLAGS_UPPER;
	      
	    case SPECIFIER_BINARY:
	      flags |= FLAGS_NILPADDING;
	      flags &= ~FLAGS_BASE_PARAMETER;
	      base = BASE_BINARY;
	      parameters[pos].type = FORMAT_INT;
	      break;
#endif

	    case SPECIFIER_HEX_UPPER:
	      flags |= FLAGS_UPPER;
	      
	    case SPECIFIER_HEX:
	      flags |= FLAGS_UNSIGNED;
	      flags &= ~FLAGS_BASE_PARAMETER;
	      base = BASE_HEX;
	      parameters[pos].type = FORMAT_INT;
	      break;

	    case SPECIFIER_FLOAT_E_UPPER:
	      flags |= FLAGS_UPPER;
	      
	    case SPECIFIER_FLOAT_E:
	      flags |= FLAGS_FLOAT_E;
	      parameters[pos].type = FORMAT_DOUBLE;
	      break;

	    case SPECIFIER_FLOAT_G_UPPER:
	      flags |= FLAGS_UPPER;
	      
	    case SPECIFIER_FLOAT_G:
	      flags |= FLAGS_FLOAT_G;
	      parameters[pos].type = FORMAT_DOUBLE;
	      break;

	    case SPECIFIER_FLOAT_F_UPPER:
	      flags |= FLAGS_UPPER;
	      
	    case SPECIFIER_FLOAT_F:
	      parameters[pos].type = FORMAT_DOUBLE;
	      break;

	    case SPECIFIER_POINTER:
	      if (sizeof(trio_pointer_t) == sizeof(trio_ulonglong_t))
		flags |= FLAGS_QUAD;
	      else if (sizeof(trio_pointer_t) == sizeof(long))
		flags |= FLAGS_LONG;
	      parameters[pos].type = FORMAT_POINTER;
	      break;

	    case SPECIFIER_COUNT:
	      parameters[pos].type = FORMAT_COUNT;
	      break;

#if defined(SPECIFIER_HEXFLOAT)
# if defined(SPECIFIER_HEXFLOAT_UPPER)
	    case SPECIFIER_HEXFLOAT_UPPER:
	      flags |= FLAGS_UPPER;
	      
# endif
	    case SPECIFIER_HEXFLOAT:
	      base = BASE_HEX;
	      parameters[pos].type = FORMAT_DOUBLE;
	      break;
#endif

#if defined(FORMAT_ERRNO)
	    case SPECIFIER_ERRNO:
	      parameters[pos].type = FORMAT_ERRNO;
	      break;
#endif

#if defined(SPECIFIER_USER_DEFINED_BEGIN)
	    case SPECIFIER_USER_DEFINED_BEGIN:
	      {
		unsigned int max;
		int without_namespace = TRUE;
		
		parameters[pos].type = FORMAT_USER_DEFINED;
		parameters[pos].user_name[0] = NIL;
		tmpformat = (char *)&format[index];
	      
		while ((ch = format[index]))
		  {
		    index++;
		    if (ch == SPECIFIER_USER_DEFINED_END)
		      {
			if (without_namespace)
			  {
			    
			    parameters[pos].type = FORMAT_PARAMETER;
			    parameters[pos].indexAfterSpecifier = index;
			    parameters[pos].flags = FLAGS_USER_DEFINED;
			    
			    pos++;
			    usedEntries[currentParam] += 1;
			    parameters[pos].type = FORMAT_USER_DEFINED;
			    currentParam++;
			    indices[currentParam] = pos;
			    if (currentParam > maxParam)
			      maxParam = currentParam;
			  }
			
			max = (unsigned int)(&format[index] - tmpformat);
			if (max > MAX_USER_DATA)
			  max = MAX_USER_DATA;
			trio_copy_max(parameters[pos].user_data,
				      max,
				      tmpformat);
			break; 
		      }
		    if (ch == SPECIFIER_USER_DEFINED_SEPARATOR)
		      {
			without_namespace = FALSE;
			
			max = (int)(&format[index] - tmpformat);
			if (max > MAX_USER_NAME)
			  max = MAX_USER_NAME;
			trio_copy_max(parameters[pos].user_name,
				      max,
				      tmpformat);
			tmpformat = (char *)&format[index];
		      }
		  }
		if (ch != SPECIFIER_USER_DEFINED_END)
		  return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
	      }
	      break;
#endif 
	      
	    default:
	      
              return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
	    }

	  
	  usedEntries[currentParam] += 1;
	  
	  
	  if (gotSticky && !(flags & FLAGS_STICKY))
	    {
	      for (i = pos - 1; i >= 0; i--)
		{
		  if (parameters[i].type == FORMAT_PARAMETER)
		    continue;
		  if ((parameters[i].flags & FLAGS_STICKY) &&
		      (parameters[i].type == parameters[pos].type))
		    {
		      
		      flags |= (parameters[i].flags & (unsigned long)~FLAGS_STICKY);
		      if (width == NO_WIDTH)
			width = parameters[i].width;
		      if (precision == NO_PRECISION)
			precision = parameters[i].precision;
		      if (base == NO_BASE)
			base = parameters[i].base;
		      break;
		    }
		}
	    }
	  
	  parameters[pos].indexAfterSpecifier = index;
	  parameters[pos].flags = flags;
	  parameters[pos].width = width;
	  parameters[pos].precision = precision;
	  parameters[pos].base = (base == NO_BASE) ? BASE_DECIMAL : base;
	  parameters[pos].varsize = varsize;
	  pos++;
	  
	  if (! positional)
	    parameterPosition++;
	  
	} 
      
    } 

  for (num = 0; num <= maxParam; num++)
    {
      if (usedEntries[num] != 1)
	{
	  if (usedEntries[num] == 0) 
	    return TRIO_ERROR_RETURN(TRIO_EGAP, num);
	  else 
	    return TRIO_ERROR_RETURN(TRIO_EDBLREF, num);
	}
      
      i = indices[num];

      if ((parameters[i].type != FORMAT_PARAMETER) &&
	  (parameters[i].flags & FLAGS_IGNORE))
	continue; 

      switch (parameters[i].type)
	{
	case FORMAT_GROUP:
	case FORMAT_STRING:
#if TRIO_WIDECHAR
	  if (flags & FLAGS_WIDECHAR)
	    {
	      parameters[i].data.wstring = (argarray == NULL)
		? va_arg(*arglist, trio_wchar_t *)
		: (trio_wchar_t *)(argarray[num]);
	    }
	  else
#endif
	    {
	      parameters[i].data.string = (argarray == NULL)
		? va_arg(*arglist, char *)
		: (char *)(argarray[num]);
	    }
	  break;

#if defined(FORMAT_USER_DEFINED)
	case FORMAT_USER_DEFINED:
#endif
	case FORMAT_POINTER:
	case FORMAT_COUNT:
	case FORMAT_UNKNOWN:
	  parameters[i].data.pointer = (argarray == NULL)
	    ? va_arg(*arglist, trio_pointer_t )
	    : argarray[num];
	  break;

	case FORMAT_CHAR:
	case FORMAT_INT:
	  if (TYPE_SCAN == type)
	    {
              if (argarray == NULL)
                parameters[i].data.pointer = 
                  (trio_pointer_t)va_arg(*arglist, trio_pointer_t);
              else
                {
                  if (parameters[i].type == FORMAT_CHAR)
                    parameters[i].data.pointer =
                      (trio_pointer_t)((char *)argarray[num]);
                  else if (parameters[i].flags & FLAGS_SHORT)
                    parameters[i].data.pointer =
                      (trio_pointer_t)((short *)argarray[num]);
                  else
                    parameters[i].data.pointer =
                      (trio_pointer_t)((int *)argarray[num]);
                }
	    }
	  else
	    {
#if defined(QUALIFIER_VARSIZE) || defined(QUALIFIER_FIXED_SIZE)
	      if (parameters[i].flags
		  & (FLAGS_VARSIZE_PARAMETER | FLAGS_FIXED_SIZE))
		{
		  if (parameters[i].flags & FLAGS_VARSIZE_PARAMETER)
		    {
		      varsize = (int)parameters[parameters[i].varsize].data.number.as_unsigned;
		    }
		  else
		    {
		      
		      varsize = parameters[i].varsize;
		    }
		  parameters[i].flags &= ~FLAGS_ALL_VARSIZES;
		  
		  if (varsize <= (int)sizeof(int))
		    ;
		  else if (varsize <= (int)sizeof(long))
		    parameters[i].flags |= FLAGS_LONG;
#if defined(QUALIFIER_INTMAX_T)
		  else if (varsize <= (int)sizeof(trio_longlong_t))
		    parameters[i].flags |= FLAGS_QUAD;
		  else
		    parameters[i].flags |= FLAGS_INTMAX_T;
#else
		  else
		    parameters[i].flags |= FLAGS_QUAD;
#endif
		}
#endif 
#if defined(QUALIFIER_SIZE_T) || defined(QUALIFIER_SIZE_T_UPPER)
	      if (parameters[i].flags & FLAGS_SIZE_T)
		parameters[i].data.number.as_unsigned = (argarray == NULL)
		  ? (trio_uintmax_t)va_arg(*arglist, size_t)
		  : (trio_uintmax_t)(*((size_t *)argarray[num]));
	      else
#endif
#if defined(QUALIFIER_PTRDIFF_T)
	      if (parameters[i].flags & FLAGS_PTRDIFF_T)
		parameters[i].data.number.as_unsigned = (argarray == NULL)
		  ? (trio_uintmax_t)va_arg(*arglist, ptrdiff_t)
		  : (trio_uintmax_t)(*((ptrdiff_t *)argarray[num]));
	      else
#endif
#if defined(QUALIFIER_INTMAX_T)
	      if (parameters[i].flags & FLAGS_INTMAX_T)
		parameters[i].data.number.as_unsigned = (argarray == NULL)
		  ? (trio_uintmax_t)va_arg(*arglist, trio_intmax_t)
		  : (trio_uintmax_t)(*((trio_intmax_t *)argarray[num]));
	      else
#endif
	      if (parameters[i].flags & FLAGS_QUAD)
		parameters[i].data.number.as_unsigned = (argarray == NULL)
		  ? (trio_uintmax_t)va_arg(*arglist, trio_ulonglong_t)
		  : (trio_uintmax_t)(*((trio_ulonglong_t *)argarray[num]));
	      else if (parameters[i].flags & FLAGS_LONG)
		parameters[i].data.number.as_unsigned = (argarray == NULL)
		  ? (trio_uintmax_t)va_arg(*arglist, long)
		  : (trio_uintmax_t)(*((long *)argarray[num]));
	      else
		{
		  if (argarray == NULL)
		    parameters[i].data.number.as_unsigned = (trio_uintmax_t)va_arg(*arglist, int);
		  else
		    {
		      if (parameters[i].type == FORMAT_CHAR)
			parameters[i].data.number.as_unsigned = (trio_uintmax_t)(*((char *)argarray[num]));
		      else if (parameters[i].flags & FLAGS_SHORT)
			parameters[i].data.number.as_unsigned = (trio_uintmax_t)(*((short *)argarray[num]));
		      else
			parameters[i].data.number.as_unsigned = (trio_uintmax_t)(*((int *)argarray[num]));
		    }
		}
	    }
	  break;

	case FORMAT_PARAMETER:
	  if (parameters[i].flags & FLAGS_USER_DEFINED)
	    parameters[i].data.pointer = (argarray == NULL)
	      ? va_arg(*arglist, trio_pointer_t )
	      : argarray[num];
	  else
	    parameters[i].data.number.as_unsigned = (argarray == NULL)
	      ? (trio_uintmax_t)va_arg(*arglist, int)
	      : (trio_uintmax_t)(*((int *)argarray[num]));
	  break;

	case FORMAT_DOUBLE:
	  if (TYPE_SCAN == type)
	    {
	      if (parameters[i].flags & FLAGS_LONGDOUBLE)
		parameters[i].data.longdoublePointer = (argarray == NULL)
		  ? va_arg(*arglist, trio_long_double_t *)
		  : (trio_long_double_t *)argarray[num];
	      else
                {
		  if (parameters[i].flags & FLAGS_LONG)
		    parameters[i].data.doublePointer = (argarray == NULL)
		      ? va_arg(*arglist, double *)
		      : (double *)argarray[num];
		  else
		    parameters[i].data.doublePointer = (argarray == NULL)
		      ? (double *)va_arg(*arglist, float *)
		      : (double *)((float *)argarray[num]);
                }
	    }
	  else
	    {
	      if (parameters[i].flags & FLAGS_LONGDOUBLE)
		parameters[i].data.longdoubleNumber = (argarray == NULL)
		  ? va_arg(*arglist, trio_long_double_t)
		  : (trio_long_double_t)(*((trio_long_double_t *)argarray[num]));
	      else
		{
		  if (argarray == NULL)
		    parameters[i].data.longdoubleNumber =
		      (trio_long_double_t)va_arg(*arglist, double);
		  else
		    {
		      if (parameters[i].flags & FLAGS_SHORT)
			parameters[i].data.longdoubleNumber =
			  (trio_long_double_t)(*((float *)argarray[num]));
		      else
			parameters[i].data.longdoubleNumber =
			  (trio_long_double_t)(*((double *)argarray[num]));
		    }
		}
	    }
	  break;

#if defined(FORMAT_ERRNO)
	case FORMAT_ERRNO:
	  parameters[i].data.errorNumber = save_errno;
	  break;
#endif

	default:
	  break;
	}
    } 
  return num;
}




TRIO_PRIVATE void
TrioWriteNumber
TRIO_ARGS6((self, number, flags, width, precision, base),
	   trio_class_t *self,
	   trio_uintmax_t number,
	   trio_flags_t flags,
	   int width,
	   int precision,
	   int base)
{
  BOOLEAN_T isNegative;
  BOOLEAN_T isNumberZero;
  BOOLEAN_T isPrecisionZero;
  BOOLEAN_T ignoreNumber;
  char buffer[MAX_CHARS_IN(trio_uintmax_t) * (1 + MAX_LOCALE_SEPARATOR_LENGTH) + 1];
  char *bufferend;
  char *pointer;
  TRIO_CONST char *digits;
  int i;
  int length;
  char *p;
  int count;

  assert(VALID(self));
  assert(VALID(self->OutStream));
  assert(((base >= MIN_BASE) && (base <= MAX_BASE)) || (base == NO_BASE));

  digits = (flags & FLAGS_UPPER) ? internalDigitsUpper : internalDigitsLower;
  if (base == NO_BASE)
    base = BASE_DECIMAL;

  isNumberZero = (number == 0);
  isPrecisionZero = (precision == 0);
  ignoreNumber = (isNumberZero
		  && isPrecisionZero
		  && !((flags & FLAGS_ALTERNATIVE) && (base == BASE_OCTAL)));

  if (flags & FLAGS_UNSIGNED)
    {
      isNegative = FALSE;
      flags &= ~FLAGS_SHOWSIGN;
    }
  else
    {
      isNegative = ((trio_intmax_t)number < 0);
      if (isNegative)
	number = -((trio_intmax_t)number);
    }

  if (flags & FLAGS_QUAD)
    number &= (trio_ulonglong_t)-1;
  else if (flags & FLAGS_LONG)
    number &= (unsigned long)-1;
  else
    number &= (unsigned int)-1;
  
  
  pointer = bufferend = &buffer[sizeof(buffer) - 1];
  *pointer-- = NIL;
  for (i = 1; i < (int)sizeof(buffer); i++)
    {
      *pointer-- = digits[number % base];
      number /= base;
      if (number == 0)
	break;

      if ((flags & FLAGS_QUOTE) && TrioFollowedBySeparator(i + 1))
	{
	  length = internalThousandSeparatorLength;
	  if (((int)(pointer - buffer) - length) > 0)
	    {
	      p = &internalThousandSeparator[length - 1];
	      while (length-- > 0)
		*pointer-- = *p--;
	    }
	}
    }

  if (! ignoreNumber)
    {
      
      width -= (bufferend - pointer) - 1;
    }

  
  if (NO_PRECISION != precision)
    {
      precision -= (bufferend - pointer) - 1;
      if (precision < 0)
	precision = 0;
      flags |= FLAGS_NILPADDING;
    }

  
  count = (! ((flags & FLAGS_LEFTADJUST) || (precision == NO_PRECISION)))
    ? precision
    : 0;
  
  
  if (isNegative || (flags & FLAGS_SHOWSIGN) || (flags & FLAGS_SPACE))
    width--;
  if ((flags & FLAGS_ALTERNATIVE) && !isNumberZero)
    {
      switch (base)
	{
	case BASE_BINARY:
	case BASE_HEX:
	  width -= 2;
	  break;
	case BASE_OCTAL:
	  if (!(flags & FLAGS_NILPADDING) || (count == 0))
	    width--;
	  break;
	default:
	  break;
	}
    }

  
  if (! ((flags & FLAGS_LEFTADJUST) ||
	 ((flags & FLAGS_NILPADDING) && (precision == NO_PRECISION))))
    {
      while (width-- > count)
	self->OutStream(self, CHAR_ADJUST);
    }

  
  if (isNegative)
    self->OutStream(self, '-');
  else if (flags & FLAGS_SHOWSIGN)
    self->OutStream(self, '+');
  else if (flags & FLAGS_SPACE)
    self->OutStream(self, ' ');

  /* Prefix is not written when the value is zero */
  if ((flags & FLAGS_ALTERNATIVE) && !isNumberZero)
    {
      switch (base)
	{
	case BASE_BINARY:
	  self->OutStream(self, '0');
	  self->OutStream(self, (flags & FLAGS_UPPER) ? 'B' : 'b');
	  break;

	case BASE_OCTAL:
	  if (!(flags & FLAGS_NILPADDING) || (count == 0))
	    self->OutStream(self, '0');
	  break;

	case BASE_HEX:
	  self->OutStream(self, '0');
	  self->OutStream(self, (flags & FLAGS_UPPER) ? 'X' : 'x');
	  break;

	default:
	  break;
	} 
    }

  
  if (flags & FLAGS_NILPADDING)
    {
      if (precision == NO_PRECISION)
	precision = width;
      while (precision-- > 0)
	{
	  self->OutStream(self, '0');
	  width--;
	}
    }

  if (! ignoreNumber)
    {
      
      while (*(++pointer))
	{
	  self->OutStream(self, *pointer);
	}
    }

  
  if (flags & FLAGS_LEFTADJUST)
    {
      while (width-- > 0)
	self->OutStream(self, CHAR_ADJUST);
    }
}

TRIO_PRIVATE void
TrioWriteStringCharacter
TRIO_ARGS3((self, ch, flags),
	   trio_class_t *self,
	   int ch,
	   trio_flags_t flags)
{
  if (flags & FLAGS_ALTERNATIVE)
    {
      if (! isprint(ch))
	{
	  self->OutStream(self, CHAR_BACKSLASH);
	  switch (ch)
	    {
	    case '\007': self->OutStream(self, 'a'); break;
	    case '\b': self->OutStream(self, 'b'); break;
	    case '\f': self->OutStream(self, 'f'); break;
	    case '\n': self->OutStream(self, 'n'); break;
	    case '\r': self->OutStream(self, 'r'); break;
	    case '\t': self->OutStream(self, 't'); break;
	    case '\v': self->OutStream(self, 'v'); break;
	    case '\\': self->OutStream(self, '\\'); break;
	    default:
	      self->OutStream(self, 'x');
	      TrioWriteNumber(self, (trio_uintmax_t)ch,
			      FLAGS_UNSIGNED | FLAGS_NILPADDING,
			      2, 2, BASE_HEX);
	      break;
	    }
	}
      else if (ch == CHAR_BACKSLASH)
	{
	  self->OutStream(self, CHAR_BACKSLASH);
	  self->OutStream(self, CHAR_BACKSLASH);
	}
      else
	{
	  self->OutStream(self, ch);
	}
    }
  else
    {
      self->OutStream(self, ch);
    }
}

TRIO_PRIVATE void
TrioWriteString
TRIO_ARGS5((self, string, flags, width, precision),
	   trio_class_t *self,
	   TRIO_CONST char *string,
	   trio_flags_t flags,
	   int width,
	   int precision)
{
  int length;
  int ch;

  assert(VALID(self));
  assert(VALID(self->OutStream));

  if (string == NULL)
    {
      string = internalNullString;
      length = sizeof(internalNullString) - 1;
      
      flags &= (~FLAGS_QUOTE);
      width = 0;
    }
  else
    {
      length = trio_length(string);
    }
  if ((NO_PRECISION != precision) &&
      (precision < length))
    {
      length = precision;
    }
  width -= length;

  if (flags & FLAGS_QUOTE)
    self->OutStream(self, CHAR_QUOTE);

  if (! (flags & FLAGS_LEFTADJUST))
    {
      while (width-- > 0)
	self->OutStream(self, CHAR_ADJUST);
    }

  while (length-- > 0)
    {
      
      ch = (int)((unsigned char)(*string++));
      TrioWriteStringCharacter(self, ch, flags);
    }

  if (flags & FLAGS_LEFTADJUST)
    {
      while (width-- > 0)
	self->OutStream(self, CHAR_ADJUST);
    }
  if (flags & FLAGS_QUOTE)
    self->OutStream(self, CHAR_QUOTE);
}

#if TRIO_WIDECHAR
TRIO_PRIVATE int
TrioWriteWideStringCharacter
TRIO_ARGS4((self, wch, flags, width),
	   trio_class_t *self,
	   trio_wchar_t wch,
	   trio_flags_t flags,
	   int width)
{
  int size;
  int i;
  int ch;
  char *string;
  char buffer[MB_LEN_MAX + 1];

  if (width == NO_WIDTH)
    width = sizeof(buffer);
  
  size = wctomb(buffer, wch);
  if ((size <= 0) || (size > width) || (buffer[0] == NIL))
    return 0;

  string = buffer;
  i = size;
  while ((width >= i) && (width-- > 0) && (i-- > 0))
    {
      
      ch = (int)((unsigned char)(*string++));
      TrioWriteStringCharacter(self, ch, flags);
    }
  return size;
}
#endif 

#if TRIO_WIDECHAR
TRIO_PRIVATE void
TrioWriteWideString
TRIO_ARGS5((self, wstring, flags, width, precision),
	   trio_class_t *self,
	   TRIO_CONST trio_wchar_t *wstring,
	   trio_flags_t flags,
	   int width,
	   int precision)
{
  int length;
  int size;

  assert(VALID(self));
  assert(VALID(self->OutStream));

#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  (void)mblen(NULL, 0);
#endif
  
  if (wstring == NULL)
    {
      TrioWriteString(self, NULL, flags, width, precision);
      return;
    }
  
  if (NO_PRECISION == precision)
    {
      length = INT_MAX;
    }
  else
    {
      length = precision;
      width -= length;
    }

  if (flags & FLAGS_QUOTE)
    self->OutStream(self, CHAR_QUOTE);

  if (! (flags & FLAGS_LEFTADJUST))
    {
      while (width-- > 0)
	self->OutStream(self, CHAR_ADJUST);
    }

  while (length > 0)
    {
      size = TrioWriteWideStringCharacter(self, *wstring++, flags, length);
      if (size == 0)
	break; 
      length -= size;
    }

  if (flags & FLAGS_LEFTADJUST)
    {
      while (width-- > 0)
	self->OutStream(self, CHAR_ADJUST);
    }
  if (flags & FLAGS_QUOTE)
    self->OutStream(self, CHAR_QUOTE);
}
#endif 

TRIO_PRIVATE void
TrioWriteDouble
TRIO_ARGS6((self, number, flags, width, precision, base),
	   trio_class_t *self,
	   trio_long_double_t number,
	   trio_flags_t flags,
	   int width,
	   int precision,
	   int base)
{
  trio_long_double_t integerNumber;
  trio_long_double_t fractionNumber;
  trio_long_double_t workNumber;
  int integerDigits;
  int fractionDigits;
  int exponentDigits;
  int baseDigits;
  int integerThreshold;
  int fractionThreshold;
  int expectedWidth;
  int exponent = 0;
  unsigned int uExponent = 0;
  int exponentBase;
  trio_long_double_t dblBase;
  trio_long_double_t dblIntegerBase;
  trio_long_double_t dblFractionBase;
  trio_long_double_t integerAdjust;
  trio_long_double_t fractionAdjust;
  BOOLEAN_T isNegative;
  BOOLEAN_T isExponentNegative = FALSE;
  BOOLEAN_T requireTwoDigitExponent;
  BOOLEAN_T isHex;
  TRIO_CONST char *digits;
  char *groupingPointer;
  int i;
  int index;
  BOOLEAN_T hasOnlyZeroes;
  int zeroes = 0;
  register int trailingZeroes;
  BOOLEAN_T keepTrailingZeroes;
  BOOLEAN_T keepDecimalPoint;
  trio_long_double_t epsilon;
  
  assert(VALID(self));
  assert(VALID(self->OutStream));
  assert(((base >= MIN_BASE) && (base <= MAX_BASE)) || (base == NO_BASE));

  
  switch (trio_fpclassify_and_signbit(number, &isNegative))
    {
    case TRIO_FP_NAN:
      TrioWriteString(self,
		      (flags & FLAGS_UPPER)
		      ? NAN_UPPER
		      : NAN_LOWER,
		      flags, width, precision);
      return;
      
    case TRIO_FP_INFINITE:
      if (isNegative)
	{
	  
	  TrioWriteString(self,
			  (flags & FLAGS_UPPER)
			  ? "-" INFINITE_UPPER
			  : "-" INFINITE_LOWER,
			  flags, width, precision);
	  return;
	}
      else
	{
	  
	  TrioWriteString(self,
			  (flags & FLAGS_UPPER)
			  ? INFINITE_UPPER
			  : INFINITE_LOWER,
			  flags, width, precision);
	  return;
	}

    default:
      
      break;
    }
  
  
  if (flags & FLAGS_LONGDOUBLE)
    {
      baseDigits = (base == 10)
	? LDBL_DIG
	: (int)floor(LDBL_MANT_DIG / TrioLogarithmBase(base));
      epsilon = LDBL_EPSILON;
    }
  else if (flags & FLAGS_SHORT)
    {
      baseDigits = (base == BASE_DECIMAL)
	? FLT_DIG
	: (int)floor(FLT_MANT_DIG / TrioLogarithmBase(base));
      epsilon = FLT_EPSILON;
    }
  else
    {
      baseDigits = (base == BASE_DECIMAL)
	? DBL_DIG
	: (int)floor(DBL_MANT_DIG / TrioLogarithmBase(base));
      epsilon = DBL_EPSILON;
    }

  digits = (flags & FLAGS_UPPER) ? internalDigitsUpper : internalDigitsLower;
  isHex = (base == BASE_HEX);
  if (base == NO_BASE)
    base = BASE_DECIMAL;
  dblBase = (trio_long_double_t)base;
  keepTrailingZeroes = !( (flags & FLAGS_ROUNDING) ||
			  ( (flags & FLAGS_FLOAT_G) &&
			    !(flags & FLAGS_ALTERNATIVE) ) );

  if (flags & FLAGS_ROUNDING)
    precision = baseDigits;

  if (precision == NO_PRECISION)
    {
      if (isHex)
	{
	  keepTrailingZeroes = FALSE;
	  precision = FLT_MANT_DIG;
	}
      else
	{
	  precision = FLT_DIG;
	}
    }
  
  if (isNegative)
    number = -number;

  if (isHex)
    flags |= FLAGS_FLOAT_E;
  
  if (flags & FLAGS_FLOAT_G)
    {
      if (precision == 0)
	precision = 1;

      if ((number < 1.0E-4) || (number > powl(base,
					      (trio_long_double_t)precision)))
	{
	  
	  flags |= FLAGS_FLOAT_E;
	}
      else if (number < 1.0)
	{
	  workNumber = TrioLogarithm(number, base);
	  workNumber = TRIO_FABS(workNumber);
	  if (workNumber - floorl(workNumber) < 0.001)
	    workNumber--;
	  zeroes = (int)floorl(workNumber);
	}
    }

  if (flags & FLAGS_FLOAT_E)
    {
      
      workNumber = TrioLogarithm(number, base);
      if (trio_isinf(workNumber) == -1)
	{
	  exponent = 0;
	  
	  if (flags & FLAGS_FLOAT_G)
	    flags &= ~FLAGS_FLOAT_E;
	}
      else
	{
	  exponent = (int)floorl(workNumber);
	  number /= powl(dblBase, (trio_long_double_t)exponent);
	  isExponentNegative = (exponent < 0);
	  uExponent = (isExponentNegative) ? -exponent : exponent;
	  if (isHex)
	    uExponent *= 4; 
	  
	  flags &= ~FLAGS_QUOTE;
	}
    }

  integerNumber = floorl(number);
  fractionNumber = number - integerNumber;
  
  integerDigits = (integerNumber > epsilon)
    ? 1 + (int)TrioLogarithm(integerNumber, base)
    : 1;
  fractionDigits = ((flags & FLAGS_FLOAT_G) && (zeroes == 0))
    ? precision - integerDigits
    : zeroes + precision;

  dblFractionBase = TrioPower(base, fractionDigits);
  
  workNumber = number + 0.5 / dblFractionBase;
  if (floorl(number) != floorl(workNumber))
    {
      if (flags & FLAGS_FLOAT_E)
	{
	  
	  exponent++;
	  isExponentNegative = (exponent < 0);
	  uExponent = (isExponentNegative) ? -exponent : exponent;
	  if (isHex)
	    uExponent *= 4; 
	  workNumber = (number + 0.5 / dblFractionBase) / dblBase;
	  integerNumber = floorl(workNumber);
	  fractionNumber = workNumber - integerNumber;
	}
      else
	{
	  
	  integerNumber = floorl(number + 0.5);
	  fractionNumber = 0.0;
	  integerDigits = (integerNumber > epsilon)
	    ? 1 + (int)TrioLogarithm(integerNumber, base)
	    : 1;
	}
    }

  
  integerAdjust = fractionAdjust = 0.5;
  if (flags & FLAGS_ROUNDING)
    {
      if (integerDigits > baseDigits)
	{
	  integerThreshold = baseDigits;
	  fractionDigits = 0;
	  dblFractionBase = 1.0;
	  fractionThreshold = 0;
	  precision = 0; 
	  integerAdjust = TrioPower(base, integerDigits - integerThreshold - 1);
	  fractionAdjust = 0.0;
	}
      else
	{
	  integerThreshold = integerDigits;
	  fractionThreshold = fractionDigits - integerThreshold;
	  fractionAdjust = 1.0;
	}
    }
  else
    {
      integerThreshold = INT_MAX;
      fractionThreshold = INT_MAX;
    }
  
  fractionAdjust /= dblFractionBase;
  hasOnlyZeroes = (floorl((fractionNumber + fractionAdjust) * dblFractionBase) < epsilon);
  keepDecimalPoint = ( (flags & FLAGS_ALTERNATIVE) ||
		       !((precision == 0) ||
			 (!keepTrailingZeroes && hasOnlyZeroes)) );
  if (flags & FLAGS_FLOAT_E)
    {
      exponentDigits = (uExponent == 0)
	? 1
	: (int)ceil(TrioLogarithm((double)(uExponent + 1),
				  (isHex) ? 10.0 : base));
    }
  else
    exponentDigits = 0;
  requireTwoDigitExponent = ((base == BASE_DECIMAL) && (exponentDigits == 1));

  expectedWidth = integerDigits + fractionDigits
    + (keepDecimalPoint
       ? internalDecimalPointLength
       : 0)
    + ((flags & FLAGS_QUOTE)
       ? TrioCalcThousandSeparatorLength(integerDigits)
       : 0);
  if (isNegative || (flags & FLAGS_SHOWSIGN) || (flags & FLAGS_SPACE))
    expectedWidth += sizeof("-") - 1;
  if (exponentDigits > 0)
    expectedWidth += exponentDigits +
      ((requireTwoDigitExponent ? sizeof("E+0") : sizeof("E+")) - 1);
  if (isHex)
    expectedWidth += sizeof("0X") - 1;
  
  
  if (flags & FLAGS_NILPADDING)
    {
      
      if (isNegative)
	self->OutStream(self, '-');
      else if (flags & FLAGS_SHOWSIGN)
	self->OutStream(self, '+');
      else if (flags & FLAGS_SPACE)
	self->OutStream(self, ' ');
      if (isHex)
	{
	  self->OutStream(self, '0');
	  self->OutStream(self, (flags & FLAGS_UPPER) ? 'X' : 'x');
	}
      if (!(flags & FLAGS_LEFTADJUST))
	{
	  for (i = expectedWidth; i < width; i++)
	    {
	      self->OutStream(self, '0');
	    }
	}
    }
  else
    {
      
      if (!(flags & FLAGS_LEFTADJUST))
	{
	  for (i = expectedWidth; i < width; i++)
	    {
	      self->OutStream(self, CHAR_ADJUST);
	    }
	}
      if (isNegative)
	self->OutStream(self, '-');
      else if (flags & FLAGS_SHOWSIGN)
	self->OutStream(self, '+');
      else if (flags & FLAGS_SPACE)
	self->OutStream(self, ' ');
      if (isHex)
	{
	  self->OutStream(self, '0');
	  self->OutStream(self, (flags & FLAGS_UPPER) ? 'X' : 'x');
	}
    }
  
  
  dblIntegerBase = 1.0 / TrioPower(base, integerDigits - 1);
  for (i = 0; i < integerDigits; i++)
    {
      workNumber = floorl(((integerNumber + integerAdjust) * dblIntegerBase));
      if (i > integerThreshold)
	{
	  
	  self->OutStream(self, digits[0]);
	}
      else
	{
	  self->OutStream(self, digits[(int)fmodl(workNumber, dblBase)]);
	}
      dblIntegerBase *= dblBase;
      
      if (((flags & (FLAGS_FLOAT_E | FLAGS_QUOTE)) == FLAGS_QUOTE)
	  && TrioFollowedBySeparator(integerDigits - i))
	{
	  for (groupingPointer = internalThousandSeparator;
	       *groupingPointer != NIL;
	       groupingPointer++)
	    {
	      self->OutStream(self, *groupingPointer);
	    }
	}
    }
  
  
  trailingZeroes = 0;

  if (keepDecimalPoint)
    {
      if (internalDecimalPoint)
	{
	  self->OutStream(self, internalDecimalPoint);
	}
      else
	{
	  for (i = 0; i < internalDecimalPointLength; i++)
	    {
	      self->OutStream(self, internalDecimalPointString[i]);
	    }
	}
    }

  for (i = 0; i < fractionDigits; i++)
    {
      if ((integerDigits > integerThreshold) || (i > fractionThreshold))
	{
	  
	  trailingZeroes++;
	}
      else
	{
	  fractionNumber *= dblBase;
	  fractionAdjust *= dblBase;
	  workNumber = floorl(fractionNumber + fractionAdjust);
	  fractionNumber -= workNumber;
	  index = (int)fmodl(workNumber, dblBase);
	  if (index == 0)
	    {
	      trailingZeroes++;
	    }
	  else
	    {
	      while (trailingZeroes > 0)
		{
		  
		  self->OutStream(self, digits[0]);
		  trailingZeroes--;
		}
	      self->OutStream(self, digits[index]);
	    }
	}
    }
  
  if (keepTrailingZeroes)
    {
      while (trailingZeroes > 0)
	{
	  self->OutStream(self, digits[0]);
	  trailingZeroes--;
	}
    }
  
  
  if (exponentDigits > 0)
    {
      self->OutStream(self,
		      isHex
		      ? ((flags & FLAGS_UPPER) ? 'P' : 'p')
		      : ((flags & FLAGS_UPPER) ? 'E' : 'e'));
      self->OutStream(self, (isExponentNegative) ? '-' : '+');

      
      if (requireTwoDigitExponent)
        self->OutStream(self, '0');

      if (isHex)
	base = 10.0;
      exponentBase = (int)TrioPower(base, exponentDigits - 1);
      for (i = 0; i < exponentDigits; i++)
	{
	  self->OutStream(self, digits[(uExponent / exponentBase) % base]);
	  exponentBase /= base;
	}
    }
  
  if (flags & FLAGS_LEFTADJUST)
    {
      for (i = expectedWidth; i < width; i++)
	{
	  self->OutStream(self, CHAR_ADJUST);
	}
    }
}

TRIO_PRIVATE int
TrioFormatProcess
TRIO_ARGS3((data, format, parameters),
	   trio_class_t *data,
	   TRIO_CONST char *format,
	   trio_parameter_t *parameters)
{
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  int charlen;
#endif
  int i;
  TRIO_CONST char *string;
  trio_pointer_t pointer;
  trio_flags_t flags;
  int width;
  int precision;
  int base;
  int index;
  
  index = 0;
  i = 0;
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  (void)mblen(NULL, 0);
#endif
  
  while (format[index])
    {
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
      if (! isascii(format[index]))
	{
	  charlen = mblen(&format[index], MB_LEN_MAX);
	  if (charlen != -1)
	    {
	      while (charlen-- > 0)
		{
		  data->OutStream(data, format[index++]);
		}
	      continue; 
	    }
	}
#endif 
      if (CHAR_IDENTIFIER == format[index])
	{
	  if (CHAR_IDENTIFIER == format[index + 1])
	    {
	      data->OutStream(data, CHAR_IDENTIFIER);
	      index += 2;
	    }
	  else
	    {
	      
	      while (parameters[i].type == FORMAT_PARAMETER)
		i++;
	      
	      flags = parameters[i].flags;

	      
	      width = parameters[i].width;
	      if (flags & FLAGS_WIDTH_PARAMETER)
		{
		  
		  width = (int)parameters[width].data.number.as_signed;
		  if (width < 0)
		    {
		      flags |= FLAGS_LEFTADJUST;
		      flags &= ~FLAGS_NILPADDING;
		      width = -width;
		    }
		}
	      
	      
	      if (flags & FLAGS_PRECISION)
		{
		  precision = parameters[i].precision;
		  if (flags & FLAGS_PRECISION_PARAMETER)
		    {
		      
		      precision = (int)parameters[precision].data.number.as_signed;
		      if (precision < 0)
			{
			  precision = NO_PRECISION;
			}
		    }
		}
	      else
		{
		  precision = NO_PRECISION;
		}

	      
	      base = parameters[i].base;
	      if (flags & FLAGS_BASE_PARAMETER)
		{
		  
		  base = (int)parameters[base].data.number.as_signed;
		}
	      
	      switch (parameters[i].type)
		{
		case FORMAT_CHAR:
		  if (flags & FLAGS_QUOTE)
		    data->OutStream(data, CHAR_QUOTE);
		  if (! (flags & FLAGS_LEFTADJUST))
		    {
		      while (--width > 0)
			data->OutStream(data, CHAR_ADJUST);
		    }
#if TRIO_WIDECHAR
		  if (flags & FLAGS_WIDECHAR)
		    {
		      TrioWriteWideStringCharacter(data,
						   (trio_wchar_t)parameters[i].data.number.as_signed,
						   flags,
						   NO_WIDTH);
		    }
		  else
#endif
		    {
		      TrioWriteStringCharacter(data,
					       (int)parameters[i].data.number.as_signed,
					       flags);
		    }

		  if (flags & FLAGS_LEFTADJUST)
		    {
		      while(--width > 0)
			data->OutStream(data, CHAR_ADJUST);
		    }
		  if (flags & FLAGS_QUOTE)
		    data->OutStream(data, CHAR_QUOTE);

		  break; 

		case FORMAT_INT:
		  TrioWriteNumber(data,
				  parameters[i].data.number.as_unsigned,
				  flags,
				  width,
				  precision,
				  base);

		  break; 

		case FORMAT_DOUBLE:
		  TrioWriteDouble(data,
				  parameters[i].data.longdoubleNumber,
				  flags,
				  width,
				  precision,
				  base);
		  break; 

		case FORMAT_STRING:
#if TRIO_WIDECHAR
		  if (flags & FLAGS_WIDECHAR)
		    {
		      TrioWriteWideString(data,
					  parameters[i].data.wstring,
					  flags,
					  width,
					  precision);
		    }
		  else
#endif
		    {
		      TrioWriteString(data,
				      parameters[i].data.string,
				      flags,
				      width,
				      precision);
		    }
		  break; 

		case FORMAT_POINTER:
		  {
		    trio_reference_t reference;
		    
		    reference.data = data;
		    reference.parameter = &parameters[i];
		    trio_print_pointer(&reference, parameters[i].data.pointer);
		  }
		  break; 

		case FORMAT_COUNT:
		  pointer = parameters[i].data.pointer;
		  if (NULL != pointer)
		    {
		      /*
		       * C99 paragraph 7.19.6.1.8 says "the number of
		       * characters written to the output stream so far by
		       * this call", which is data->committed
		       */
#if defined(QUALIFIER_SIZE_T) || defined(QUALIFIER_SIZE_T_UPPER)
		      if (flags & FLAGS_SIZE_T)
			*(size_t *)pointer = (size_t)data->committed;
		      else
#endif
#if defined(QUALIFIER_PTRDIFF_T)
		      if (flags & FLAGS_PTRDIFF_T)
			*(ptrdiff_t *)pointer = (ptrdiff_t)data->committed;
		      else
#endif
#if defined(QUALIFIER_INTMAX_T)
		      if (flags & FLAGS_INTMAX_T)
			*(trio_intmax_t *)pointer = (trio_intmax_t)data->committed;
		      else
#endif
		      if (flags & FLAGS_QUAD)
			{
			  *(trio_ulonglong_t *)pointer = (trio_ulonglong_t)data->committed;
			}
		      else if (flags & FLAGS_LONG)
			{
			  *(long int *)pointer = (long int)data->committed;
			}
		      else if (flags & FLAGS_SHORT)
			{
			  *(short int *)pointer = (short int)data->committed;
			}
		      else
			{
			  *(int *)pointer = (int)data->committed;
			}
		    }
		  break; 

		case FORMAT_PARAMETER:
		  break; 

#if defined(FORMAT_ERRNO)
		case FORMAT_ERRNO:
		  string = trio_error(parameters[i].data.errorNumber);
		  if (string)
		    {
		      TrioWriteString(data,
				      string,
				      flags,
				      width,
				      precision);
		    }
		  else
		    {
		      data->OutStream(data, '#');
		      TrioWriteNumber(data,
				      (trio_uintmax_t)parameters[i].data.errorNumber,
				      flags,
				      width,
				      precision,
				      BASE_DECIMAL);
		    }
		  break; 
#endif 

#if defined(FORMAT_USER_DEFINED)
		case FORMAT_USER_DEFINED:
		  {
		    trio_reference_t reference;
		    trio_userdef_t *def = NULL;

		    if (parameters[i].user_name[0] == NIL)
		      {
			
			if ((i > 0) ||
			    (parameters[i - 1].type == FORMAT_PARAMETER))
			  def = (trio_userdef_t *)parameters[i - 1].data.pointer;
		      }
		    else
		      {
			
			def = TrioFindNamespace(parameters[i].user_name, NULL);
		      }
		    if (def) {
		      reference.data = data;
		      reference.parameter = &parameters[i];
		      def->callback(&reference);
		    }
		  }
		  break;
#endif 
		  
		default:
		  break;
		} 

	      
	      index = parameters[i].indexAfterSpecifier;
	      i++;
	    }
	}
      else 
	{
	  data->OutStream(data, format[index++]);
	}
    }
  return data->processed;
}

TRIO_PRIVATE int
TrioFormatRef
TRIO_ARGS4((reference, format, arglist, argarray),
	   trio_reference_t *reference,
	   TRIO_CONST char *format,
	   va_list *arglist,
	   trio_pointer_t *argarray)
{
  int status;
  trio_parameter_t parameters[MAX_PARAMETERS];

  status = TrioParse(TYPE_PRINT, format, parameters, arglist, argarray);
  if (status < 0)
    return status;

  status = TrioFormatProcess(reference->data, format, parameters);
  if (reference->data->error != 0)
    {
      status = reference->data->error;
    }
  return status;
}

TRIO_PRIVATE int
TrioFormat
TRIO_ARGS6((destination, destinationSize, OutStream, format, arglist, argarray),
	   trio_pointer_t destination,
	   size_t destinationSize,
	   void (*OutStream) TRIO_PROTO((trio_class_t *, int)),
	   TRIO_CONST char *format,
	   va_list *arglist,
	   trio_pointer_t *argarray)
{
  int status;
  trio_class_t data;
  trio_parameter_t parameters[MAX_PARAMETERS];

  assert(VALID(OutStream));
  assert(VALID(format));

  memset(&data, 0, sizeof(data));
  data.OutStream = OutStream;
  data.location = destination;
  data.max = destinationSize;
  data.error = 0;

#if defined(USE_LOCALE)
  if (NULL == internalLocaleValues)
    {
      TrioSetLocale();
    }
#endif

  status = TrioParse(TYPE_PRINT, format, parameters, arglist, argarray);
  if (status < 0)
    return status;

  status = TrioFormatProcess(&data, format, parameters);
  if (data.error != 0)
    {
      status = data.error;
    }
  return status;
}

TRIO_PRIVATE void
TrioOutStreamFile
TRIO_ARGS2((self, output),
	   trio_class_t *self,
	   int output)
{
  FILE *file;

  assert(VALID(self));
  assert(VALID(self->location));

  file = (FILE *)self->location;
  self->processed++;
  if (fputc(output, file) == EOF)
    {
      self->error = TRIO_ERROR_RETURN(TRIO_EOF, 0);
    }
  else
    {
      self->committed++;
    }
}

TRIO_PRIVATE void
TrioOutStreamFileDescriptor
TRIO_ARGS2((self, output),
	   trio_class_t *self,
	   int output)
{
  int fd;
  char ch;

  assert(VALID(self));

  fd = *((int *)self->location);
  ch = (char)output;
  self->processed++;
  if (write(fd, &ch, sizeof(char)) == -1)
    {
      self->error = TRIO_ERROR_RETURN(TRIO_ERRNO, 0);
    }
  else
    {
      self->committed++;
    }
}

TRIO_PRIVATE void
TrioOutStreamCustom
TRIO_ARGS2((self, output),
	   trio_class_t *self,
	   int output)
{
  int status;
  trio_custom_t *data;

  assert(VALID(self));
  assert(VALID(self->location));

  data = (trio_custom_t *)self->location;
  if (data->stream.out)
    {
      status = (data->stream.out)(data->closure, output);
      if (status >= 0)
	{
	  self->committed++;
	}
      else
	{
	  if (self->error == 0)
	    {
	      self->error = TRIO_ERROR_RETURN(TRIO_ECUSTOM, -status);
	    }
	}
    }
  self->processed++;
}

TRIO_PRIVATE void
TrioOutStreamString
TRIO_ARGS2((self, output),
	   trio_class_t *self,
	   int output)
{
  char **buffer;

  assert(VALID(self));
  assert(VALID(self->location));

  buffer = (char **)self->location;
  **buffer = (char)output;
  (*buffer)++;
  self->processed++;
  self->committed++;
}

TRIO_PRIVATE void
TrioOutStreamStringMax
TRIO_ARGS2((self, output),
	   trio_class_t *self,
	   int output)
{
  char **buffer;

  assert(VALID(self));
  assert(VALID(self->location));
  
  buffer = (char **)self->location;

  if (self->processed < self->max)
    {
      **buffer = (char)output;
      (*buffer)++;
      self->committed++;
    }
  self->processed++;
}

TRIO_PRIVATE void
TrioOutStreamStringDynamic
TRIO_ARGS2((self, output),
	   trio_class_t *self,
	   int output)
{
  assert(VALID(self));
  assert(VALID(self->location));

  if (self->error == 0)
    {
      trio_xstring_append_char((trio_string_t *)self->location,
			       (char)output);
      self->committed++;
    }
  
  self->processed++;
}


#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_printf.h"
#endif


TRIO_PUBLIC int
trio_printf
TRIO_VARGS2((format, va_alist),
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioFormat(stdout, 0, TrioOutStreamFile, format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vprintf
TRIO_ARGS2((format, args),
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(format));

  return TrioFormat(stdout, 0, TrioOutStreamFile, format, &args, NULL);
}

TRIO_PUBLIC int
trio_printfv
TRIO_ARGS2((format, args),
	   TRIO_CONST char *format,
	   trio_pointer_t * args)
{
  assert(VALID(format));

  return TrioFormat(stdout, 0, TrioOutStreamFile, format, NULL, args);
}


TRIO_PUBLIC int
trio_fprintf
TRIO_VARGS3((file, format, va_alist),
	    FILE *file,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(file));
  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioFormat(file, 0, TrioOutStreamFile, format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vfprintf
TRIO_ARGS3((file, format, args),
	   FILE *file,
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(file));
  assert(VALID(format));
  
  return TrioFormat(file, 0, TrioOutStreamFile, format, &args, NULL);
}

TRIO_PUBLIC int
trio_fprintfv
TRIO_ARGS3((file, format, args),
	   FILE *file,
	   TRIO_CONST char *format,
	   trio_pointer_t * args)
{
  assert(VALID(file));
  assert(VALID(format));
  
  return TrioFormat(file, 0, TrioOutStreamFile, format, NULL, args);
}


TRIO_PUBLIC int
trio_dprintf
TRIO_VARGS3((fd, format, va_alist),
	    int fd,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioFormat(&fd, 0, TrioOutStreamFileDescriptor, format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vdprintf
TRIO_ARGS3((fd, format, args),
	   int fd,
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(format));
  
  return TrioFormat(&fd, 0, TrioOutStreamFileDescriptor, format, &args, NULL);
}

TRIO_PUBLIC int
trio_dprintfv
TRIO_ARGS3((fd, format, args),
	   int fd,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  assert(VALID(format));
  
  return TrioFormat(&fd, 0, TrioOutStreamFileDescriptor, format, NULL, args);
}

TRIO_PUBLIC int
trio_cprintf
TRIO_VARGS4((stream, closure, format, va_alist),
	    trio_outstream_t stream,
	    trio_pointer_t closure,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;
  trio_custom_t data;

  assert(VALID(stream));
  assert(VALID(format));

  TRIO_VA_START(args, format);
  data.stream.out = stream;
  data.closure = closure;
  status = TrioFormat(&data, 0, TrioOutStreamCustom, format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vcprintf
TRIO_ARGS4((stream, closure, format, args),
	   trio_outstream_t stream,
	   trio_pointer_t closure,
	   TRIO_CONST char *format,
	   va_list args)
{
  trio_custom_t data;

  assert(VALID(stream));
  assert(VALID(format));

  data.stream.out = stream;
  data.closure = closure;
  return TrioFormat(&data, 0, TrioOutStreamCustom, format, &args, NULL);
}

TRIO_PUBLIC int
trio_cprintfv
TRIO_ARGS4((stream, closure, format, args),
	   trio_outstream_t stream,
	   trio_pointer_t closure,
	   TRIO_CONST char *format,
	   void **args)
{
  trio_custom_t data;

  assert(VALID(stream));
  assert(VALID(format));

  data.stream.out = stream;
  data.closure = closure;
  return TrioFormat(&data, 0, TrioOutStreamCustom, format, NULL, args);
}


TRIO_PUBLIC int
trio_sprintf
TRIO_VARGS3((buffer, format, va_alist),
	    char *buffer,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(buffer));
  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioFormat(&buffer, 0, TrioOutStreamString, format, &args, NULL);
  *buffer = NIL; 
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vsprintf
TRIO_ARGS3((buffer, format, args),
	   char *buffer,
	   TRIO_CONST char *format,
	   va_list args)
{
  int status;

  assert(VALID(buffer));
  assert(VALID(format));

  status = TrioFormat(&buffer, 0, TrioOutStreamString, format, &args, NULL);
  *buffer = NIL;
  return status;
}

TRIO_PUBLIC int
trio_sprintfv
TRIO_ARGS3((buffer, format, args),
	   char *buffer,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  int status;

  assert(VALID(buffer));
  assert(VALID(format));

  status = TrioFormat(&buffer, 0, TrioOutStreamString, format, NULL, args);
  *buffer = NIL;
  return status;
}


TRIO_PUBLIC int
trio_snprintf
TRIO_VARGS4((buffer, max, format, va_alist),
	    char *buffer,
	    size_t max,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(buffer));
  assert(VALID(format));

  TRIO_VA_START(args, format);
  status = TrioFormat(&buffer, max > 0 ? max - 1 : 0,
		      TrioOutStreamStringMax, format, &args, NULL);
  if (max > 0)
    *buffer = NIL;
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vsnprintf
TRIO_ARGS4((buffer, max, format, args),
	   char *buffer,
	   size_t max,
	   TRIO_CONST char *format,
	   va_list args)
{
  int status;

  assert(VALID(buffer));
  assert(VALID(format));

  status = TrioFormat(&buffer, max > 0 ? max - 1 : 0,
		      TrioOutStreamStringMax, format, &args, NULL);
  if (max > 0)
    *buffer = NIL;
  return status;
}

TRIO_PUBLIC int
trio_snprintfv
TRIO_ARGS4((buffer, max, format, args),
	   char *buffer,
	   size_t max,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  int status;

  assert(VALID(buffer));
  assert(VALID(format));

  status = TrioFormat(&buffer, max > 0 ? max - 1 : 0,
		      TrioOutStreamStringMax, format, NULL, args);
  if (max > 0)
    *buffer = NIL;
  return status;
}

TRIO_PUBLIC int
trio_snprintfcat
TRIO_VARGS4((buffer, max, format, va_alist),
	    char *buffer,
	    size_t max,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;
  size_t buf_len;

  TRIO_VA_START(args, format);

  assert(VALID(buffer));
  assert(VALID(format));

  buf_len = trio_length(buffer);
  buffer = &buffer[buf_len];

  status = TrioFormat(&buffer, max - 1 - buf_len,
		      TrioOutStreamStringMax, format, &args, NULL);
  TRIO_VA_END(args);
  *buffer = NIL;
  return status;
}

TRIO_PUBLIC int
trio_vsnprintfcat
TRIO_ARGS4((buffer, max, format, args),
	   char *buffer,
	   size_t max,
	   TRIO_CONST char *format,
	   va_list args)
{
  int status;
  size_t buf_len;
  
  assert(VALID(buffer));
  assert(VALID(format));

  buf_len = trio_length(buffer);
  buffer = &buffer[buf_len];
  status = TrioFormat(&buffer, max - 1 - buf_len,
		      TrioOutStreamStringMax, format, &args, NULL);
  *buffer = NIL;
  return status;
}


TRIO_PUBLIC char *
trio_aprintf
TRIO_VARGS2((format, va_alist),
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  va_list args;
  trio_string_t *info;
  char *result = NULL;

  assert(VALID(format));
  
  info = trio_xstring_duplicate("");
  if (info)
    {
      TRIO_VA_START(args, format);
      (void)TrioFormat(info, 0, TrioOutStreamStringDynamic,
		       format, &args, NULL);
      TRIO_VA_END(args);

      trio_string_terminate(info);
      result = trio_string_extract(info);
      trio_string_destroy(info);
    }
  return result;
}

TRIO_PUBLIC char *
trio_vaprintf
TRIO_ARGS2((format, args),
	   TRIO_CONST char *format,
	   va_list args)
{
  trio_string_t *info;
  char *result = NULL;
  
  assert(VALID(format));
  
  info = trio_xstring_duplicate("");
  if (info)
    {
      (void)TrioFormat(info, 0, TrioOutStreamStringDynamic,
		       format, &args, NULL);
      trio_string_terminate(info);
      result = trio_string_extract(info);
      trio_string_destroy(info);
    }
  return result;
}

TRIO_PUBLIC int
trio_asprintf
TRIO_VARGS3((result, format, va_alist),
	    char **result,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  va_list args;
  int status;
  trio_string_t *info;

  assert(VALID(format));

  *result = NULL;
  
  info = trio_xstring_duplicate("");
  if (info == NULL)
    {
      status = TRIO_ERROR_RETURN(TRIO_ENOMEM, 0);
    }
  else
    {
      TRIO_VA_START(args, format);
      status = TrioFormat(info, 0, TrioOutStreamStringDynamic,
			  format, &args, NULL);
      TRIO_VA_END(args);
      if (status >= 0)
	{
	  trio_string_terminate(info);
	  *result = trio_string_extract(info);
	}
      trio_string_destroy(info);
    }
  return status;
}

TRIO_PUBLIC int
trio_vasprintf
TRIO_ARGS3((result, format, args),
	   char **result,
	   TRIO_CONST char *format,
	   va_list args)
{
  int status;
  trio_string_t *info;
  
  assert(VALID(format));

  *result = NULL;
  
  info = trio_xstring_duplicate("");
  if (info == NULL)
    {
      status = TRIO_ERROR_RETURN(TRIO_ENOMEM, 0);
    }
  else
    {
      status = TrioFormat(info, 0, TrioOutStreamStringDynamic,
			  format, &args, NULL);
      if (status >= 0)
	{
	  trio_string_terminate(info);
	  *result = trio_string_extract(info);
	}
      trio_string_destroy(info);
    }
  return status;
}



#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_register.h"
#endif

#if TRIO_EXTENSION


TRIO_PUBLIC trio_pointer_t 
trio_register
TRIO_ARGS2((callback, name),
	   trio_callback_t callback,
	   TRIO_CONST char *name)
{
  trio_userdef_t *def;
  trio_userdef_t *prev = NULL;

  if (callback == NULL)
    return NULL;

  if (name)
    {
      
      if (name[0] == ':')
	{
	  if (trio_equal(name, ":enter"))
	    {
	      internalEnterCriticalRegion = callback;
	    }
	  else if (trio_equal(name, ":leave"))
	    {
	      internalLeaveCriticalRegion = callback;
	    }
	  return NULL;
	}
      
      
      if (trio_length(name) >= MAX_USER_NAME)
	return NULL;
      
      
      def = TrioFindNamespace(name, &prev);
      if (def)
	return NULL;
    }
  
  def = (trio_userdef_t *)TRIO_MALLOC(sizeof(trio_userdef_t));
  if (def)
    {
      if (internalEnterCriticalRegion)
	(void)internalEnterCriticalRegion(NULL);
      
      if (name)
	{
	  
	  if (prev == NULL)
	    internalUserDef = def;
	  else
	    prev->next = def;
	}
      
      def->callback = callback;
      def->name = (name == NULL)
	? NULL
	: trio_duplicate(name);
      def->next = NULL;

      if (internalLeaveCriticalRegion)
	(void)internalLeaveCriticalRegion(NULL);
    }
  return (trio_pointer_t)def;
}

void
trio_unregister
TRIO_ARGS1((handle),
	   trio_pointer_t handle)
{
  trio_userdef_t *self = (trio_userdef_t *)handle;
  trio_userdef_t *def;
  trio_userdef_t *prev = NULL;

  assert(VALID(self));

  if (self->name)
    {
      def = TrioFindNamespace(self->name, &prev);
      if (def)
	{
	  if (internalEnterCriticalRegion)
	    (void)internalEnterCriticalRegion(NULL);
	  
	  if (prev == NULL)
	    internalUserDef = NULL;
	  else
	    prev->next = def->next;
	  
	  if (internalLeaveCriticalRegion)
	    (void)internalLeaveCriticalRegion(NULL);
	}
      trio_destroy(self->name);
    }
  TRIO_FREE(self);
}

TRIO_CONST char *
trio_get_format
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
#if defined(FORMAT_USER_DEFINED)
  assert(((trio_reference_t *)ref)->parameter->type == FORMAT_USER_DEFINED);
#endif
  
  return (((trio_reference_t *)ref)->parameter->user_data);
}

trio_pointer_t 
trio_get_argument
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
#if defined(FORMAT_USER_DEFINED)
  assert(((trio_reference_t *)ref)->parameter->type == FORMAT_USER_DEFINED);
#endif
  
  return ((trio_reference_t *)ref)->parameter->data.pointer;
}

int
trio_get_width
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return ((trio_reference_t *)ref)->parameter->width;
}

void
trio_set_width
TRIO_ARGS2((ref, width),
	   trio_pointer_t ref,
	   int width)
{
  ((trio_reference_t *)ref)->parameter->width = width;
}

int
trio_get_precision
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->precision);
}

void
trio_set_precision
TRIO_ARGS2((ref, precision),
	   trio_pointer_t ref,
	   int precision)
{
  ((trio_reference_t *)ref)->parameter->precision = precision;
}

int
trio_get_base
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->base);
}

void
trio_set_base
TRIO_ARGS2((ref, base),
	   trio_pointer_t ref,
	   int base)
{
  ((trio_reference_t *)ref)->parameter->base = base;
}

int
trio_get_long
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_LONG)
    ? TRUE
    : FALSE;
}

void
trio_set_long
TRIO_ARGS2((ref, is_long),
	   trio_pointer_t ref,
	   int is_long)
{
  if (is_long)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_LONG;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_LONG;
}

int
trio_get_longlong
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_QUAD)
    ? TRUE
    : FALSE;
}

void
trio_set_longlong
TRIO_ARGS2((ref, is_longlong),
	   trio_pointer_t ref,
	   int is_longlong)
{
  if (is_longlong)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_QUAD;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_QUAD;
}

int
trio_get_longdouble
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_LONGDOUBLE)
    ? TRUE
    : FALSE;
}

void
trio_set_longdouble
TRIO_ARGS2((ref, is_longdouble),
	   trio_pointer_t ref,
	   int is_longdouble)
{
  if (is_longdouble)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_LONGDOUBLE;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_LONGDOUBLE;
}

int
trio_get_short
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_SHORT)
    ? TRUE
    : FALSE;
}

void
trio_set_short
TRIO_ARGS2((ref, is_short),
	   trio_pointer_t ref,
	   int is_short)
{
  if (is_short)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_SHORT;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_SHORT;
}

int
trio_get_shortshort
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_SHORTSHORT)
    ? TRUE
    : FALSE;
}

void
trio_set_shortshort
TRIO_ARGS2((ref, is_shortshort),
	   trio_pointer_t ref,
	   int is_shortshort)
{
  if (is_shortshort)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_SHORTSHORT;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_SHORTSHORT;
}

int
trio_get_alternative
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_ALTERNATIVE)
    ? TRUE
    : FALSE;
}

void
trio_set_alternative
TRIO_ARGS2((ref, is_alternative),
	   trio_pointer_t ref,
	   int is_alternative)
{
  if (is_alternative)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_ALTERNATIVE;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_ALTERNATIVE;
}

int
trio_get_alignment
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_LEFTADJUST)
    ? TRUE
    : FALSE;
}

void
trio_set_alignment
TRIO_ARGS2((ref, is_leftaligned),
	   trio_pointer_t ref,
	   int is_leftaligned)
{
  if (is_leftaligned)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_LEFTADJUST;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_LEFTADJUST;
}

int
trio_get_spacing
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_SPACE)
    ? TRUE
    : FALSE;
}

void
trio_set_spacing
TRIO_ARGS2((ref, is_space),
	   trio_pointer_t ref,
	   int is_space)
{
  if (is_space)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_SPACE;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_SPACE;
}

int
trio_get_sign
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_SHOWSIGN)
    ? TRUE
    : FALSE;
}

void
trio_set_sign
TRIO_ARGS2((ref, is_sign),
	   trio_pointer_t ref,
	   int is_sign)
{
  if (is_sign)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_SHOWSIGN;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_SHOWSIGN;
}

int
trio_get_padding
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_NILPADDING)
    ? TRUE
    : FALSE;
}

void
trio_set_padding
TRIO_ARGS2((ref, is_padding),
	   trio_pointer_t ref,
	   int is_padding)
{
  if (is_padding)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_NILPADDING;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_NILPADDING;
}

int
trio_get_quote
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_QUOTE)
    ? TRUE
    : FALSE;
}

void
trio_set_quote
TRIO_ARGS2((ref, is_quote),
	   trio_pointer_t ref,
	   int is_quote)
{
  if (is_quote)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_QUOTE;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_QUOTE;
}

int
trio_get_upper
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_UPPER)
    ? TRUE
    : FALSE;
}

void
trio_set_upper
TRIO_ARGS2((ref, is_upper),
	   trio_pointer_t ref,
	   int is_upper)
{
  if (is_upper)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_UPPER;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_UPPER;
}

#if TRIO_C99
int
trio_get_largest
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_INTMAX_T)
    ? TRUE
    : FALSE;
}

void
trio_set_largest
TRIO_ARGS2((ref, is_largest),
	   trio_pointer_t ref,
	   int is_largest)
{
  if (is_largest)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_INTMAX_T;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_INTMAX_T;
}
#endif

int
trio_get_ptrdiff
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_PTRDIFF_T)
    ? TRUE
    : FALSE;
}

void
trio_set_ptrdiff
TRIO_ARGS2((ref, is_ptrdiff),
	   trio_pointer_t ref,
	   int is_ptrdiff)
{
  if (is_ptrdiff)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_PTRDIFF_T;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_PTRDIFF_T;
}

#if TRIO_C99
int
trio_get_size
TRIO_ARGS1((ref),
	   trio_pointer_t ref)
{
  return (((trio_reference_t *)ref)->parameter->flags & FLAGS_SIZE_T)
    ? TRUE
    : FALSE;
}

void
trio_set_size
TRIO_ARGS2((ref, is_size),
	   trio_pointer_t ref,
	   int is_size)
{
  if (is_size)
    ((trio_reference_t *)ref)->parameter->flags |= FLAGS_SIZE_T;
  else
    ((trio_reference_t *)ref)->parameter->flags &= ~FLAGS_SIZE_T;
}
#endif

void
trio_print_int
TRIO_ARGS2((ref, number),
	   trio_pointer_t ref,
	   int number)
{
  trio_reference_t *self = (trio_reference_t *)ref;

  TrioWriteNumber(self->data,
		  (trio_uintmax_t)number,
		  self->parameter->flags,
		  self->parameter->width,
		  self->parameter->precision,
		  self->parameter->base);
}

void
trio_print_uint
TRIO_ARGS2((ref, number),
	   trio_pointer_t ref,
	   unsigned int number)
{
  trio_reference_t *self = (trio_reference_t *)ref;

  TrioWriteNumber(self->data,
		  (trio_uintmax_t)number,
		  self->parameter->flags | FLAGS_UNSIGNED,
		  self->parameter->width,
		  self->parameter->precision,
		  self->parameter->base);
}

void
trio_print_double
TRIO_ARGS2((ref, number),
	   trio_pointer_t ref,
	   double number)
{
  trio_reference_t *self = (trio_reference_t *)ref;

  TrioWriteDouble(self->data,
		  number,
		  self->parameter->flags,
		  self->parameter->width,
		  self->parameter->precision,
		  self->parameter->base);
}

void
trio_print_string
TRIO_ARGS2((ref, string),
	   trio_pointer_t ref,
	   char *string)
{
  trio_reference_t *self = (trio_reference_t *)ref;

  TrioWriteString(self->data,
		  string,
		  self->parameter->flags,
		  self->parameter->width,
		  self->parameter->precision);
}

int
trio_print_ref
TRIO_VARGS3((ref, format, va_alist),
	    trio_pointer_t ref,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list arglist;

  assert(VALID(format));
  
  TRIO_VA_START(arglist, format);
  status = TrioFormatRef((trio_reference_t *)ref, format, &arglist, NULL);
  TRIO_VA_END(arglist);
  return status;
}

int
trio_vprint_ref
TRIO_ARGS3((ref, format, arglist),
	   trio_pointer_t ref,
	   TRIO_CONST char *format,
	   va_list arglist)
{
  assert(VALID(format));
  
  return TrioFormatRef((trio_reference_t *)ref, format, &arglist, NULL);
}

int
trio_printv_ref
TRIO_ARGS3((ref, format, argarray),
	   trio_pointer_t ref,
	   TRIO_CONST char *format,
	   trio_pointer_t *argarray)
{
  assert(VALID(format));
  
  return TrioFormatRef((trio_reference_t *)ref, format, NULL, argarray);
}

#endif 

void
trio_print_pointer
TRIO_ARGS2((ref, pointer),
	   trio_pointer_t ref,
	   trio_pointer_t pointer)
{
  trio_reference_t *self = (trio_reference_t *)ref;
  trio_flags_t flags;
  trio_uintmax_t number;

  if (NULL == pointer)
    {
      TRIO_CONST char *string = internalNullString;
      while (*string)
	self->data->OutStream(self->data, *string++);
    }
  else
    {
      number = (trio_uintmax_t)((char *)pointer - (char *)0);
      
      number &= (trio_uintmax_t)-1;
      flags = self->parameter->flags;
      flags |= (FLAGS_UNSIGNED | FLAGS_ALTERNATIVE |
	        FLAGS_NILPADDING);
      TrioWriteNumber(self->data,
		      number,
		      flags,
		      POINTER_WIDTH,
		      NO_PRECISION,
		      BASE_HEX);
    }
}



TRIO_PUBLIC void
trio_locale_set_decimal_point
TRIO_ARGS1((decimalPoint),
	   char *decimalPoint)
{
#if defined(USE_LOCALE)
  if (NULL == internalLocaleValues)
    {
      TrioSetLocale();
    }
#endif
  internalDecimalPointLength = trio_length(decimalPoint);
  if (internalDecimalPointLength == 1)
    {
      internalDecimalPoint = *decimalPoint;
    }
  else
    {
      internalDecimalPoint = NIL;
      trio_copy_max(internalDecimalPointString,
		    sizeof(internalDecimalPointString),
		    decimalPoint);
    }
}

TRIO_PUBLIC void
trio_locale_set_thousand_separator
TRIO_ARGS1((thousandSeparator),
	   char *thousandSeparator)
{
#if defined(USE_LOCALE)
  if (NULL == internalLocaleValues)
    {
      TrioSetLocale();
    }
#endif
  trio_copy_max(internalThousandSeparator,
		sizeof(internalThousandSeparator),
		thousandSeparator);
  internalThousandSeparatorLength = trio_length(internalThousandSeparator);
}

TRIO_PUBLIC void
trio_locale_set_grouping
TRIO_ARGS1((grouping),
	   char *grouping)
{
#if defined(USE_LOCALE)
  if (NULL == internalLocaleValues)
    {
      TrioSetLocale();
    }
#endif
  trio_copy_max(internalGrouping,
		sizeof(internalGrouping),
		grouping);
}



TRIO_PRIVATE int
TrioSkipWhitespaces
TRIO_ARGS1((self),
	   trio_class_t *self)
{
  int ch;

  ch = self->current;
  while (isspace(ch))
    {
      self->InStream(self, &ch);
    }
  return ch;
}

#if TRIO_EXTENSION
TRIO_PRIVATE void
TrioGetCollation(TRIO_NOARGS)
{
  int i;
  int j;
  int k;
  char first[2];
  char second[2];

  
  first[1] = NIL;
  second[1] = NIL;
  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
    {
      k = 0;
      first[0] = (char)i;
      for (j = 0; j < MAX_CHARACTER_CLASS; j++)
	{
	  second[0] = (char)j;
	  if (trio_equal_locale(first, second))
	    internalCollationArray[i][k++] = (char)j;
	}
      internalCollationArray[i][k] = NIL;
    }
}
#endif

TRIO_PRIVATE int
TrioGetCharacterClass
TRIO_ARGS4((format, indexPointer, flagsPointer, characterclass),
	   TRIO_CONST char *format,
	   int *indexPointer,
	   trio_flags_t *flagsPointer,
	   int *characterclass)
{
  int index = *indexPointer;
  int i;
  char ch;
  char range_begin;
  char range_end;

  *flagsPointer &= ~FLAGS_EXCLUDE;

  if (format[index] == QUALIFIER_CIRCUMFLEX)
    {
      *flagsPointer |= FLAGS_EXCLUDE;
      index++;
    }
  if (format[index] == SPECIFIER_UNGROUP)
    {
      characterclass[(int)SPECIFIER_UNGROUP]++;
      index++;
    }
  if (format[index] == QUALIFIER_MINUS)
    {
      characterclass[(int)QUALIFIER_MINUS]++;
      index++;
    }
  
  for (ch = format[index];
       (ch != SPECIFIER_UNGROUP) && (ch != NIL);
       ch = format[++index])
    {
      switch (ch)
	{
	case QUALIFIER_MINUS: 
	  
	  range_begin = format[index - 1];
	  range_end = format[++index];
	  if (range_end == SPECIFIER_UNGROUP)
	    {
	      
	      characterclass[(int)ch]++;
	      ch = range_end;
	      break; 
	    }
	  if (range_end == NIL)
	    return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
	  if (range_begin > range_end)
	    return TRIO_ERROR_RETURN(TRIO_ERANGE, index);
	    
	  for (i = (int)range_begin; i <= (int)range_end; i++)
	    characterclass[i]++;
	    
	  ch = range_end;
	  break;
	  
#if TRIO_EXTENSION

	case SPECIFIER_GROUP:
	  
	  switch (format[index + 1])
	    {
	    case QUALIFIER_DOT: 
	      for (i = index + 2; ; i++)
		{
		  if (format[i] == NIL)
		    
		    return -1;
		  else if (format[i] == QUALIFIER_DOT)
		    break; 
		}
	      if (format[++i] != SPECIFIER_UNGROUP)
		return -1;
	      
	      index = i;
	      break;
	  
	    case QUALIFIER_EQUAL: 
	      {
		unsigned int j;
		unsigned int k;
	    
		if (internalCollationUnconverted)
		  {
		    
		    TrioGetCollation();
		    internalCollationUnconverted = FALSE;
		  }
		for (i = index + 2; ; i++)
		  {
		    if (format[i] == NIL)
		      
		      return -1;
		    else if (format[i] == QUALIFIER_EQUAL)
		      break; 
		    else
		      {
			
			k = (unsigned int)format[i];
			for (j = 0; internalCollationArray[k][j] != NIL; j++)
			  characterclass[(int)internalCollationArray[k][j]]++;
		      }
		  }
		if (format[++i] != SPECIFIER_UNGROUP)
		  return -1;
		
		index = i;
	      }
	      break;
	  
	    case QUALIFIER_COLON: 
	  
	      if (trio_equal_max(CLASS_ALNUM, sizeof(CLASS_ALNUM) - 1,
				 &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isalnum(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_ALNUM) - 1;
		}
	      else if (trio_equal_max(CLASS_ALPHA, sizeof(CLASS_ALPHA) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isalpha(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_ALPHA) - 1;
		}
	      else if (trio_equal_max(CLASS_CNTRL, sizeof(CLASS_CNTRL) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (iscntrl(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_CNTRL) - 1;
		}
	      else if (trio_equal_max(CLASS_DIGIT, sizeof(CLASS_DIGIT) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isdigit(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_DIGIT) - 1;
		}
	      else if (trio_equal_max(CLASS_GRAPH, sizeof(CLASS_GRAPH) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isgraph(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_GRAPH) - 1;
		}
	      else if (trio_equal_max(CLASS_LOWER, sizeof(CLASS_LOWER) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (islower(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_LOWER) - 1;
		}
	      else if (trio_equal_max(CLASS_PRINT, sizeof(CLASS_PRINT) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isprint(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_PRINT) - 1;
		}
	      else if (trio_equal_max(CLASS_PUNCT, sizeof(CLASS_PUNCT) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (ispunct(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_PUNCT) - 1;
		}
	      else if (trio_equal_max(CLASS_SPACE, sizeof(CLASS_SPACE) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isspace(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_SPACE) - 1;
		}
	      else if (trio_equal_max(CLASS_UPPER, sizeof(CLASS_UPPER) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isupper(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_UPPER) - 1;
		}
	      else if (trio_equal_max(CLASS_XDIGIT, sizeof(CLASS_XDIGIT) - 1,
				      &format[index]))
		{
		  for (i = 0; i < MAX_CHARACTER_CLASS; i++)
		    if (isxdigit(i))
		      characterclass[i]++;
		  index += sizeof(CLASS_XDIGIT) - 1;
		}
	      else
		{
		  characterclass[(int)ch]++;
		}
	      break;

	    default:
	      characterclass[(int)ch]++;
	      break;
	    }
	  break;
	  
#endif 
	  
	default:
	  characterclass[(int)ch]++;
	  break;
	}
    }
  return 0;
}

TRIO_PRIVATE BOOLEAN_T
TrioReadNumber
TRIO_ARGS5((self, target, flags, width, base),
	   trio_class_t *self,
	   trio_uintmax_t *target,
	   trio_flags_t flags,
	   int width,
	   int base)
{
  trio_uintmax_t number = 0;
  int digit;
  int count;
  BOOLEAN_T isNegative = FALSE;
  BOOLEAN_T gotNumber = FALSE;
  int j;

  assert(VALID(self));
  assert(VALID(self->InStream));
  assert((base >= MIN_BASE && base <= MAX_BASE) || (base == NO_BASE));

  if (internalDigitsUnconverted)
    {
      
      memset(internalDigitArray, -1, sizeof(internalDigitArray));
      for (j = 0; j < (int)sizeof(internalDigitsLower) - 1; j++)
	{
	  internalDigitArray[(int)internalDigitsLower[j]] = j;
	  internalDigitArray[(int)internalDigitsUpper[j]] = j;
	}
      internalDigitsUnconverted = FALSE;
    }
  
  TrioSkipWhitespaces(self);
  
  if (!(flags & FLAGS_UNSIGNED))
    {
      
      if (self->current == '+')
	{
	  self->InStream(self, NULL);
	}
      else if (self->current == '-')
	{
	  self->InStream(self, NULL);
	  isNegative = TRUE;
	}
    }
  
  count = self->processed;
  
  if (flags & FLAGS_ALTERNATIVE)
    {
      switch (base)
	{
	case NO_BASE:
	case BASE_OCTAL:
	case BASE_HEX:
	case BASE_BINARY:
	  if (self->current == '0')
	    {
	      self->InStream(self, NULL);
	      if (self->current)
		{
		  if ((base == BASE_HEX) &&
		      (trio_to_upper(self->current) == 'X'))
		    {
		      self->InStream(self, NULL);
		    }
		  else if ((base == BASE_BINARY) &&
			   (trio_to_upper(self->current) == 'B'))
		    {
		      self->InStream(self, NULL);
		    }
		}
	    }
	  else
	    return FALSE;
	  break;
	default:
	  break;
	}
    }

  while (((width == NO_WIDTH) || (self->processed - count < width)) &&
	 (! ((self->current == EOF) || isspace(self->current))))
    {
      if (isascii(self->current))
	{
	  digit = internalDigitArray[self->current];
	  
	  if ((digit == -1) || (digit >= base))
	    break;
	}
      else if (flags & FLAGS_QUOTE)
	{
	  
	  for (j = 0; internalThousandSeparator[j] && self->current; j++)
	    {
	      if (internalThousandSeparator[j] != self->current)
		break;

	      self->InStream(self, NULL);
	    }
	  if (internalThousandSeparator[j])
	    break; 
	  else
	    continue; 
	}
      else
	break;
            
      number *= base;
      number += digit;
      gotNumber = TRUE; 

      self->InStream(self, NULL);
    }

  
  if (!gotNumber)
    return FALSE;
  
  if (target)
    *target = (isNegative) ? -((trio_intmax_t)number) : number;
  return TRUE;
}

TRIO_PRIVATE int
TrioReadChar
TRIO_ARGS4((self, target, flags, width),
	   trio_class_t *self,
	   char *target,
	   trio_flags_t flags,
	   int width)
{
  int i;
  char ch;
  trio_uintmax_t number;
  
  assert(VALID(self));
  assert(VALID(self->InStream));

  for (i = 0;
       (self->current != EOF) && (i < width);
       i++)
    {
      ch = (char)self->current;
      self->InStream(self, NULL);
      if ((flags & FLAGS_ALTERNATIVE) && (ch == CHAR_BACKSLASH))
	{
	  switch (self->current)
	    {
	    case '\\': ch = '\\'; break;
	    case 'a': ch = '\007'; break;
	    case 'b': ch = '\b'; break;
	    case 'f': ch = '\f'; break;
	    case 'n': ch = '\n'; break;
	    case 'r': ch = '\r'; break;
	    case 't': ch = '\t'; break;
	    case 'v': ch = '\v'; break;
	    default:
	      if (isdigit(self->current))
		{
		  
		  if (!TrioReadNumber(self, &number, 0, 3, BASE_OCTAL))
		    return 0;
		  ch = (char)number;
		}
	      else if (trio_to_upper(self->current) == 'X')
		{
		  
		  self->InStream(self, NULL);
		  if (!TrioReadNumber(self, &number, 0, 2, BASE_HEX))
		    return 0;
		  ch = (char)number;
		}
	      else
		{
		  ch = (char)self->current;
		}
	      break;
	    }
	}
      
      if (target)
	target[i] = ch;
    }
  return i + 1;
}

TRIO_PRIVATE BOOLEAN_T
TrioReadString
TRIO_ARGS4((self, target, flags, width),
	   trio_class_t *self,
	   char *target,
	   trio_flags_t flags,
	   int width)
{
  int i;
  
  assert(VALID(self));
  assert(VALID(self->InStream));

  TrioSkipWhitespaces(self);
    
  for (i = 0;
       ((width == NO_WIDTH) || (i < width)) &&
       (! ((self->current == EOF) || isspace(self->current)));
       i++)
    {
      if (TrioReadChar(self, (target ? &target[i] : 0), flags, 1) == 0)
	break; 
    }
  if (target)
    target[i] = NIL;
  return TRUE;
}

#if TRIO_WIDECHAR
TRIO_PRIVATE int
TrioReadWideChar
TRIO_ARGS4((self, target, flags, width),
	   trio_class_t *self,
	   trio_wchar_t *target,
	   trio_flags_t flags,
	   int width)
{
  int i;
  int j;
  int size;
  int amount = 0;
  trio_wchar_t wch;
  char buffer[MB_LEN_MAX + 1];
  
  assert(VALID(self));
  assert(VALID(self->InStream));

  for (i = 0;
       (self->current != EOF) && (i < width);
       i++)
    {
      if (isascii(self->current))
	{
	  if (TrioReadChar(self, buffer, flags, 1) == 0)
	    return 0;
	  buffer[1] = NIL;
	}
      else
	{
	  j = 0;
	  do
	    {
	      buffer[j++] = (char)self->current;
	      buffer[j] = NIL;
	      self->InStream(self, NULL);
	    }
	  while ((j < (int)sizeof(buffer)) && (mblen(buffer, (size_t)j) != j));
	}
      if (target)
	{
	  size = mbtowc(&wch, buffer, sizeof(buffer));
	  if (size > 0)
	    target[i] = wch;
	}
      amount += size;
      self->InStream(self, NULL);
    }
  return amount;
}
#endif 

#if TRIO_WIDECHAR
TRIO_PRIVATE BOOLEAN_T
TrioReadWideString
TRIO_ARGS4((self, target, flags, width),
	   trio_class_t *self,
	   trio_wchar_t *target,
	   trio_flags_t flags,
	   int width)
{
  int i;
  int size;
  
  assert(VALID(self));
  assert(VALID(self->InStream));

  TrioSkipWhitespaces(self);

#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  (void)mblen(NULL, 0);
#endif
  
  for (i = 0;
       ((width == NO_WIDTH) || (i < width)) &&
       (! ((self->current == EOF) || isspace(self->current)));
       )
    {
      size = TrioReadWideChar(self, &target[i], flags, 1);
      if (size == 0)
	break; 

      i += size;
    }
  if (target)
    target[i] = WCONST('\0');
  return TRUE;
}
#endif 

TRIO_PRIVATE BOOLEAN_T
TrioReadGroup
TRIO_ARGS5((self, target, characterclass, flags, width),
	   trio_class_t *self,
	   char *target,
	   int *characterclass,
	   trio_flags_t flags,
	   int width)
{
  int ch;
  int i;
  
  assert(VALID(self));
  assert(VALID(self->InStream));

  ch = self->current;
  for (i = 0;
       ((width == NO_WIDTH) || (i < width)) &&
       (! ((ch == EOF) ||
	   (((flags & FLAGS_EXCLUDE) != 0) ^ (characterclass[ch] == 0))));
       i++)
    {
      if (target)
	target[i] = (char)ch;
      self->InStream(self, &ch);
    }
  
  if (target)
    target[i] = NIL;
  return TRUE;
}

TRIO_PRIVATE BOOLEAN_T
TrioReadDouble
TRIO_ARGS4((self, target, flags, width),
	   trio_class_t *self,
	   trio_pointer_t target,
	   trio_flags_t flags,
	   int width)
{
  int ch;
  char doubleString[512];
  int index = 0;
  int start;
  int j;
  BOOLEAN_T isHex = FALSE;

  doubleString[0] = 0;
  
  if ((width == NO_WIDTH) || (width > (int)sizeof(doubleString) - 1))
    width = sizeof(doubleString) - 1;
  
  TrioSkipWhitespaces(self);
  
  ch = self->current;
  if ((ch == '+') || (ch == '-'))
    {
      doubleString[index++] = (char)ch;
      self->InStream(self, &ch);
      width--;
    }

  start = index;
  switch (ch)
    {
    case 'n':
    case 'N':
      
      if (index != 0)
	break;
      
    case 'i':
    case 'I':
      
      while (isalpha(ch) && (index - start < width))
	{
	  doubleString[index++] = (char)ch;
	  self->InStream(self, &ch);
	}
      doubleString[index] = NIL;

      
      if (trio_equal(&doubleString[start], INFINITE_UPPER) ||
	  trio_equal(&doubleString[start], LONG_INFINITE_UPPER))
	{
	  if (flags & FLAGS_LONGDOUBLE)
	    {
	      if ((start == 1) && (doubleString[0] == '-'))
		{
		  *((trio_long_double_t *)target) = trio_ninf();
		}
	      else
		{
		  *((trio_long_double_t *)target) = trio_pinf();
		}
	    }
	  else
	    {
	      if ((start == 1) && (doubleString[0] == '-'))
		{
		  *((double *)target) = trio_ninf();
		}
	      else
		{
		  *((double *)target) = trio_pinf();
		}
	    }
	  return TRUE;
	}
      if (trio_equal(doubleString, NAN_UPPER))
	{
	  
	  if (flags & FLAGS_LONGDOUBLE)
	    {
	      *((trio_long_double_t *)target) = trio_nan();
	    }
	  else
	    {
	      *((double *)target) = trio_nan();
	    }
	  return TRUE;
	}
      return FALSE;

    case '0':
      doubleString[index++] = (char)ch;
      self->InStream(self, &ch);
      if (trio_to_upper(ch) == 'X')
	{
	  isHex = TRUE;
	  doubleString[index++] = (char)ch;
	  self->InStream(self, &ch);
	}
      break;
      
    default:
      break;
    }
  
  while ((ch != EOF) && (index - start < width))
    {
      
      if (isHex ? isxdigit(ch) : isdigit(ch))
	{
	  doubleString[index++] = (char)ch;
	  self->InStream(self, &ch);
	}
      else if (flags & FLAGS_QUOTE)
	{
	  
	  for (j = 0; internalThousandSeparator[j] && self->current; j++)
	    {
	      if (internalThousandSeparator[j] != self->current)
		break;

	      self->InStream(self, &ch);
	    }
	  if (internalThousandSeparator[j])
	    break; 
	  else
	    continue; 
	}
      else
	break; 
    }
  if (ch == '.')
    {
      
      doubleString[index++] = (char)ch;
      self->InStream(self, &ch);
      while ((isHex ? isxdigit(ch) : isdigit(ch)) &&
	     (index - start < width))
	{
	  doubleString[index++] = (char)ch;
	  self->InStream(self, &ch);
	}
      if (isHex ? (trio_to_upper(ch) == 'P') : (trio_to_upper(ch) == 'E'))
	{
	  
	  doubleString[index++] = (char)ch;
	  self->InStream(self, &ch);
	  if ((ch == '+') || (ch == '-'))
	    {
	      doubleString[index++] = (char)ch;
	      self->InStream(self, &ch);
	    }
	  while (isdigit(ch) && (index - start < width))
	    {
	      doubleString[index++] = (char)ch;
	      self->InStream(self, &ch);
	    }
	}
    }

  if ((index == start) || (*doubleString == NIL))
    return FALSE;

  doubleString[index] = 0;
  
  if (flags & FLAGS_LONGDOUBLE)
    {
      *((trio_long_double_t *)target) = trio_to_long_double(doubleString, NULL);
    }
  else
    {
      *((double *)target) = trio_to_double(doubleString, NULL);
    }
  return TRUE;
}

TRIO_PRIVATE BOOLEAN_T
TrioReadPointer
TRIO_ARGS3((self, target, flags),
	   trio_class_t *self,
	   trio_pointer_t *target,
	   trio_flags_t flags)
{
  trio_uintmax_t number;
  char buffer[sizeof(internalNullString)];

  flags |= (FLAGS_UNSIGNED | FLAGS_ALTERNATIVE | FLAGS_NILPADDING);
  
  if (TrioReadNumber(self,
		     &number,
		     flags,
		     POINTER_WIDTH,
		     BASE_HEX))
    {
      if (target)
	*target = (char *)0 + number;
      return TRUE;
    }
  else if (TrioReadString(self,
			  (flags & FLAGS_IGNORE)
			  ? NULL
			  : buffer,
			  0,
			  sizeof(internalNullString) - 1))
    {  
      if (trio_equal_case(buffer, internalNullString))
	{
	  if (target)
	    *target = NULL;
	  return TRUE;
	}
    }
  return FALSE;
}

TRIO_PRIVATE int
TrioScanProcess
TRIO_ARGS3((data, format, parameters),
	   trio_class_t *data,
	   TRIO_CONST char *format,
	   trio_parameter_t *parameters)
{
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  int charlen;
  int cnt;
#endif
  int assignment;
  int ch;
  int index; 
  int i; 
  trio_flags_t flags;
  int width;
  int base;
  trio_pointer_t pointer;

  assignment = 0;
  i = 0;
  index = 0;
  data->InStream(data, &ch);

#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
  (void)mblen(NULL, 0);
#endif

  while (format[index])
    {
#if defined(TRIO_COMPILER_SUPPORTS_MULTIBYTE)
      if (! isascii(format[index]))
	{
	  charlen = mblen(&format[index], MB_LEN_MAX);
	  if (charlen != -1)
	    {
	      
	      for (cnt = 0; cnt < charlen - 1; cnt++)
		{
		  if (ch != format[index + cnt])
		    {
		      return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
		    }
		  data->InStream(data, &ch);
		}
	      continue; 
	    }
	}
#endif 
      
      if ((EOF == ch) && (parameters[i].type != FORMAT_COUNT))
	{
	  return (assignment > 0) ? assignment : EOF;
	}
      
      if (CHAR_IDENTIFIER == format[index])
	{
	  if (CHAR_IDENTIFIER == format[index + 1])
	    {
	      
	      if (CHAR_IDENTIFIER == ch)
		{
		  data->InStream(data, &ch);
		  index += 2;
		  continue; 
		}
	      else
		return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
	    }

	  
	  while (parameters[i].type == FORMAT_PARAMETER)
	    i++;
	  
	  flags = parameters[i].flags;
	  
	  width = parameters[i].width;
	  if (flags & FLAGS_WIDTH_PARAMETER)
	    {
	      
	      width = (int)parameters[width].data.number.as_signed;
	    }
	  
	  base = parameters[i].base;
	  if (flags & FLAGS_BASE_PARAMETER)
	    {
	      
	      base = (int)parameters[base].data.number.as_signed;
	    }
	  
	  switch (parameters[i].type)
	    {
	    case FORMAT_INT:
	      {
		trio_uintmax_t number;

		if (0 == base)
		  base = BASE_DECIMAL;

		if (!TrioReadNumber(data,
				    &number,
				    flags,
				    width,
				    base))
		  return assignment;

		if (!(flags & FLAGS_IGNORE))
		  {
		    assignment++;

		    pointer = parameters[i].data.pointer;
#if defined(QUALIFIER_SIZE_T) || defined(QUALIFIER_SIZE_T_UPPER)
		    if (flags & FLAGS_SIZE_T)
		      *(size_t *)pointer = (size_t)number;
		    else
#endif
#if defined(QUALIFIER_PTRDIFF_T)
		    if (flags & FLAGS_PTRDIFF_T)
		      *(ptrdiff_t *)pointer = (ptrdiff_t)number;
		    else
#endif
#if defined(QUALIFIER_INTMAX_T)
		    if (flags & FLAGS_INTMAX_T)
		      *(trio_intmax_t *)pointer = (trio_intmax_t)number;
		    else
#endif
		    if (flags & FLAGS_QUAD)
		      *(trio_ulonglong_t *)pointer = (trio_ulonglong_t)number;
		    else if (flags & FLAGS_LONG)
		      *(long int *)pointer = (long int)number;
		    else if (flags & FLAGS_SHORT)
		      *(short int *)pointer = (short int)number;
		    else
		      *(int *)pointer = (int)number;
		  }
	      }
	      break; 
	      
	    case FORMAT_STRING:
#if TRIO_WIDECHAR
	      if (flags & FLAGS_WIDECHAR)
		{
		  if (!TrioReadWideString(data,
					  (flags & FLAGS_IGNORE)
					  ? NULL
					  : parameters[i].data.wstring,
					  flags,
					  width))
		    return assignment;
		}
	      else
#endif
		{
		  if (!TrioReadString(data,
				      (flags & FLAGS_IGNORE)
				      ? NULL
				      : parameters[i].data.string,
				      flags,
				      width))
		    return assignment;
		}
	      if (!(flags & FLAGS_IGNORE))
		assignment++;
	      break; 

	    case FORMAT_DOUBLE:
	      {
		trio_pointer_t pointer;

		if (flags & FLAGS_IGNORE)
		  {
		    pointer = NULL;
		  }
		else
		  {
		    pointer = (flags & FLAGS_LONGDOUBLE)
		      ? (trio_pointer_t)parameters[i].data.longdoublePointer
		      : (trio_pointer_t)parameters[i].data.doublePointer;
		  }
		if (!TrioReadDouble(data, pointer, flags, width))
		  {
		    return assignment;
		  }
		if (!(flags & FLAGS_IGNORE))
		  {
		    assignment++;
		  }
		break; 
	      }
	    case FORMAT_GROUP:
	      {
		int characterclass[MAX_CHARACTER_CLASS + 1];
		int rc;

		
		while (format[index] != SPECIFIER_GROUP)
		  {
		    index++;
		  }
		
		index++;
		
		memset(characterclass, 0, sizeof(characterclass));
		rc = TrioGetCharacterClass(format,
					   &index,
					   &flags,
					   characterclass);
		if (rc < 0)
		  return rc;

		if (!TrioReadGroup(data,
				   (flags & FLAGS_IGNORE)
				   ? NULL
				   : parameters[i].data.string,
				   characterclass,
				   flags,
				   parameters[i].width))
		  return assignment;
		if (!(flags & FLAGS_IGNORE))
		  assignment++;
	      }
	      break; 

	    case FORMAT_COUNT:
	      pointer = parameters[i].data.pointer;
	      if (NULL != pointer)
		{
		  int count = data->committed;
		  if (ch != EOF)
		    count--; 
#if defined(QUALIFIER_SIZE_T) || defined(QUALIFIER_SIZE_T_UPPER)
		  if (flags & FLAGS_SIZE_T)
		    *(size_t *)pointer = (size_t)count;
		  else
#endif
#if defined(QUALIFIER_PTRDIFF_T)
		  if (flags & FLAGS_PTRDIFF_T)
		    *(ptrdiff_t *)pointer = (ptrdiff_t)count;
		  else
#endif
#if defined(QUALIFIER_INTMAX_T)
		  if (flags & FLAGS_INTMAX_T)
		    *(trio_intmax_t *)pointer = (trio_intmax_t)count;
		  else
#endif
		  if (flags & FLAGS_QUAD)
		    {
		      *(trio_ulonglong_t *)pointer = (trio_ulonglong_t)count;
		    }
		  else if (flags & FLAGS_LONG)
		    {
		      *(long int *)pointer = (long int)count;
		    }
		  else if (flags & FLAGS_SHORT)
		    {
		      *(short int *)pointer = (short int)count;
		    }
		  else
		    {
		      *(int *)pointer = (int)count;
		    }
		}
	      break; 
	      
	    case FORMAT_CHAR:
#if TRIO_WIDECHAR
	      if (flags & FLAGS_WIDECHAR)
		{
		  if (TrioReadWideChar(data,
				       (flags & FLAGS_IGNORE)
				       ? NULL
				       : parameters[i].data.wstring,
				       flags,
				       (width == NO_WIDTH) ? 1 : width) == 0)
		    return assignment;
		}
	      else
#endif
		{
		  if (TrioReadChar(data,
				   (flags & FLAGS_IGNORE)
				   ? NULL
				   : parameters[i].data.string,
				   flags,
				   (width == NO_WIDTH) ? 1 : width) == 0)
		    return assignment;
		}
	      if (!(flags & FLAGS_IGNORE))
		assignment++;
	      break; 

	    case FORMAT_POINTER:
	      if (!TrioReadPointer(data,
				   (flags & FLAGS_IGNORE)
				   ? NULL
				   : (trio_pointer_t *)parameters[i].data.pointer,
				   flags))
		return assignment;
	      if (!(flags & FLAGS_IGNORE))
		assignment++;
	      break; 

	    case FORMAT_PARAMETER:
	      break; 

	    default:
	      return TRIO_ERROR_RETURN(TRIO_EINVAL, index);
	    }
	  ch = data->current;
	  index = parameters[i].indexAfterSpecifier;
	  i++;
	}
      else 
	{
	  if (isspace((int)format[index]))
	    {
	      
	      ch = TrioSkipWhitespaces(data);
	    }
	  else if (ch == format[index])
	    {
	      data->InStream(data, &ch);
	    }
	  else
	    return assignment;
	  
	  index++;
	}
    }
  return assignment;
}

TRIO_PRIVATE int
TrioScan
TRIO_ARGS6((source, sourceSize, InStream, format, arglist, argarray),
	   trio_pointer_t source,
	   size_t sourceSize,
	   void (*InStream) TRIO_PROTO((trio_class_t *, int *)),
	   TRIO_CONST char *format,
	   va_list *arglist,
	   trio_pointer_t *argarray)
{
  int status;
  trio_parameter_t parameters[MAX_PARAMETERS];
  trio_class_t data;

  assert(VALID(InStream));
  assert(VALID(format));

  memset(&data, 0, sizeof(data));
  data.InStream = InStream;
  data.location = (trio_pointer_t)source;
  data.max = sourceSize;
  data.error = 0;

#if defined(USE_LOCALE)
  if (NULL == internalLocaleValues)
    {
      TrioSetLocale();
    }
#endif
  
  status = TrioParse(TYPE_SCAN, format, parameters, arglist, argarray);
  if (status < 0)
    return status;

  status = TrioScanProcess(&data, format, parameters);
  if (data.error != 0)
    {
      status = data.error;
    }
  return status;
}

TRIO_PRIVATE void
TrioInStreamFile
TRIO_ARGS2((self, intPointer),
	   trio_class_t *self,
	   int *intPointer)
{
  FILE *file = (FILE *)self->location;

  assert(VALID(self));
  assert(VALID(file));

  self->current = fgetc(file);
  if (self->current == EOF)
    {
      self->error = (ferror(file))
	? TRIO_ERROR_RETURN(TRIO_ERRNO, 0)
	: TRIO_ERROR_RETURN(TRIO_EOF, 0);
    }
  else
    {
      self->processed++;
      self->committed++;
    }
  
  if (VALID(intPointer))
    {
      *intPointer = self->current;
    }
}

TRIO_PRIVATE void
TrioInStreamFileDescriptor
TRIO_ARGS2((self, intPointer),
	   trio_class_t *self,
	   int *intPointer)
{
  int fd = *((int *)self->location);
  int size;
  unsigned char input;

  assert(VALID(self));

  size = read(fd, &input, sizeof(char));
  if (size == -1)
    {
      self->error = TRIO_ERROR_RETURN(TRIO_ERRNO, 0);
      self->current = EOF;
    }
  else
    {
      self->current = (size == 0) ? EOF : input;
    }
  if (self->current != EOF)
    {
      self->committed++;
      self->processed++;
    }
  
  if (VALID(intPointer))
    {
      *intPointer = self->current;
    }
}

TRIO_PRIVATE void
TrioInStreamCustom
TRIO_ARGS2((self, intPointer),
	   trio_class_t *self,
	   int *intPointer)
{
  trio_custom_t *data;
  
  assert(VALID(self));
  assert(VALID(self->location));

  data = (trio_custom_t *)self->location;

  self->current = (data->stream.in == NULL)
    ? NIL
    : (data->stream.in)(data->closure);
  
  if (self->current == NIL)
    {
      self->current = EOF;
    }
  else
    {
      self->processed++;
      self->committed++;
    }
  
  if (VALID(intPointer))
    {
      *intPointer = self->current;
    }
}

TRIO_PRIVATE void
TrioInStreamString
TRIO_ARGS2((self, intPointer),
	   trio_class_t *self,
	   int *intPointer)
{
  unsigned char **buffer;

  assert(VALID(self));
  assert(VALID(self->location));

  buffer = (unsigned char **)self->location;
  self->current = (*buffer)[0];
  if (self->current == NIL)
    {
      self->current = EOF;
    }
  else
    {
      (*buffer)++;
      self->processed++;
      self->committed++;
    }
  
  if (VALID(intPointer))
    {
      *intPointer = self->current;
    }
}


#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_scanf.h"
#endif


TRIO_PUBLIC int
trio_scanf
TRIO_VARGS2((format, va_alist),
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioScan((trio_pointer_t)stdin, 0,
		    TrioInStreamFile,
		    format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vscanf
TRIO_ARGS2((format, args),
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)stdin, 0,
		  TrioInStreamFile,
		  format, &args, NULL);
}

TRIO_PUBLIC int
trio_scanfv
TRIO_ARGS2((format, args),
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)stdin, 0,
		  TrioInStreamFile,
		  format, NULL, args);
}

TRIO_PUBLIC int
trio_fscanf
TRIO_VARGS3((file, format, va_alist),
	    FILE *file,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(file));
  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioScan((trio_pointer_t)file, 0,
		    TrioInStreamFile,
		    format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vfscanf
TRIO_ARGS3((file, format, args),
	   FILE *file,
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(file));
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)file, 0,
		  TrioInStreamFile,
		  format, &args, NULL);
}

TRIO_PUBLIC int
trio_fscanfv
TRIO_ARGS3((file, format, args),
	   FILE *file,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  assert(VALID(file));
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)file, 0,
		  TrioInStreamFile,
		  format, NULL, args);
}

TRIO_PUBLIC int
trio_dscanf
TRIO_VARGS3((fd, format, va_alist),
	    int fd,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioScan((trio_pointer_t)&fd, 0,
		    TrioInStreamFileDescriptor,
		    format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vdscanf
TRIO_ARGS3((fd, format, args),
	   int fd,
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)&fd, 0,
		  TrioInStreamFileDescriptor,
		  format, &args, NULL);
}

TRIO_PUBLIC int
trio_dscanfv
TRIO_ARGS3((fd, format, args),
	   int fd,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)&fd, 0,
		  TrioInStreamFileDescriptor,
		  format, NULL, args);
}

TRIO_PUBLIC int
trio_cscanf
TRIO_VARGS4((stream, closure, format, va_alist),
	    trio_instream_t stream,
	    trio_pointer_t closure,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;
  trio_custom_t data;

  assert(VALID(stream));
  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  data.stream.in = stream;
  data.closure = closure;
  status = TrioScan(&data, 0, TrioInStreamCustom, format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vcscanf
TRIO_ARGS4((stream, closure, format, args),
	   trio_instream_t stream,
	   trio_pointer_t closure,
	   TRIO_CONST char *format,
	   va_list args)
{
  trio_custom_t data;
  
  assert(VALID(stream));
  assert(VALID(format));

  data.stream.in = stream;
  data.closure = closure;
  return TrioScan(&data, 0, TrioInStreamCustom, format, &args, NULL);
}

TRIO_PUBLIC int
trio_cscanfv
TRIO_ARGS4((stream, closure, format, args),
	   trio_instream_t stream,
	   trio_pointer_t closure,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  trio_custom_t data;
  
  assert(VALID(stream));
  assert(VALID(format));

  data.stream.in = stream;
  data.closure = closure;
  return TrioScan(&data, 0, TrioInStreamCustom, format, NULL, args);
}

TRIO_PUBLIC int
trio_sscanf
TRIO_VARGS3((buffer, format, va_alist),
	    TRIO_CONST char *buffer,
	    TRIO_CONST char *format,
	    TRIO_VA_DECL)
{
  int status;
  va_list args;

  assert(VALID(buffer));
  assert(VALID(format));
  
  TRIO_VA_START(args, format);
  status = TrioScan((trio_pointer_t)&buffer, 0,
		    TrioInStreamString,
		    format, &args, NULL);
  TRIO_VA_END(args);
  return status;
}

TRIO_PUBLIC int
trio_vsscanf
TRIO_ARGS3((buffer, format, args),
	   TRIO_CONST char *buffer,
	   TRIO_CONST char *format,
	   va_list args)
{
  assert(VALID(buffer));
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)&buffer, 0,
		  TrioInStreamString,
		  format, &args, NULL);
}

TRIO_PUBLIC int
trio_sscanfv
TRIO_ARGS3((buffer, format, args),
	   TRIO_CONST char *buffer,
	   TRIO_CONST char *format,
	   trio_pointer_t *args)
{
  assert(VALID(buffer));
  assert(VALID(format));
  
  return TrioScan((trio_pointer_t)&buffer, 0,
		  TrioInStreamString,
		  format, NULL, args);
}


TRIO_PUBLIC TRIO_CONST char *
trio_strerror
TRIO_ARGS1((errorcode),
	   int errorcode)
{
  
  switch (TRIO_ERROR_CODE(errorcode))
    {
    case TRIO_EOF:
      return "End of file";
    case TRIO_EINVAL:
      return "Invalid argument";
    case TRIO_ETOOMANY:
      return "Too many arguments";
    case TRIO_EDBLREF:
      return "Double reference";
    case TRIO_EGAP:
      return "Reference gap";
    case TRIO_ENOMEM:
      return "Out of memory";
    case TRIO_ERANGE:
      return "Invalid range";
    case TRIO_ECUSTOM:
      return "Custom error";
    default:
      return "Unknown";
    }
}
