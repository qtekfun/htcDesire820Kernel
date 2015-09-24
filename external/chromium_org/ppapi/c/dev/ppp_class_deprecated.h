/* Copyright (c) 2010 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef PPAPI_C_PPP_CLASS_DEPRECATED_H_
#define PPAPI_C_PPP_CLASS_DEPRECATED_H_

#include "ppapi/c/dev/deprecated_bool.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"


struct PP_Var;

struct PPP_Class_Deprecated {
  bool (*HasProperty)(void* object,
                      struct PP_Var name,
                      struct PP_Var* exception);

  bool (*HasMethod)(void* object,
                    struct PP_Var name,
                    struct PP_Var* exception);

  struct PP_Var (*GetProperty)(void* object,
                               struct PP_Var name,
                               struct PP_Var* exception);

  void (*GetAllPropertyNames)(void* object,
                              uint32_t* property_count,
                              struct PP_Var** properties,
                              struct PP_Var* exception);

  void (*SetProperty)(void* object,
                      struct PP_Var name,
                      struct PP_Var value,
                      struct PP_Var* exception);

  void (*RemoveProperty)(void* object,
                         struct PP_Var name,
                         struct PP_Var* exception);

  

  struct PP_Var (*Call)(void* object,
                        struct PP_Var method_name,
                        uint32_t argc,
                        struct PP_Var* argv,
                        struct PP_Var* exception);

  
  struct PP_Var (*Construct)(void* object,
                             uint32_t argc,
                             struct PP_Var* argv,
                             struct PP_Var* exception);

  void (*Deallocate)(void* object);
};

#endif  

