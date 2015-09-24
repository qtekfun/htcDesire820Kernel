// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_SDK_UTIL_REF_OBJECT
#define LIBRARIES_SDK_UTIL_REF_OBJECT

#include <stdlib.h>
#include "pthread.h"

#include "sdk_util/atomicops.h"

namespace sdk_util {

class ScopedRefBase;


class RefObject {
 public:
  RefObject() {
    ref_count_ = 0;
  }

  int RefCount() const { return ref_count_; }

 protected:
  virtual ~RefObject() {}

  
  virtual void Destroy() {}

  void Acquire() {
    AtomicAddFetch(&ref_count_, 1);
  }

  bool Release() {
    Atomic32 val = AtomicAddFetch(&ref_count_, -1);
    if (val == 0) {
      Destroy();
      delete this;
      return false;
    }
    return true;
  }

 private:
  Atomic32 ref_count_;

  friend class ScopedRefBase;
};

}  

#endif  

