// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_CFFILEDESCRIPTORREF_H_
#define BASE_MAC_SCOPED_CFFILEDESCRIPTORREF_H_

#include <CoreFoundation/CoreFoundation.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace base {
namespace mac {

class ScopedCFFileDescriptorRef {
 public:
  explicit ScopedCFFileDescriptorRef(CFFileDescriptorRef fdref = NULL)
      : fdref_(fdref) {
  }

  ~ScopedCFFileDescriptorRef() {
    if (fdref_) {
      CFFileDescriptorInvalidate(fdref_);
      CFRelease(fdref_);
    }
  }

  void reset(CFFileDescriptorRef fdref = NULL) {
    if (fdref_ == fdref)
      return;
    if (fdref_) {
      CFFileDescriptorInvalidate(fdref_);
      CFRelease(fdref_);
    }
    fdref_ = fdref;
  }

  bool operator==(CFFileDescriptorRef that) const {
    return fdref_ == that;
  }

  bool operator!=(CFFileDescriptorRef that) const {
    return fdref_ != that;
  }

  operator CFFileDescriptorRef() const {
    return fdref_;
  }

  CFFileDescriptorRef get() const {
    return fdref_;
  }

  CFFileDescriptorRef release() WARN_UNUSED_RESULT {
    CFFileDescriptorRef temp = fdref_;
    fdref_ = NULL;
    return temp;
  }

 private:
  CFFileDescriptorRef fdref_;

  DISALLOW_COPY_AND_ASSIGN(ScopedCFFileDescriptorRef);
};

}  
}  

#endif  
