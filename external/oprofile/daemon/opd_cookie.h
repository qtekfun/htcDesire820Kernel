/**
 * @file opd_cookie.h
 * cookie -> name cache
 *
 * @remark Copyright 2002, 2005 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef OPD_COOKIE_H
#define OPD_COOKIE_H

typedef unsigned long long cookie_t;

#define INVALID_COOKIE ~0LLU
#define NO_COOKIE 0LLU

#define DCOOKIE_SHIFT 7

char const * find_cookie(cookie_t cookie);

int is_cookie_ignored(cookie_t cookie);

char const * verbose_cookie(cookie_t cookie);

void cookie_init(void);

#endif 
