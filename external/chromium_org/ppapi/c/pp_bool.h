/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_BOOL_H_
#define PPAPI_C_PP_BOOL_H_

#include "ppapi/c/pp_macros.h"



typedef enum {
  PP_FALSE = 0,
  PP_TRUE = 1
} PP_Bool;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Bool, 4);

#ifdef __cplusplus
inline PP_Bool PP_FromBool(bool b) {
  return b ? PP_TRUE : PP_FALSE;
}

inline bool PP_ToBool(PP_Bool b) {
  return (b != PP_FALSE);
}

#endif  

#endif  

