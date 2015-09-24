// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef HEAP_PROFILE_STATS_H_
#define HEAP_PROFILE_STATS_H_

struct HeapProfileStats {
  
  bool Equivalent(const HeapProfileStats& other) const {
    return allocs - frees == other.allocs - other.frees &&
        alloc_size - free_size == other.alloc_size - other.free_size;
  }

  int32 allocs;      
  int32 frees;       
  int64 alloc_size;  
  int64 free_size;   
};

struct HeapProfileBucket : public HeapProfileStats {
  
  static const int kMaxStackDepth = 32;

  uintptr_t hash;           
  int depth;                
  const void** stack;       
  HeapProfileBucket* next;  
};

#endif  
