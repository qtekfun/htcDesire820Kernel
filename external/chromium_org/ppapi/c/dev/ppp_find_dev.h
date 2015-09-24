/* Copyright (c) 2010 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef PPAPI_C_DEV_PPP_FIND_DEV_H_
#define PPAPI_C_DEV_PPP_FIND_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"

#define PPP_FIND_DEV_INTERFACE "PPP_Find(Dev);0.3"

struct PPP_Find_Dev {
  
  
  
  
  
  
  
  PP_Bool (*StartFind)(PP_Instance instance,
                       const char* text,
                       PP_Bool case_sensitive);

  
  void (*SelectFindResult)(PP_Instance instance,
                           PP_Bool forward);

  
  
  void (*StopFind)(PP_Instance instance);
};

#endif  

