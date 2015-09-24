// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_CFTYPEREF_H_
#define BASE_MAC_SCOPED_CFTYPEREF_H_
#pragma once

#include <CoreFoundation/CoreFoundation.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace base {
namespace mac {

template<typename CFT>
class ScopedCFTypeRef {
 public:
  typedef CFT element_type;

  explicit ScopedCFTypeRef(CFT object = NULL)
      : object_(object) {
  }

  ~ScopedCFTypeRef() {
    if (object_)
      CFRelease(object_);
  }

  void reset(CFT object = NULL) {
    if (object_)
      CFRelease(object_);
    object_ = object;
  }

  bool operator==(CFT that) const {
    return object_ == that;
  }

  bool operator!=(CFT that) const {
    return object_ != that;
  }

  operator CFT() const {
    return object_;
  }

  CFT get() const {
    return object_;
  }

  void swap(ScopedCFTypeRef& that) {
    CFT temp = that.object_;
    that.object_ = object_;
    object_ = temp;
  }

  
  
  
  CFT release() WARN_UNUSED_RESULT {
    CFT temp = object_;
    object_ = NULL;
    return temp;
  }

 private:
  CFT object_;

  DISALLOW_COPY_AND_ASSIGN(ScopedCFTypeRef);
};

}  
}  

#endif  
