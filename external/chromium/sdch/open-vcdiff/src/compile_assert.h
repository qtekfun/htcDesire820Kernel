// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_COMPILE_ASSERT_H_
#define OPEN_VCDIFF_COMPILE_ASSERT_H_

#include <config.h>


template <bool>
struct CompileAssert {
};

#define COMPILE_ASSERT(expr, msg) \
  typedef CompileAssert<static_cast<bool>(expr)> \
      msg[static_cast<bool>(expr) ? 1 : -1]


#endif  
