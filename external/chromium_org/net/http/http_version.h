// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_VERSION_H_
#define NET_HTTP_HTTP_VERSION_H_

#include "base/basictypes.h"

namespace net {

class HttpVersion {
 public:
  
  HttpVersion() : value_(0) { }

  
  HttpVersion(uint16 major, uint16 minor) : value_(major << 16 | minor) { }

  
  uint16 major_value() const {
    return value_ >> 16;
  }

  
  uint16 minor_value() const {
    return value_ & 0xffff;
  }

  

  bool operator==(const HttpVersion& v) const {
    return value_ == v.value_;
  }
  bool operator!=(const HttpVersion& v) const {
    return value_ != v.value_;
  }
  bool operator>(const HttpVersion& v) const {
    return value_ > v.value_;
  }
  bool operator>=(const HttpVersion& v) const {
    return value_ >= v.value_;
  }
  bool operator<(const HttpVersion& v) const {
    return value_ < v.value_;
  }
  bool operator<=(const HttpVersion& v) const {
    return value_ <= v.value_;
  }

 private:
  uint32 value_; 
};

}  

#endif  
