// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_BITS_H_
#define BASE_BITS_H_

#include "base/basictypes.h"
#include "base/logging.h"

namespace base {
namespace bits {

inline int Log2Floor(uint32 n) {
  if (n == 0)
    return -1;
  int log = 0;
  uint32 value = n;
  for (int i = 4; i >= 0; --i) {
    int shift = (1 << i);
    uint32 x = value >> shift;
    if (x != 0) {
      value = x;
      log += shift;
    }
  }
  DCHECK_EQ(value, 1u);
  return log;
}

inline int Log2Ceiling(uint32 n) {
  if (n == 0) {
    return -1;
  } else {
    
    return 1 + Log2Floor(n - 1);
  }
}

}  
}  

#endif  
