// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_REGION_H_
#define COURGETTE_REGION_H_

#include <string>

#include "base/basictypes.h"

namespace courgette {

class Region {
 public:
  
  Region() : start_(NULL), length_(0) {}

  
  Region(const void* start, size_t length)
      : start_(static_cast<const uint8*>(start)),
        length_(length) {
  }

  
  
  explicit Region(const std::string& string)
      : start_(reinterpret_cast<const uint8*>(string.c_str())),
        length_(string.length()) {
  }

  
  Region(const Region& other) : start_(other.start_), length_(other.length_) {}

  
  Region& assign(const Region& other) {
    this->start_ = other.start_;
    this->length_ = other.length_;
    return *this;
  }

  
  const uint8* start() const { return start_; }

  
  size_t length() const { return length_; }

  
  const uint8* end() const { return start_ + length_; }

 private:
  const uint8* start_;
  size_t length_;

  void operator=(const Region&);  
};

}  
#endif  
