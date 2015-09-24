// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_LIB_CRC8_H_
#define RLZ_LIB_CRC8_H_

namespace rlz_lib {
class Crc8 {
 public:
  static bool Generate(const unsigned char* data,
                       int length,
                       unsigned char* check_sum);
  static bool Verify(const unsigned char* data,
                     int length,
                     unsigned char checksum,
                     bool * matches);
};
};  

#endif  
