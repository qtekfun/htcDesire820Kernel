/*
 * The authors of this software are Rob Pike and Ken Thompson.
 *              Copyright (c) 2002 by Lucent Technologies.
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR LUCENT TECHNOLOGIES MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * This file and rune.cc have been converted to compile as C++ code
 * in name space re2.
 */
#ifndef RE2_UTIL_UTF_H__
#define RE2_UTIL_UTF_H__

#include <stdint.h>

namespace re2 {

typedef signed int Rune;	

enum
{
  UTFmax	= 4,		
  Runesync	= 0x80,		
  Runeself	= 0x80,		
  Runeerror	= 0xFFFD,	
  Runemax	= 0x10FFFF,	
};

int runetochar(char* s, const Rune* r);
int chartorune(Rune* r, const char* s);
int fullrune(const char* s, int n);
int utflen(const char* s);
char* utfrune(const char*, Rune);

}  

#endif  
