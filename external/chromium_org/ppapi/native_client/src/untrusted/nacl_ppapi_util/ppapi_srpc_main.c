/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/native_client/src/untrusted/nacl_ppapi_util/ppapi_srpc_main.h"


struct nacl_irt_ppapihook;

void __nacl_register_thread_creator(const struct nacl_irt_ppapihook *)
    __attribute__((weak));

int main(void) {
  int rv;

  if (__nacl_register_thread_creator) {
    if (!NaClSrpcModuleInit()) {
      return 1;
    }
  }

  rv = PpapiPluginMain();

  if (__nacl_register_thread_creator) {
    NaClSrpcModuleFini();
  }

  return rv;
}
