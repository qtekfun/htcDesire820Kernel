/* vsprintf with automatic memory allocation.
   Copyright (C) 2002-2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  */

#ifndef _VASNPRINTF_H
#define _VASNPRINTF_H

#include <stdarg.h>

#include <stddef.h>

#ifndef __attribute__
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#  define __attribute__(Spec) 
# endif
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#  define __format__ format
#  define __printf__ printf
# endif
#endif

#ifdef	__cplusplus
extern "C" {
#endif

extern char * asnprintf (char *resultbuf, size_t *lengthp, const char *format, ...)
       __attribute__ ((__format__ (__printf__, 3, 4)));
extern char * vasnprintf (char *resultbuf, size_t *lengthp, const char *format, va_list args)
       __attribute__ ((__format__ (__printf__, 3, 0)));

#ifdef	__cplusplus
}
#endif

#endif 
