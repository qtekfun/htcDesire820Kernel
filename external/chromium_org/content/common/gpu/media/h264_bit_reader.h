// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_H264_BIT_READER_H_
#define CONTENT_COMMON_GPU_MEDIA_H264_BIT_READER_H_

#include <sys/types.h>

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT H264BitReader {
 public:
  H264BitReader();
  ~H264BitReader();

  
  
  
  
  
  bool Initialize(const uint8* data, off_t size);

  
  
  
  
  
  bool ReadBits(int num_bits, int *out);

  
  off_t NumBitsLeft();

  
  bool HasMoreRBSPData();

  
  size_t NumEmulationPreventionBytesRead();

 private:
  
  
  bool UpdateCurrByte();

  
  const uint8* data_;

  
  off_t bytes_left_;

  
  
  int curr_byte_;

  
  int num_remaining_bits_in_curr_byte_;

  
  
  int prev_two_bytes_;

  
  size_t emulation_prevention_bytes_;

  DISALLOW_COPY_AND_ASSIGN(H264BitReader);
};

}  

#endif  
