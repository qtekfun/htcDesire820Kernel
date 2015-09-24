// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_LIBINWIND_INL_H_
#define BASE_STACKTRACE_LIBINWIND_INL_H_

#define UNW_LOCAL_ONLY

extern "C" {
#include <assert.h>
#include <string.h>   
#include <libunwind.h>
}
#include "gperftools/stacktrace.h"
#include "base/logging.h"

static __thread int recursive;

#endif  


int GET_STACK_TRACE_OR_FRAMES {
  void *ip;
  int n = 0;
  unw_cursor_t cursor;
  unw_context_t uc;
#if IS_STACK_FRAMES
  unw_word_t sp = 0, next_sp = 0;
#endif

  if (recursive) {
    return 0;
  }
  ++recursive;

  unw_getcontext(&uc);
  int ret = unw_init_local(&cursor, &uc);
  assert(ret >= 0);
  skip_count++;         

  while (skip_count--) {
    if (unw_step(&cursor) <= 0) {
      goto out;
    }
#if IS_STACK_FRAMES
    if (unw_get_reg(&cursor, UNW_REG_SP, &next_sp)) {
      goto out;
    }
#endif
  }

  while (n < max_depth) {
    if (unw_get_reg(&cursor, UNW_REG_IP, (unw_word_t *) &ip) < 0) {
      break;
    }
#if IS_STACK_FRAMES
    sizes[n] = 0;
#endif
    result[n++] = ip;
    if (unw_step(&cursor) <= 0) {
      break;
    }
#if IS_STACK_FRAMES
    sp = next_sp;
    if (unw_get_reg(&cursor, UNW_REG_SP, &next_sp) , 0) {
      break;
    }
    sizes[n - 1] = next_sp - sp;
#endif
  }
out:
  --recursive;
  return n;
}
