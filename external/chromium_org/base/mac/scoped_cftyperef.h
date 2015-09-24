// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_CFTYPEREF_H_
#define BASE_MAC_SCOPED_CFTYPEREF_H_

#include <CoreFoundation/CoreFoundation.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_policy.h"

namespace base {


template<typename CFT>
class ScopedCFTypeRef {
 public:
  typedef CFT element_type;

  explicit ScopedCFTypeRef(
      CFT object = NULL,
      base::scoped_policy::OwnershipPolicy policy = base::scoped_policy::ASSUME)
      : object_(object) {
    if (object_ && policy == base::scoped_policy::RETAIN)
      CFRetain(object_);
  }

  ScopedCFTypeRef(const ScopedCFTypeRef<CFT>& that)
      : object_(that.object_) {
    if (object_)
      CFRetain(object_);
  }

  ~ScopedCFTypeRef() {
    if (object_)
      CFRelease(object_);
  }

  ScopedCFTypeRef& operator=(const ScopedCFTypeRef<CFT>& that) {
    reset(that.get(), base::scoped_policy::RETAIN);
    return *this;
  }

  void reset(CFT object = NULL,
             base::scoped_policy::OwnershipPolicy policy =
                base::scoped_policy::ASSUME) {
    if (object && policy == base::scoped_policy::RETAIN)
      CFRetain(object);
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
};

}  

#endif  
