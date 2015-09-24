

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2012 Julian Seward
      jseward@acm.org

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#ifndef __PUB_TOOL_LIBCBASE_H
#define __PUB_TOOL_LIBCBASE_H


extern Bool VG_(isspace) ( Char c );
extern Bool VG_(isdigit) ( Char c );
extern Char VG_(tolower) ( Char c );


extern Long  VG_(strtoll10) ( Char* str, Char** endptr );
extern Long  VG_(strtoll16) ( Char* str, Char** endptr );
extern ULong  VG_(strtoull10) ( Char* str, Char** endptr );
extern ULong  VG_(strtoull16) ( Char* str, Char** endptr );

extern double VG_(strtod)  ( Char* str, Char** endptr );


#define VG_STREQ(s1,s2) ( (s1 != NULL && s2 != NULL \
                           && VG_(strcmp)((s1),(s2))==0) ? True : False )
#define VG_STREQN(n,s1,s2) ( (s1 != NULL && s2 != NULL \
                             && VG_(strncmp)((s1),(s2),(n))==0) ? True : False )

extern SizeT VG_(strlen)         ( const Char* str );
extern Char* VG_(strcat)         ( Char* dest, const Char* src );
extern Char* VG_(strncat)        ( Char* dest, const Char* src, SizeT n );
extern Char* VG_(strpbrk)        ( const Char* s, const Char* accpt );
extern Char* VG_(strcpy)         ( Char* dest, const Char* src );
extern Char* VG_(strncpy)        ( Char* dest, const Char* src, SizeT ndest );
extern Int   VG_(strcmp)         ( const Char* s1, const Char* s2 );
extern Int   VG_(strcasecmp)     ( const Char* s1, const Char* s2 );
extern Int   VG_(strncmp)        ( const Char* s1, const Char* s2, SizeT nmax );
extern Int   VG_(strncasecmp)    ( const Char* s1, const Char* s2, SizeT nmax );
extern Char* VG_(strstr)         ( const Char* haystack, Char* needle );
extern Char* VG_(strcasestr)     ( const Char* haystack, Char* needle );
extern Char* VG_(strchr)         ( const Char* s, Char c );
extern Char* VG_(strrchr)        ( const Char* s, Char c );
extern SizeT VG_(strspn)         ( const Char* s, const Char* accpt );
extern SizeT VG_(strcspn)        ( const Char* s, const char* reject );

extern Char* VG_(strtok_r)       (Char* s, const Char* delim, Char** saveptr);
extern Char* VG_(strtok)         (Char* s, const Char* delim);

extern Bool VG_(parse_Addr) ( UChar** ppc, Addr* result );

extern void  VG_(strncpy_safely) ( Char* dest, const Char* src, SizeT ndest );


extern void* VG_(memcpy) ( void *d, const void *s, SizeT sz );
extern void* VG_(memmove)( void *d, const void *s, SizeT sz );
extern void* VG_(memset) ( void *s, Int c, SizeT sz );
extern Int   VG_(memcmp) ( const void* s1, const void* s2, SizeT n );

inline __attribute__((always_inline))
static void VG_(bzero_inline) ( void* s, SizeT sz )
{
   if (LIKELY(0 == (((Addr)sz) & (Addr)(sizeof(UWord)-1)))
       && LIKELY(0 == (((Addr)s) & (Addr)(sizeof(UWord)-1)))) {
      UWord* p = (UWord*)s;
      switch (sz / (SizeT)sizeof(UWord)) {
          case 8: p[0] = p[1] = p[2] = p[3]
                  = p[4] = p[5] = p[6] = p[7] = 0UL; return;
          case 7: p[0] = p[1] = p[2] = p[3]
                  = p[4] = p[5] = p[6] = 0UL; return;
          case 6: p[0] = p[1] = p[2] = p[3]
                  = p[4] = p[5] = 0UL; return;
          case 5: p[0] = p[1] = p[2] = p[3] = p[4] = 0UL; return;
          case 4: p[0] = p[1] = p[2] = p[3] = 0UL; return;
          case 3: p[0] = p[1] = p[2] = 0UL; return;
          case 2: p[0] = p[1] = 0UL; return;
          case 1: p[0] = 0UL; return;
          case 0: return;
          default: break;
      }
   }
   VG_(memset)(s, 0, sz);
}



#define VG_IS_2_ALIGNED(aaa_p)    (0 == (((Addr)(aaa_p)) & ((Addr)0x1)))
#define VG_IS_4_ALIGNED(aaa_p)    (0 == (((Addr)(aaa_p)) & ((Addr)0x3)))
#define VG_IS_8_ALIGNED(aaa_p)    (0 == (((Addr)(aaa_p)) & ((Addr)0x7)))
#define VG_IS_16_ALIGNED(aaa_p)   (0 == (((Addr)(aaa_p)) & ((Addr)0xf)))
#define VG_IS_32_ALIGNED(aaa_p)   (0 == (((Addr)(aaa_p)) & ((Addr)0x1f)))
#define VG_IS_WORD_ALIGNED(aaa_p) (0 == (((Addr)(aaa_p)) & ((Addr)(sizeof(Addr)-1))))
#define VG_IS_PAGE_ALIGNED(aaa_p) (0 == (((Addr)(aaa_p)) & ((Addr)(VKI_PAGE_SIZE-1))))

#define VG_ROUNDDN(p, a)   ((Addr)(p) & ~((Addr)(a)-1))
#define VG_ROUNDUP(p, a)   VG_ROUNDDN((p)+(a)-1, (a))
#define VG_PGROUNDDN(p)    VG_ROUNDDN(p, VKI_PAGE_SIZE)
#define VG_PGROUNDUP(p)    VG_ROUNDUP(p, VKI_PAGE_SIZE)


extern void VG_(ssort)( void* base, SizeT nmemb, SizeT size,
                        Int (*compar)(void*, void*) );

extern Int VG_(log2) ( UInt x );

extern Int VG_(log2_64)( ULong x );

extern UInt VG_(random) ( UInt* pSeed );
#define VG_RAND_MAX (1ULL << 32)

#endif   

