// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ALLOCATOR_TYPE_PROFILER_H_
#define BASE_ALLOCATOR_TYPE_PROFILER_H_

#if defined(TYPE_PROFILING)

#include <stddef.h>  
#include <typeinfo>  

namespace base {
namespace type_profiler {

typedef void* InterceptFunction(void*, size_t, const std::type_info&);

class InterceptFunctions {
 public:
  
  
  
  static void SetFunctions(InterceptFunction* new_intercept,
                           InterceptFunction* delete_intercept);

 private:
  friend class TypeProfilerTest;

  
  
  static void ResetFunctions();
  static bool IsAvailable();
};

}  
}  

#endif  

#endif  
