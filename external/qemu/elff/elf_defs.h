/* Copyright (C) 2007-2010 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/


#ifndef ELFF_ELF_DEFS_H_
#define ELFF_ELF_DEFS_H_

#include "elff_elf.h"


#define INC_PTR(p, n)   (reinterpret_cast<uint8_t*>(p) + (n))

#define INC_CPTR(p, n)  (reinterpret_cast<const uint8_t*>(p) + (n))

#define INC_PTR_T(T, p, n)                              \
    reinterpret_cast<T*>                                \
        (reinterpret_cast<uint8_t*>(p) + (n))

#define INC_CPTR_T(T, p, n)                                 \
    reinterpret_cast<const T*>                              \
        (reinterpret_cast<const uint8_t*>(p) + (n))

#define ELFF_ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))

#define ELFF_FIELD_OFFSET(T, f) ((size_t)(size_t*)&(((T *)0)->f))


static inline size_t
diff_ptr(const void* s, const void* e) {
  assert(s <= e);
  return ((size_t)(reinterpret_cast<const uint8_t*>(e) -
         reinterpret_cast<const uint8_t*>(s)));
}

static inline uint8_t
get_byte(const void* ptr, uint32_t bt) {
  return *(reinterpret_cast<const uint8_t*>(ptr) + bt);
}

static inline bool
is_in_section(const void* rp, size_t rsize, const void* ss, size_t ssize) {
  const void* rend = INC_CPTR(rp, rsize);
  
  return rp >= ss && ss != NULL && (diff_ptr(ss, rend) <= ssize) && rend >= rp;
}

static inline bool
is_little_endian_cpu(void) {
  uint16_t tmp = 0x00FF;
  
  return get_byte(&tmp, 0) == 0xFF;
}

#ifdef _WIN32
#  define FMT_I64  "I64"
#else
#  define FMT_I64  "ll"
#endif

#endif  
