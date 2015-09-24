/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "ppapi/native_client/src/untrusted/irt_stub/thread_creator.h"

#include <pthread.h>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/untrusted/irt/irt.h"


static int thread_create(uintptr_t *tid,
                         void (*func)(void *thread_argument),
                         void *thread_argument) {
  NACL_ASSERT_SAME_SIZE(pthread_t, uintptr_t);

  return pthread_create((pthread_t *) tid, NULL,
                        (void *(*)(void *thread_argument)) func,
                        thread_argument);
}

static int thread_join(uintptr_t tid) {
  return pthread_join((pthread_t) tid, NULL);
}

const static struct PP_ThreadFunctions thread_funcs = {
  thread_create,
  thread_join
};

void __nacl_register_thread_creator(const struct nacl_irt_ppapihook *hooks) {
  hooks->ppapi_register_thread_creator(&thread_funcs);
}
