// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_CRITICAL_CLOSURE_H_
#define BASE_CRITICAL_CLOSURE_H_

#include "base/callback.h"

namespace base {

#if defined(OS_IOS)
base::Closure MakeCriticalClosure(const base::Closure& closure);
#else
inline base::Closure MakeCriticalClosure(const base::Closure& closure) {
  
  
  return closure;
}
#endif  

}  

#endif  
