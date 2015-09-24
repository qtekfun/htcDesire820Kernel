/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_RESOURCE_H_
#define PPAPI_C_PP_RESOURCE_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



typedef int32_t PP_Resource;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Resource, 4);

#endif  

