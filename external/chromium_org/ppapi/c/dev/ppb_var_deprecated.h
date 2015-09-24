/* Copyright (c) 2010 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef PPAPI_C_PPB_VAR_DEPRECATED_H_
#define PPAPI_C_PPB_VAR_DEPRECATED_H_

#include "ppapi/c/dev/deprecated_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

struct PPP_Class_Deprecated;

#define PPB_VAR_DEPRECATED_INTERFACE_0_3 "PPB_Var(Deprecated);0.3"
#define PPB_VAR_DEPRECATED_INTERFACE PPB_VAR_DEPRECATED_INTERFACE_0_3


struct PPB_Var_Deprecated {
  void (*AddRef)(struct PP_Var var);

  void (*Release)(struct PP_Var var);

  struct PP_Var (*VarFromUtf8)(PP_Module module,
                               const char* data, uint32_t len);

  const char* (*VarToUtf8)(struct PP_Var var, uint32_t* len);

  bool (*HasProperty)(struct PP_Var object,
                      struct PP_Var name,
                      struct PP_Var* exception);

  bool (*HasMethod)(struct PP_Var object,
                    struct PP_Var name,
                    struct PP_Var* exception);

  struct PP_Var (*GetProperty)(struct PP_Var object,
                               struct PP_Var name,
                               struct PP_Var* exception);

  void (*GetAllPropertyNames)(struct PP_Var object,
                              uint32_t* property_count,
                              struct PP_Var** properties,
                              struct PP_Var* exception);

  void (*SetProperty)(struct PP_Var object,
                      struct PP_Var name,
                      struct PP_Var value,
                      struct PP_Var* exception);

  void (*RemoveProperty)(struct PP_Var object,
                         struct PP_Var name,
                         struct PP_Var* exception);

  

  struct PP_Var (*Call)(struct PP_Var object,
                        struct PP_Var method_name,
                        uint32_t argc,
                        struct PP_Var* argv,
                        struct PP_Var* exception);

  struct PP_Var (*Construct)(struct PP_Var object,
                             uint32_t argc,
                             struct PP_Var* argv,
                             struct PP_Var* exception);

  bool (*IsInstanceOf)(struct PP_Var var,
                       const struct PPP_Class_Deprecated* object_class,
                       void** object_data);

  struct PP_Var (*CreateObject)(PP_Instance instance,
                                const struct PPP_Class_Deprecated* object_class,
                                void* object_data);

  
  
  struct PP_Var (*CreateObjectWithModuleDeprecated)(
      PP_Module module,
      const struct PPP_Class_Deprecated* object_class,
      void* object_data);
};

#endif  

