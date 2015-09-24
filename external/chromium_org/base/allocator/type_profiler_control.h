// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ALLOCATOR_TYPE_PROFILER_CONTROL_H_
#define BASE_ALLOCATOR_TYPE_PROFILER_CONTROL_H_

#include "base/gtest_prod_util.h"

namespace base {
namespace type_profiler {

class Controller {
 public:
  static void Stop();
  static bool IsProfiling();

 private:
  FRIEND_TEST_ALL_PREFIXES(TypeProfilerTest,
                           TestProfileNewWithoutProfiledDelete);

  
  
  
  static void Restart();
};

}  
}  

#endif  
