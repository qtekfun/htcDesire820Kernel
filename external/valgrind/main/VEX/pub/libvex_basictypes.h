

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2004-2012 OpenWorks LLP
      info@open-works.net

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   The GNU General Public License is contained in the file COPYING.

   Neither the names of the U.S. Department of Energy nor the
   University of California nor the names of its contributors may be
   used to endorse or promote products derived from this software
   without prior written permission.
*/

#ifndef __LIBVEX_BASICTYPES_H
#define __LIBVEX_BASICTYPES_H


typedef  unsigned char   UChar;
typedef    signed char   Char;
typedef           char   HChar; 

typedef  unsigned short  UShort;
typedef    signed short  Short;

typedef  unsigned int    UInt;
typedef    signed int    Int;

typedef  unsigned long long int   ULong;
typedef    signed long long int   Long;

typedef  UInt  U128[4];

typedef  UInt  U256[8];

typedef
   union {
      UChar  w8[16];
      UShort w16[8];
      UInt   w32[4];
      ULong  w64[2];
   }
   V128;

typedef  float   Float;    
typedef  double  Double;   

typedef  unsigned char  Bool;
#define  True   ((Bool)1)
#define  False  ((Bool)0)

static inline Bool toBool ( Int x ) {
   Int r = (x == 0) ? False : True;
   return (Bool)r;
}
static inline UChar toUChar ( Int x ) {
   x &= 0xFF;
   return (UChar)x;
}
static inline HChar toHChar ( Int x ) {
   x &= 0xFF;
   return (HChar)x;
}
static inline UShort toUShort ( Int x ) {
   x &= 0xFFFF;
   return (UShort)x;
}
static inline Short toShort ( Int x ) {
   x &= 0xFFFF;
   return (Short)x;
}
static inline UInt toUInt ( Long x ) {
   x &= 0xFFFFFFFFLL;
   return (UInt)x;
}

typedef  UInt      Addr32;
typedef  ULong     Addr64;

typedef  unsigned long HWord;


#if !defined(offsetof)
#   define offsetof(type,memb) ((Int)(HWord)&((type*)0)->memb)
#endif



#undef VEX_HOST_WORDSIZE
#undef VEX_REGPARM

#if defined(__x86_64__)
#   define VEX_HOST_WORDSIZE 8
#   define VEX_REGPARM(_n) 

#elif defined(__i386__)
#   define VEX_HOST_WORDSIZE 4
#   define VEX_REGPARM(_n) __attribute__((regparm(_n)))

#elif defined(__powerpc__) && defined(__powerpc64__)
#   define VEX_HOST_WORDSIZE 8
#   define VEX_REGPARM(_n) 

#elif defined(__powerpc__) && !defined(__powerpc64__)
#   define VEX_HOST_WORDSIZE 4
#   define VEX_REGPARM(_n) 

#elif defined(__arm__)
#   define VEX_HOST_WORDSIZE 4
#   define VEX_REGPARM(_n) 

#elif defined(_AIX) && !defined(__64BIT__)
#   define VEX_HOST_WORDSIZE 4
#   define VEX_REGPARM(_n) 

#elif defined(_AIX) && defined(__64BIT__)
#   define VEX_HOST_WORDSIZE 8
#   define VEX_REGPARM(_n) 

#elif defined(__s390x__)
#   define VEX_HOST_WORDSIZE 8
#   define VEX_REGPARM(_n) 

#elif defined(__mips__)
#   define VEX_HOST_WORDSIZE 4
#   define VEX_REGPARM(_n) 

#else
#   error "Vex: Fatal: Can't establish the host architecture"
#endif


#if VEX_HOST_WORDSIZE == 8
   static inline ULong Ptr_to_ULong ( void* p ) {
      return (ULong)p;
   }
   static inline void* ULong_to_Ptr ( ULong n ) {
      return (void*)n;
   }
#elif VEX_HOST_WORDSIZE == 4
   static inline ULong Ptr_to_ULong ( void* p ) {
      UInt w = (UInt)p;
      return (ULong)w;
   }
   static inline void* ULong_to_Ptr ( ULong n ) {
      UInt w = (UInt)n;
      return (void*)w;
   }
#else
#   error "Vex: Fatal: Can't define  Ptr_to_ULong / ULong_to_Ptr"
#endif


#endif 

