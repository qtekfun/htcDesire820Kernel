// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_COMMON_BITFIELD_HELPERS_H_
#define GPU_COMMAND_BUFFER_COMMON_BITFIELD_HELPERS_H_

namespace gpu {

template<int shift, int length> class BitField {
 public:
  static const unsigned int kShift = shift;
  static const unsigned int kLength = length;
  
  
  static const unsigned int kMask = 1U + ((1U << (length-1)) - 1U) * 2U;

  
  static unsigned int Get(unsigned int container) {
    return (container >> kShift) & kMask;
  }

  
  static unsigned int MakeValue(unsigned int value) {
    return (value & kMask) << kShift;
  }

  
  static void Set(unsigned int *container, unsigned int field_value) {
    *container = (*container & ~(kMask << kShift)) | MakeValue(field_value);
  }
};

}  

#endif  
