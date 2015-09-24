/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#ifndef PPAPI_SIMPLE_PS_EVENT_H_
#define PPAPI_SIMPLE_PS_EVENT_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"

#include "sdk_util/macros.h"

EXTERN_C_BEGIN



typedef enum {
  
  PSE_NONE = 0,

  
  PSE_INSTANCE_HANDLEINPUT = 1,

  
  PSE_INSTANCE_HANDLEMESSAGE = 2,

  
  PSE_INSTANCE_DIDCHANGEVIEW = 4,

  
  PSE_INSTANCE_DIDCHANGEFOCUS = 8,

  
  PSE_GRAPHICS3D_GRAPHICS3DCONTEXTLOST = 16,

  
  PSE_MOUSELOCK_MOUSELOCKLOST = 32,

  
  PSE_ALL = -1,
} PSEventType;

typedef uint32_t PSEventTypeMask;

typedef struct {
  PSEventType type;
  union {
    PP_Bool as_bool;
    PP_Resource as_resource;
    struct PP_Var as_var;
  };
} PSEvent;


PSEvent* PSEventTryAcquire();
PSEvent* PSEventWaitAcquire();
void PSEventRelease(PSEvent* event);
void PSEventSetFilter(PSEventTypeMask mask);

void PSEventPost(PSEventType type);
void PSEventPostBool(PSEventType type, PP_Bool state);
void PSEventPostVar(PSEventType type, struct PP_Var var);
void PSEventPostResource(PSEventType type, PP_Resource resource);

EXTERN_C_END

#endif  
