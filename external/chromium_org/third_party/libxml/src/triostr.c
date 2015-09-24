/*************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 2001 Bjorn Reese and Daniel Stenberg.
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


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "triodef.h"
#include "triostr.h"


#if !defined(TRIO_STRING_PUBLIC)
# define TRIO_STRING_PUBLIC TRIO_PUBLIC
#endif
#if !defined(TRIO_STRING_PRIVATE)
# define TRIO_STRING_PRIVATE TRIO_PRIVATE
#endif

#if !defined(NULL)
# define NULL 0
#endif
#if !defined(NIL)
# define NIL ((char)0)
#endif
#if !defined(FALSE)
# define FALSE (1 == 0)
# define TRUE (! FALSE)
#endif
#if !defined(BOOLEAN_T)
# define BOOLEAN_T int
#endif

#if defined(TRIO_COMPILER_SUPPORTS_C99)
# define USE_STRTOD
# define USE_STRTOF
#elif defined(TRIO_COMPILER_MSVC)
# define USE_STRTOD
#endif

#if defined(TRIO_PLATFORM_UNIX)
# define USE_STRCASECMP
# define USE_STRNCASECMP
# if defined(TRIO_PLATFORM_SUNOS)
#  define USE_SYS_ERRLIST
# else
#  define USE_STRERROR
# endif
# if defined(TRIO_PLATFORM_QNX)
#  define strcasecmp(x,y) stricmp(x,y)
#  define strncasecmp(x,y,n) strnicmp(x,y,n)
# endif
#elif defined(TRIO_PLATFORM_WIN32)
# define USE_STRCASECMP
# if defined(_WIN32_WCE)
#  define strcasecmp(x,y) _stricmp(x,y)
# else
#  define strcasecmp(x,y) strcmpi(x,y)
# endif
#endif

#if !(defined(TRIO_PLATFORM_SUNOS))
# define USE_TOLOWER
# define USE_TOUPPER
#endif


struct _trio_string_t
{
  char *content;
  size_t length;
  size_t allocated;
};


#if !defined(TRIO_MINIMAL)
static TRIO_CONST char rcsid[] = "@(#)$Id$";
#endif


#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_static.h"
#endif

TRIO_STRING_PUBLIC char *
trio_create
TRIO_ARGS1((size),
	   size_t size)
{
  return (char *)TRIO_MALLOC(size);
}


TRIO_STRING_PUBLIC void
trio_destroy
TRIO_ARGS1((string),
	   char *string)
{
  if (string)
    {
      TRIO_FREE(string);
    }
}


TRIO_STRING_PUBLIC size_t
trio_length
TRIO_ARGS1((string),
	   TRIO_CONST char *string)
{
  return strlen(string);
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_append
TRIO_ARGS2((target, source),
	   char *target,
	   TRIO_CONST char *source)
{
  assert(target);
  assert(source);
  
  return (strcat(target, source) != NULL);
}
#endif 

#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_append_max
TRIO_ARGS3((target, max, source),
	   char *target,
	   size_t max,
	   TRIO_CONST char *source)
{
  size_t length;
  
  assert(target);
  assert(source);

  length = trio_length(target);
  
  if (max > length)
    {
      strncat(target, source, max - length - 1);
    }
  return TRUE;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_contains
TRIO_ARGS2((string, substring),
	   TRIO_CONST char *string,
	   TRIO_CONST char *substring)
{
  assert(string);
  assert(substring);
  
  return (0 != strstr(string, substring));
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_copy
TRIO_ARGS2((target, source),
	   char *target,
	   TRIO_CONST char *source)
{
  assert(target);
  assert(source);
     
  (void)strcpy(target, source);
  return TRUE;
}
#endif 


TRIO_STRING_PUBLIC int
trio_copy_max
TRIO_ARGS3((target, max, source),
	   char *target,
	   size_t max,
	   TRIO_CONST char *source)
{
  assert(target);
  assert(source);
  assert(max > 0); 

  (void)strncpy(target, source, max - 1);
  target[max - 1] = (char)0;
  return TRUE;
}


TRIO_STRING_PRIVATE char *
TrioDuplicateMax
TRIO_ARGS2((source, size),
	   TRIO_CONST char *source,
	   size_t size)
{
  char *target;

  assert(source);

  
  size++;
  target = trio_create(size);
  if (target)
    {
      trio_copy_max(target, size, source);
    }
  return target;
}


TRIO_STRING_PUBLIC char *
trio_duplicate
TRIO_ARGS1((source),
	   TRIO_CONST char *source)
{
  return TrioDuplicateMax(source, trio_length(source));
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_duplicate_max TRIO_ARGS2((source, max),
			      TRIO_CONST char *source,
			      size_t max)
{
  size_t length;

  assert(source);
  assert(max > 0);

  length = trio_length(source);
  if (length > max)
    {
      length = max;
    }
  return TrioDuplicateMax(source, length);
}
#endif 


TRIO_STRING_PUBLIC int
trio_equal
TRIO_ARGS2((first, second),
	   TRIO_CONST char *first,
	   TRIO_CONST char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
#if defined(USE_STRCASECMP)
      return (0 == strcasecmp(first, second));
#else
      while ((*first != NIL) && (*second != NIL))
	{
	  if (trio_to_upper(*first) != trio_to_upper(*second))
	    {
	      break;
	    }
	  first++;
	  second++;
	}
      return ((*first == NIL) && (*second == NIL));
#endif
    }
  return FALSE;
}


TRIO_STRING_PUBLIC int
trio_equal_case
TRIO_ARGS2((first, second),
	   TRIO_CONST char *first,
	   TRIO_CONST char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
      return (0 == strcmp(first, second));
    }
  return FALSE;
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_equal_case_max
TRIO_ARGS3((first, max, second),
	   TRIO_CONST char *first,
	   size_t max,
	   TRIO_CONST char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
      return (0 == strncmp(first, second, max));
    }
  return FALSE;
}
#endif 


TRIO_STRING_PUBLIC int
trio_equal_locale
TRIO_ARGS2((first, second),
	   TRIO_CONST char *first,
	   TRIO_CONST char *second)
{
  assert(first);
  assert(second);

#if defined(LC_COLLATE)
  return (strcoll(first, second) == 0);
#else
  return trio_equal(first, second);
#endif
}


TRIO_STRING_PUBLIC int
trio_equal_max
TRIO_ARGS3((first, max, second),
	   TRIO_CONST char *first,
	   size_t max,
	   TRIO_CONST char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
#if defined(USE_STRNCASECMP)
      return (0 == strncasecmp(first, second, max));
#else
      
      size_t cnt = 0;
      while ((*first != NIL) && (*second != NIL) && (cnt <= max))
	{
	  if (trio_to_upper(*first) != trio_to_upper(*second))
	    {
	      break;
	    }
	  first++;
	  second++;
	  cnt++;
	}
      return ((cnt == max) || ((*first == NIL) && (*second == NIL)));
#endif
    }
  return FALSE;
}


TRIO_STRING_PUBLIC TRIO_CONST char *
trio_error
TRIO_ARGS1((error_number),
	   int error_number)
{
#if defined(USE_STRERROR)
  
  return strerror(error_number);

#elif defined(USE_SYS_ERRLIST)

  extern char *sys_errlist[];
  extern int sys_nerr;

  return ((error_number < 0) || (error_number >= sys_nerr))
    ? "unknown"
    : sys_errlist[error_number];
 
#else
  
  return "unknown";
  
#endif
}


#if !defined(TRIO_MINIMAL) && !defined(_WIN32_WCE)
TRIO_STRING_PUBLIC size_t
trio_format_date_max
TRIO_ARGS4((target, max, format, datetime),
	   char *target,
	   size_t max,
	   TRIO_CONST char *format,
	   TRIO_CONST struct tm *datetime)
{
  assert(target);
  assert(format);
  assert(datetime);
  assert(max > 0);
  
  return strftime(target, max, format, datetime);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC unsigned long
trio_hash
TRIO_ARGS2((string, type),
	   TRIO_CONST char *string,
	   int type)
{
  unsigned long value = 0L;
  char ch;

  assert(string);
  
  switch (type)
    {
    case TRIO_HASH_PLAIN:
      while ( (ch = *string++) != NIL )
	{
	  value *= 31;
	  value += (unsigned long)ch;
	}
      break;
    default:
      assert(FALSE);
      break;
    }
  return value;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_index
TRIO_ARGS2((string, character),
	   TRIO_CONST char *string,
	   int character)
{
  assert(string);

  return strchr(string, character);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_index_last
TRIO_ARGS2((string, character),
	   TRIO_CONST char *string,
	   int character)
{
  assert(string);

  return strchr(string, character);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_lower
TRIO_ARGS1((target),
	   char *target)
{
  assert(target);

  return trio_span_function(target, target, trio_to_lower);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_match
TRIO_ARGS2((string, pattern),
	   TRIO_CONST char *string,
	   TRIO_CONST char *pattern)
{
  assert(string);
  assert(pattern);
  
  for (; ('*' != *pattern); ++pattern, ++string)
    {
      if (NIL == *string)
	{
	  return (NIL == *pattern);
	}
      if ((trio_to_upper((int)*string) != trio_to_upper((int)*pattern))
	  && ('?' != *pattern))
	{
	  return FALSE;
	}
    }
  
  while ('*' == pattern[1])
    pattern++;

  do
    {
      if ( trio_match(string, &pattern[1]) )
	{
	  return TRUE;
	}
    }
  while (*string++);
  
  return FALSE;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_match_case
TRIO_ARGS2((string, pattern),
	   TRIO_CONST char *string,
	   TRIO_CONST char *pattern)
{
  assert(string);
  assert(pattern);
  
  for (; ('*' != *pattern); ++pattern, ++string)
    {
      if (NIL == *string)
	{
	  return (NIL == *pattern);
	}
      if ((*string != *pattern)
	  && ('?' != *pattern))
	{
	  return FALSE;
	}
    }
  
  while ('*' == pattern[1])
    pattern++;

  do
    {
      if ( trio_match_case(string, &pattern[1]) )
	{
	  return TRUE;
	}
    }
  while (*string++);
  
  return FALSE;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC size_t
trio_span_function
TRIO_ARGS3((target, source, Function),
	   char *target,
	   TRIO_CONST char *source,
	   int (*Function) TRIO_PROTO((int)))
{
  size_t count = 0;

  assert(target);
  assert(source);
  assert(Function);
  
  while (*source != NIL)
    {
      *target++ = Function(*source++);
      count++;
    }
  return count;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_substring
TRIO_ARGS2((string, substring),
	   TRIO_CONST char *string,
	   TRIO_CONST char *substring)
{
  assert(string);
  assert(substring);

  return strstr(string, substring);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_substring_max
TRIO_ARGS3((string, max, substring),
	   TRIO_CONST char *string,
	   size_t max,
	   TRIO_CONST char *substring)
{
  size_t count;
  size_t size;
  char *result = NULL;

  assert(string);
  assert(substring);
  
  size = trio_length(substring);
  if (size <= max)
    {
      for (count = 0; count <= max - size; count++)
	{
	  if (trio_equal_max(substring, size, &string[count]))
	    {
	      result = (char *)&string[count];
	      break;
	    }
	}
    }
  return result;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_tokenize
TRIO_ARGS2((string, delimiters),
	   char *string,
	   TRIO_CONST char *delimiters)
{
  assert(delimiters);
  
  return strtok(string, delimiters);
}
#endif 


TRIO_STRING_PUBLIC trio_long_double_t
trio_to_long_double
TRIO_ARGS2((source, endp),
	   TRIO_CONST char *source,
	   char **endp)
{
#if defined(USE_STRTOLD)
  return strtold(source, endp);
#else
  int isNegative = FALSE;
  int isExponentNegative = FALSE;
  trio_long_double_t integer = 0.0;
  trio_long_double_t fraction = 0.0;
  unsigned long exponent = 0;
  trio_long_double_t base;
  trio_long_double_t fracdiv = 1.0;
  trio_long_double_t value = 0.0;

  
  if ((source[0] == '0') && ((source[1] == 'x') || (source[1] == 'X')))
    {
      base = 16.0;
      source += 2;
      while (isxdigit((int)*source))
	{
	  integer *= base;
	  integer += (isdigit((int)*source)
		      ? (*source - '0')
		      : 10 + (trio_to_upper((int)*source) - 'A'));
	  source++;
	}
      if (*source == '.')
	{
	  source++;
	  while (isxdigit((int)*source))
	    {
	      fracdiv /= base;
	      fraction += fracdiv * (isdigit((int)*source)
				     ? (*source - '0')
				     : 10 + (trio_to_upper((int)*source) - 'A'));
	      source++;
	    }
	  if ((*source == 'p') || (*source == 'P'))
	    {
	      source++;
	      if ((*source == '+') || (*source == '-'))
		{
		  isExponentNegative = (*source == '-');
		  source++;
		}
	      while (isdigit((int)*source))
		{
		  exponent *= 10;
		  exponent += (*source - '0');
		  source++;
		}
	    }
	}
      
      base = 2.0;
    }
  else 
    {
      base = 10.0;
      isNegative = (*source == '-');
      
      if ((*source == '+') || (*source == '-'))
	source++;

      
      while (isdigit((int)*source))
	{
	  integer *= base;
	  integer += (*source - '0');
	  source++;
	}

      if (*source == '.')
	{
	  source++; 
	  while (isdigit((int)*source))
	    {
	      fracdiv /= base;
	      fraction += (*source - '0') * fracdiv;
	      source++;
	    }
	}
      if ((*source == 'e')
	  || (*source == 'E')
#if TRIO_MICROSOFT
	  || (*source == 'd')
	  || (*source == 'D')
#endif
	  )
	{
	  source++; 
	  isExponentNegative = (*source == '-');
	  if ((*source == '+') || (*source == '-'))
	    source++;
	  while (isdigit((int)*source))
	    {
	      exponent *= (int)base;
	      exponent += (*source - '0');
	      source++;
	    }
	}
    }
  
  value = integer + fraction;
  if (exponent != 0)
    {
      if (isExponentNegative)
	value /= pow(base, (double)exponent);
      else
	value *= pow(base, (double)exponent);
    }
  if (isNegative)
    value = -value;

  if (endp)
    *endp = (char *)source;
  return value;
#endif
}


TRIO_STRING_PUBLIC double
trio_to_double
TRIO_ARGS2((source, endp),
	   TRIO_CONST char *source,
	   char **endp)
{
#if defined(USE_STRTOD)
  return strtod(source, endp);
#else
  return (double)trio_to_long_double(source, endp);
#endif
}

#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC float
trio_to_float
TRIO_ARGS2((source, endp),
	   TRIO_CONST char *source,
	   char **endp)
{
#if defined(USE_STRTOF)
  return strtof(source, endp);
#else
  return (float)trio_to_long_double(source, endp);
#endif
}
#endif 


TRIO_STRING_PUBLIC long
trio_to_long
TRIO_ARGS3((string, endp, base),
	   TRIO_CONST char *string,
	   char **endp,
	   int base)
{
  assert(string);
  assert((base >= 2) && (base <= 36));
  
  return strtol(string, endp, base);
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_to_lower
TRIO_ARGS1((source),
	   int source)
{
#if defined(USE_TOLOWER)
  
  return tolower(source);
  
#else

  
  return ((source >= (int)'A') && (source <= (int)'Z'))
    ? source - 'A' + 'a'
    : source;
  
#endif
}
#endif 

#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC unsigned long
trio_to_unsigned_long
TRIO_ARGS3((string, endp, base),
	   TRIO_CONST char *string,
	   char **endp,
	   int base)
{
  assert(string);
  assert((base >= 2) && (base <= 36));
  
  return strtoul(string, endp, base);
}
#endif 


TRIO_STRING_PUBLIC int
trio_to_upper
TRIO_ARGS1((source),
	   int source)
{
#if defined(USE_TOUPPER)
  
  return toupper(source);
  
#else

  
  return ((source >= (int)'a') && (source <= (int)'z'))
    ? source - 'a' + 'A'
    : source;
  
#endif
}

#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_upper
TRIO_ARGS1((target),
	   char *target)
{
  assert(target);

  return trio_span_function(target, target, trio_to_upper);
}
#endif 





#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_dynamic.h"
#endif

TRIO_STRING_PRIVATE trio_string_t *
TrioStringAlloc(TRIO_NOARGS)
{
  trio_string_t *self;
  
  self = (trio_string_t *)TRIO_MALLOC(sizeof(trio_string_t));
  if (self)
    {
      self->content = NULL;
      self->length = 0;
      self->allocated = 0;
    }
  return self;
}


TRIO_STRING_PRIVATE BOOLEAN_T
TrioStringGrow
TRIO_ARGS2((self, delta),
	   trio_string_t *self,
	   size_t delta)
{
  BOOLEAN_T status = FALSE;
  char *new_content;
  size_t new_size;

  new_size = (delta == 0)
    ? ( (self->allocated == 0) ? 1 : self->allocated * 2 )
    : self->allocated + delta;
  
  new_content = (char *)TRIO_REALLOC(self->content, new_size);
  if (new_content)
    {
      self->content = new_content;
      self->allocated = new_size;
      status = TRUE;
    }
  return status;
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PRIVATE BOOLEAN_T
TrioStringGrowTo
TRIO_ARGS2((self, length),
	   trio_string_t *self,
	   size_t length)
{
  length++; 
  return (self->allocated < length)
    ? TrioStringGrow(self, length - self->allocated)
    : TRUE;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC trio_string_t *
trio_string_create
TRIO_ARGS1((initial_size),
	   int initial_size)
{
  trio_string_t *self;

  self = TrioStringAlloc();
  if (self)
    {
      if (TrioStringGrow(self,
			 (size_t)((initial_size > 0) ? initial_size : 1)))
	{
	  self->content[0] = (char)0;
	  self->allocated = initial_size;
	}
      else
	{
	  trio_string_destroy(self);
	  self = NULL;
	}
    }
  return self;
}
#endif 


TRIO_STRING_PUBLIC void
trio_string_destroy
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  assert(self);
  
  if (self)
    {
      trio_destroy(self->content);
      TRIO_FREE(self);
    }
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_string_get
TRIO_ARGS2((self, offset),
	   trio_string_t *self,
	   int offset)
{
  char *result = NULL;
  
  assert(self);

  if (self->content != NULL)
    {
      if (self->length == 0)
	{
	  (void)trio_string_length(self);
	}
      if (offset >= 0)
	{
	  if (offset > (int)self->length)
	    {
	      offset = self->length;
	    }
	}
      else
	{
	  offset += self->length + 1;
	  if (offset < 0)
	    {
	      offset = 0;
	    }
	}
      result = &(self->content[offset]);
    }
  return result;
}
#endif 


TRIO_STRING_PUBLIC char *
trio_string_extract
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  char *result;
  
  assert(self);

  result = self->content;
  
  self->content = NULL;
  self->length = self->allocated = 0;
  return result;
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC void
trio_xstring_set
TRIO_ARGS2((self, buffer),
	   trio_string_t *self,
	   char *buffer)
{
  assert(self);

  trio_destroy(self->content);
  self->content = trio_duplicate(buffer);
}
#endif 


TRIO_STRING_PUBLIC int
trio_string_size
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  assert(self);

  return self->allocated;
}


TRIO_STRING_PUBLIC void
trio_string_terminate
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  trio_xstring_append_char(self, 0);
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_append
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  size_t length;
  
  assert(self);
  assert(other);

  length = self->length + other->length;
  if (!TrioStringGrowTo(self, length))
    goto error;
  trio_copy(&self->content[self->length], other->content);
  self->length = length;
  return TRUE;
  
 error:
  return FALSE;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_append
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  size_t length;
  
  assert(self);
  assert(other);

  length = self->length + trio_length(other);
  if (!TrioStringGrowTo(self, length))
    goto error;
  trio_copy(&self->content[self->length], other);
  self->length = length;
  return TRUE;
  
 error:
  return FALSE;
}
#endif 


TRIO_STRING_PUBLIC int
trio_xstring_append_char
TRIO_ARGS2((self, character),
	   trio_string_t *self,
	   char character)
{
  assert(self);

  if ((int)self->length >= trio_string_size(self))
    {
      if (!TrioStringGrow(self, 0))
	goto error;
    }
  self->content[self->length] = character;
  self->length++;
  return TRUE;
  
 error:
  return FALSE;
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_contains
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_contains(self->content, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_contains
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_contains(self->content, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_copy
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  self->length = 0;
  return trio_string_append(self, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_copy
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  self->length = 0;
  return trio_xstring_append(self, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC trio_string_t *
trio_string_duplicate
TRIO_ARGS1((other),
	   trio_string_t *other)
{
  trio_string_t *self;
  
  assert(other);

  self = TrioStringAlloc();
  if (self)
    {
      self->content = TrioDuplicateMax(other->content, other->length);
      if (self->content)
	{
	  self->length = other->length;
	  self->allocated = self->length + 1;
	}
      else
	{
	  self->length = self->allocated = 0;
	}
    }
  return self;
}
#endif 


TRIO_STRING_PUBLIC trio_string_t *
trio_xstring_duplicate
TRIO_ARGS1((other),
	   TRIO_CONST char *other)
{
  trio_string_t *self;
  
  assert(other);

  self = TrioStringAlloc();
  if (self)
    {
      self->content = TrioDuplicateMax(other, trio_length(other));
      if (self->content)
	{
	  self->length = trio_length(self->content);
	  self->allocated = self->length + 1;
	}
      else
	{
	  self->length = self->allocated = 0;
	}
    }
  return self;
}


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_equal
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal(self->content, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_equal
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_equal(self->content, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_equal_max
TRIO_ARGS3((self, max, other),
	   trio_string_t *self,
	   size_t max,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_max(self->content, max, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_equal_max
TRIO_ARGS3((self, max, other),
	   trio_string_t *self,
	   size_t max,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_equal_max(self->content, max, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_equal_case
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_case(self->content, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_equal_case
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_equal_case(self->content, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_equal_case_max
TRIO_ARGS3((self, max, other),
	   trio_string_t *self,
	   size_t max,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_case_max(self->content, max, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_equal_case_max
TRIO_ARGS3((self, max, other),
	   trio_string_t *self,
	   size_t max,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_equal_case_max(self->content, max, other);
}
#endif 


#if !defined(TRIO_MINIMAL) && !defined(_WIN32_WCE)
TRIO_STRING_PUBLIC size_t
trio_string_format_date_max
TRIO_ARGS4((self, max, format, datetime),
	   trio_string_t *self,
	   size_t max,
	   TRIO_CONST char *format,
	   TRIO_CONST struct tm *datetime)
{
  assert(self);

  return trio_format_date_max(self->content, max, format, datetime);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_string_index
TRIO_ARGS2((self, character),
	   trio_string_t *self,
	   int character)
{
  assert(self);

  return trio_index(self->content, character);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_string_index_last
TRIO_ARGS2((self, character),
	   trio_string_t *self,
	   int character)
{
  assert(self);

  return trio_index_last(self->content, character);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_length
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  assert(self);

  if (self->length == 0)
    {
      self->length = trio_length(self->content);
    }
  return self->length;
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_lower
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  assert(self);

  return trio_lower(self->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_match
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_match(self->content, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_match
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_match(self->content, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_match_case
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_match_case(self->content, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_xstring_match_case
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_match_case(self->content, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_string_substring
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_substring(self->content, other->content);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC char *
trio_xstring_substring
TRIO_ARGS2((self, other),
	   trio_string_t *self,
	   TRIO_CONST char *other)
{
  assert(self);
  assert(other);

  return trio_substring(self->content, other);
}
#endif 


#if !defined(TRIO_MINIMAL)
TRIO_STRING_PUBLIC int
trio_string_upper
TRIO_ARGS1((self),
	   trio_string_t *self)
{
  assert(self);

  return trio_upper(self->content);
}
#endif 

