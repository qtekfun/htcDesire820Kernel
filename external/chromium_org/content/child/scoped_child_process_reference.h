// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_SCOPED_CHILD_PROCESS_REFERENCE_H_
#define CONTENT_CHILD_SCOPED_CHILD_PROCESS_REFERENCE_H_

#include "base/basictypes.h"

namespace base {
class TimeDelta;
}

namespace content {

class ScopedChildProcessReference {
 public:
  ScopedChildProcessReference();
  ~ScopedChildProcessReference();

  
  
  
  void ReleaseWithDelay(const base::TimeDelta& delay);

 private:
  bool has_reference_;
  DISALLOW_COPY_AND_ASSIGN(ScopedChildProcessReference);
};

}  

#endif  
