/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_TIME_H_
#define PPAPI_C_PP_TIME_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



typedef double PP_Time;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Time, 8);

typedef double PP_TimeTicks;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TimeTicks, 8);

typedef double PP_TimeDelta;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TimeDelta, 8);

#endif  

