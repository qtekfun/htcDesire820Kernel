// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_BITMAP_H_
#define NET_DISK_CACHE_BITMAP_H_
#pragma once

#include "base/basictypes.h"

namespace disk_cache {

class Bitmap {
 public:
  Bitmap() : map_(NULL), num_bits_(0), array_size_(0), alloc_(false) {}

  
  
  Bitmap(int num_bits, bool clear_bits);

  
  
  
  Bitmap(uint32* map, int num_bits, int num_words);

  ~Bitmap();

  
  
  
  
  
  void Resize(int num_bits, bool clear_bits);

  
  int Size() const { return num_bits_; }

  
  int ArraySize() const { return array_size_; }

  
  void SetAll(bool value) {
    memset(map_, (value ? 0xFF : 0x00), array_size_ * sizeof(*map_));
  }

  
  void Clear() { SetAll(false); }

  
  void Set(int index, bool value);
  bool Get(int index) const;
  void Toggle(int index);

  
  
  void SetMapElement(int array_index, uint32 value);

  
  
  uint32 GetMapElement(int array_index) const;

  
  
  void SetMap(const uint32* map, int size);

  
  const uint32* GetMap() const { return map_; }

  
  void SetRange(int begin, int end, bool value);

  
  
  bool TestRange(int begin, int end, bool value) const;

  
  
  
  
  
  
  
  
  
  
  
  bool FindNextBit(int* index, int limit, bool value) const;

  
  
  bool FindNextSetBitBeforeLimit(int* index, int limit) const {
    return FindNextBit(index, limit, true);
  }

  
  
  bool FindNextSetBit(int *index) const {
    return FindNextSetBitBeforeLimit(index, num_bits_);
  }

  
  
  
  
  
  int FindBits(int* index, int limit, bool value) const;

  
  static int RequiredArraySize(int num_bits) {
    
    if (num_bits <= kIntBits)
      return 1;

    return (num_bits + kIntBits - 1) >> kLogIntBits;
  }

 private:
  static const int kIntBits = sizeof(uint32) * 8;
  static const int kLogIntBits = 5;  

  
  
  void SetWordBits(int start, int len, bool value);

  uint32* map_;           
  int num_bits_;          
  int array_size_;        
  bool alloc_;            

  DISALLOW_COPY_AND_ASSIGN(Bitmap);
};

}  

#endif  
