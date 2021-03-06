/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef NATIVE_CLIENT_SRC_SHARED_PPAPI_PROXY_PPRUNTIME_H_
#define NATIVE_CLIENT_SRC_SHARED_PPAPI_PROXY_PPRUNTIME_H_

#include "native_client/src/include/portability.h"
#include "native_client/src/untrusted/irt/irt_ppapi.h"

EXTERN_C_BEGIN

int IrtInit(void);

int PpapiPluginMain(void);

void PpapiPluginRegisterThreadCreator(
    const struct PP_ThreadFunctions* new_funcs);

EXTERN_C_END

#endif  
