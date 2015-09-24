/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_COMPLETION_CALLBACK_H_
#define PPAPI_C_PP_COMPLETION_CALLBACK_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



typedef void (*PP_CompletionCallback_Func)(void* user_data, int32_t result);

typedef enum {
  PP_COMPLETIONCALLBACK_FLAG_NONE = 0 << 0,
  PP_COMPLETIONCALLBACK_FLAG_OPTIONAL = 1 << 0
} PP_CompletionCallback_Flag;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_CompletionCallback_Flag, 4);

struct PP_CompletionCallback {
  PP_CompletionCallback_Func func;
  void* user_data;
  int32_t flags;
};

#include <stdlib.h>

PP_INLINE struct PP_CompletionCallback PP_MakeCompletionCallback(
    PP_CompletionCallback_Func func,
    void* user_data) {
  struct PP_CompletionCallback cc;
  cc.func = func;
  cc.user_data = user_data;
  cc.flags = PP_COMPLETIONCALLBACK_FLAG_NONE;
  return cc;
}

PP_INLINE struct PP_CompletionCallback PP_MakeOptionalCompletionCallback(
    PP_CompletionCallback_Func func,
    void* user_data) {
  struct PP_CompletionCallback cc = PP_MakeCompletionCallback(func, user_data);
  cc.flags = cc.flags | PP_COMPLETIONCALLBACK_FLAG_OPTIONAL;
  return cc;
}


PP_INLINE void PP_RunCompletionCallback(struct PP_CompletionCallback* cc,
                                        int32_t result) {
  cc->func(cc->user_data, result);
}



PP_INLINE struct PP_CompletionCallback PP_BlockUntilComplete(void) {
  return PP_MakeCompletionCallback(NULL, NULL);
}

PP_INLINE void PP_RunAndClearCompletionCallback(
    struct PP_CompletionCallback* cc,
    int32_t res) {
  struct PP_CompletionCallback temp = *cc;
  *cc = PP_BlockUntilComplete();
  PP_RunCompletionCallback(&temp, res);
}

#endif  

