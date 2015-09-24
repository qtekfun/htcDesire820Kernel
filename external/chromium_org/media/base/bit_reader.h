// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_BIT_READER_H_
#define MEDIA_BASE_BIT_READER_H_

#include <sys/types.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT BitReader {
 public:
  
  
  BitReader(const uint8* data, off_t size);
  ~BitReader();

  
  
  
  
  
  
  
  
  template<typename T> bool ReadBits(int num_bits, T *out) {
    DCHECK_LE(num_bits, static_cast<int>(sizeof(T) * 8));
    uint64 temp;
    bool ret = ReadBitsInternal(num_bits, &temp);
    *out = static_cast<T>(temp);
    return ret;
  }

  
  
  
  
  bool SkipBits(int num_bits);

  
  int bits_available() const;

 private:
  
  bool ReadBitsInternal(int num_bits, uint64* out);

  
  
  
  void UpdateCurrByte();

  
  const uint8* data_;

  
  off_t bytes_left_;

  
  
  uint8 curr_byte_;

  
  int num_remaining_bits_in_curr_byte_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BitReader);
};

}  

#endif  
