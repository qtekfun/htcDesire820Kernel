// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_BYTE_RANGE_H_
#define NET_HTTP_HTTP_BYTE_RANGE_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT HttpByteRange {
 public:
  HttpByteRange();

  
  static HttpByteRange Bounded(int64 first_byte_position,
                               int64 last_byte_position);
  static HttpByteRange RightUnbounded(int64 first_byte_position);
  static HttpByteRange Suffix(int64 suffix_length);

  
  
  int64 first_byte_position() const { return first_byte_position_; }
  void set_first_byte_position(int64 value) { first_byte_position_ = value; }

  int64 last_byte_position() const { return last_byte_position_; }
  void set_last_byte_position(int64 value) { last_byte_position_ = value; }

  int64 suffix_length() const { return suffix_length_; }
  void set_suffix_length(int64 value) { suffix_length_ = value; }

  
  bool IsSuffixByteRange() const;
  
  bool HasFirstBytePosition() const;
  
  bool HasLastBytePosition() const;

  
  bool IsValid() const;

  
  
  std::string GetHeaderValue() const;

  
  
  
  
  
  
  
  bool ComputeBounds(int64 size);

 private:
  int64 first_byte_position_;
  int64 last_byte_position_;
  int64 suffix_length_;
  bool has_computed_bounds_;
};

}  

#endif  
