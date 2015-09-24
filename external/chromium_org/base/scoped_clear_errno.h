// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SCOPED_CLEAR_ERRNO_H_
#define BASE_SCOPED_CLEAR_ERRNO_H_

#include <errno.h>

#include "base/basictypes.h"

namespace base {

class ScopedClearErrno {
 public:
  ScopedClearErrno() : old_errno_(errno) {
    errno = 0;
  }
  ~ScopedClearErrno() {
    if (errno == 0)
      errno = old_errno_;
  }

 private:
  const int old_errno_;

  DISALLOW_COPY_AND_ASSIGN(ScopedClearErrno);
};

}  

#endif  
