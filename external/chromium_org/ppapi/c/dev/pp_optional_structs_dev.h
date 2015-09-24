/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PP_OPTIONAL_STRUCTS_DEV_H_
#define PPAPI_C_DEV_PP_OPTIONAL_STRUCTS_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



struct PP_Optional_Double_Dev {
  double value;
  PP_Bool is_set;
};

#endif  

