// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BARRIER_CLOSURE_H_
#define BASE_BARRIER_CLOSURE_H_

#include "base/base_export.h"
#include "base/callback_forward.h"

namespace base {

BASE_EXPORT base::Closure BarrierClosure(int num_closures,
                                         const base::Closure& done_closure);

}  

#endif  
