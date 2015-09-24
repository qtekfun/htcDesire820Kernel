// Copyright (c) 2013, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_STACKTRACE_ANDROID_INL_H_
#define BASE_STACKTRACE_ANDROID_INL_H_

#include <stdint.h>   
#include <unwind.h>

#ifdef HAVE_UNWIND_CONTEXT_STRUCT
typedef struct _Unwind_Context __unwind_context;
#else
typedef _Unwind_Context __unwind_context;
#endif

struct stack_crawl_state_t {
  uintptr_t* frames;
  size_t frame_count;
  int max_depth;
  int skip_count;
  bool have_skipped_self;

  stack_crawl_state_t(uintptr_t* frames, int max_depth, int skip_count)
      : frames(frames),
        frame_count(0),
        max_depth(max_depth),
        skip_count(skip_count),
        have_skipped_self(false) {
  }
};

static _Unwind_Reason_Code tracer(__unwind_context* context, void* arg) {
  stack_crawl_state_t* state = static_cast<stack_crawl_state_t*>(arg);

#if defined(__clang__)
  
  
  uintptr_t ip = 0;
  _Unwind_VRS_Get(context, _UVRSC_CORE, 15, _UVRSD_UINT32, &ip);
  ip &= ~(uintptr_t)0x1;  
#else
  uintptr_t ip = _Unwind_GetIP(context);
#endif

  
  if (ip != 0 && !state->have_skipped_self) {
    state->have_skipped_self = true;
    return _URC_NO_REASON;
  }

  if (state->skip_count) {
    --state->skip_count;
    return _URC_NO_REASON;
  }

  state->frames[state->frame_count++] = ip;
  if (state->frame_count >= state->max_depth)
    return _URC_END_OF_STACK;
  else
    return _URC_NO_REASON;
}

#endif  


int GET_STACK_TRACE_OR_FRAMES {
  stack_crawl_state_t state(
      reinterpret_cast<uintptr_t*>(result), max_depth, skip_count);
  _Unwind_Backtrace(tracer, &state);
  return state.frame_count;
}
