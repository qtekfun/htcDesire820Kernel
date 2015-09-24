// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_COMMON_H_
#define TCMALLOC_COMMON_H_

#include "config.h"
#include <stddef.h>                     
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include "internal_logging.h"  

typedef uintptr_t PageID;

typedef uintptr_t Length;



#if defined(TCMALLOC_LARGE_PAGES)
static const size_t kPageShift  = 15;
static const size_t kNumClasses = 78;
#else
static const size_t kPageShift  = 13;
static const size_t kNumClasses = 86;
#endif
static const size_t kMaxThreadCacheSize = 4 << 20;

static const size_t kPageSize   = 1 << kPageShift;
static const size_t kMaxSize    = 256 * 1024;
static const size_t kAlignment  = 8;
static const size_t kLargeSizeClass = 0;
static const size_t kMaxPages = 1 << (20 - kPageShift);

#ifdef TCMALLOC_SMALL_BUT_SLOW
static const size_t kDefaultOverallThreadCacheSize = kMaxThreadCacheSize;
#else
static const size_t kDefaultOverallThreadCacheSize = 8u * kMaxThreadCacheSize;
#endif

static const size_t kMinThreadCacheSize = kMaxSize * 2;

static const size_t kStealAmount = 1 << 16;

static const int kMaxOverages = 3;

static const int kMaxDynamicFreeListLength = 8192;

static const Length kMaxValidPages = (~static_cast<Length>(0)) >> kPageShift;

#if defined __x86_64__
static const int kAddressBits = (sizeof(void*) < 8 ? (8 * sizeof(void*)) : 48);
#else
static const int kAddressBits = 8 * sizeof(void*);
#endif

namespace tcmalloc {

inline Length pages(size_t bytes) {
  return (bytes >> kPageShift) +
      ((bytes & (kPageSize - 1)) > 0 ? 1 : 0);
}

int AlignmentForSize(size_t size);

class SizeMap {
 private:
  
  
  
  
  
  int num_objects_to_move_[kNumClasses];

  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const int kMaxSmallSize = 1024;
  static const size_t kClassArraySize =
      ((kMaxSize + 127 + (120 << 7)) >> 7) + 1;
  unsigned char class_array_[kClassArraySize];

  
  static inline int ClassIndex(int s) {
    ASSERT(0 <= s);
    ASSERT(s <= kMaxSize);
    const bool big = (s > kMaxSmallSize);
    const int add_amount = big ? (127 + (120<<7)) : 7;
    const int shift_amount = big ? 7 : 3;
    return (s + add_amount) >> shift_amount;
  }

  int NumMoveSize(size_t size);

  
  size_t class_to_size_[kNumClasses];

  
  size_t class_to_pages_[kNumClasses];

 public:
  
  
  SizeMap() { }

  
  void Init();

  inline int SizeClass(int size) {
    return class_array_[ClassIndex(size)];
  }

  
  inline size_t ByteSizeForClass(size_t cl) {
    return class_to_size_[cl];
  }

  
  inline size_t class_to_size(size_t cl) {
    return class_to_size_[cl];
  }

  
  inline size_t class_to_pages(size_t cl) {
    return class_to_pages_[cl];
  }

  
  
  
  
  
  inline int num_objects_to_move(size_t cl) {
    return num_objects_to_move_[cl];
  }
};

void* MetaDataAlloc(size_t bytes);

uint64_t metadata_system_bytes();

static const int kMaxStackDepth = 31;
struct StackTrace {
  uintptr_t size;          
  uintptr_t depth;         
  void*     stack[kMaxStackDepth];
};

}  

#endif  
