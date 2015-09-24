

/*
   This file is part of Cachegrind, a Valgrind tool for cache
   profiling programs.

   Copyright (C) 2002-2012 Nicholas Nethercote
      njn@valgrind.org

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


typedef struct {
   Int          size;                   
   Int          assoc;
   Int          line_size;              
   Int          sets;
   Int          sets_min_1;
   Int          line_size_bits;
   Int          tag_shift;
   Char         desc_line[128];
   UWord*       tags;
} cache_t2;

static void cachesim_initcache(cache_t config, cache_t2* c)
{
   Int i;

   c->size      = config.size;
   c->assoc     = config.assoc;
   c->line_size = config.line_size;

   c->sets           = (c->size / c->line_size) / c->assoc;
   c->sets_min_1     = c->sets - 1;
   c->line_size_bits = VG_(log2)(c->line_size);
   c->tag_shift      = c->line_size_bits + VG_(log2)(c->sets);

   if (c->assoc == 1) {
      VG_(sprintf)(c->desc_line, "%d B, %d B, direct-mapped", 
                                 c->size, c->line_size);
   } else {
      VG_(sprintf)(c->desc_line, "%d B, %d B, %d-way associative",
                                 c->size, c->line_size, c->assoc);
   }

   c->tags = VG_(malloc)("cg.sim.ci.1",
                         sizeof(UWord) * c->sets * c->assoc);

   for (i = 0; i < c->sets * c->assoc; i++)
      c->tags[i] = 0;
}


#define CACHESIM(L, MISS_TREATMENT)                                         \
                             \
static cache_t2 L;                                                          \
                                                                            \
static void cachesim_##L##_initcache(cache_t config)                        \
{                                                                           \
    cachesim_initcache(config, &L);                                         \
}                                                                           \
                                                                            \
   \
   \
__attribute__((always_inline))                                              \
static __inline__                                                           \
void cachesim_##L##_doref(Addr a, UChar size, ULong* m1, ULong *mL)         \
{                                                                           \
   UInt  set1 = ( a         >> L.line_size_bits) & (L.sets_min_1);          \
   UInt  set2 = ((a+size-1) >> L.line_size_bits) & (L.sets_min_1);          \
   UWord tag  = a >> L.tag_shift;                                           \
   UWord tag2;                                                              \
   Int i, j;                                                                \
   Bool is_miss = False;                                                    \
   UWord* set;                                                              \
                                                                            \
                                \
   if (set1 == set2) {                                                      \
                                                                            \
      set = &(L.tags[set1 * L.assoc]);                                      \
                                                                            \
      \
      \
      \
      if (tag == set[0]) {                                                  \
         return;                                                            \
      }                                                                     \
      \
      \
      for (i = 1; i < L.assoc; i++) {                                       \
         if (tag == set[i]) {                                               \
            for (j = i; j > 0; j--) {                                       \
               set[j] = set[j - 1];                                         \
            }                                                               \
            set[0] = tag;                                                   \
            return;                                                         \
         }                                                                  \
      }                                                                     \
                                                                            \
                  \
      for (j = L.assoc - 1; j > 0; j--) {                                   \
         set[j] = set[j - 1];                                               \
      }                                                                     \
      set[0] = tag;                                                         \
      MISS_TREATMENT;                                                       \
      return;                                                               \
                                                                            \
                                \
                   \
   } else if (((set1 + 1) & (L.sets_min_1)) == set2) {                      \
      set = &(L.tags[set1 * L.assoc]);                                      \
      if (tag == set[0]) {                                                  \
         goto block2;                                                       \
      }                                                                     \
      for (i = 1; i < L.assoc; i++) {                                       \
         if (tag == set[i]) {                                               \
            for (j = i; j > 0; j--) {                                       \
               set[j] = set[j - 1];                                         \
            }                                                               \
            set[0] = tag;                                                   \
            goto block2;                                                    \
         }                                                                  \
      }                                                                     \
      for (j = L.assoc - 1; j > 0; j--) {                                   \
         set[j] = set[j - 1];                                               \
      }                                                                     \
      set[0] = tag;                                                         \
      is_miss = True;                                                       \
block2:                                                                     \
      set = &(L.tags[set2 * L.assoc]);                                      \
      tag2 = (a+size-1) >> L.tag_shift;                                     \
      if (tag2 == set[0]) {                                                 \
         goto miss_treatment;                                               \
      }                                                                     \
      for (i = 1; i < L.assoc; i++) {                                       \
         if (tag2 == set[i]) {                                              \
            for (j = i; j > 0; j--) {                                       \
               set[j] = set[j - 1];                                         \
            }                                                               \
            set[0] = tag2;                                                  \
            goto miss_treatment;                                            \
         }                                                                  \
      }                                                                     \
      for (j = L.assoc - 1; j > 0; j--) {                                   \
         set[j] = set[j - 1];                                               \
      }                                                                     \
      set[0] = tag2;                                                        \
      is_miss = True;                                                       \
miss_treatment:                                                             \
      if (is_miss) { MISS_TREATMENT; }                                      \
                                                                            \
   } else {                                                                 \
       VG_(printf)("addr: %lx  size: %u  sets: %d %d", a, size, set1, set2);\
       VG_(tool_panic)("item straddles more than two cache sets");          \
   }                                                                        \
   return;                                                                  \
}

CACHESIM(LL, (*mL)++ );
CACHESIM(I1, { (*m1)++; cachesim_LL_doref(a, size, m1, mL); } );
CACHESIM(D1, { (*m1)++; cachesim_LL_doref(a, size, m1, mL); } );


