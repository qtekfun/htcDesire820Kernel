// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_CALLBACK_HELPERS_H_
#define BASE_CALLBACK_HELPERS_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"

namespace base {

template <typename Sig>
base::Callback<Sig> ResetAndReturn(base::Callback<Sig>* cb) {
  base::Callback<Sig> ret(*cb);
  cb->Reset();
  return ret;
}

class BASE_EXPORT ScopedClosureRunner {
 public:
  ScopedClosureRunner();
  explicit ScopedClosureRunner(const Closure& closure);
  ~ScopedClosureRunner();

  void Reset();
  void Reset(const Closure& closure);
  Closure Release() WARN_UNUSED_RESULT;

 private:
  Closure closure_;

  DISALLOW_COPY_AND_ASSIGN(ScopedClosureRunner);
};

}  

#endif  
