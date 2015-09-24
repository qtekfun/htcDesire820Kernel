// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_GENERIC_INL_H_
#define BASE_STACKTRACE_GENERIC_INL_H_

#include <execinfo.h>
#include <string.h>
#include "gperftools/stacktrace.h"
#endif  


int GET_STACK_TRACE_OR_FRAMES {
  static const int kStackLength = 64;
  void * stack[kStackLength];
  int size;

  size = backtrace(stack, kStackLength);
  skip_count++;  
  int result_count = size - skip_count;
  if (result_count < 0)
    result_count = 0;
  if (result_count > max_depth)
    result_count = max_depth;
  for (int i = 0; i < result_count; i++)
    result[i] = stack[i + skip_count];

#if IS_STACK_FRAMES
  
  memset(sizes, 0, sizeof(*sizes) * result_count);
#endif

  return result_count;
}
