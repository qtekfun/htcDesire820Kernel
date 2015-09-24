/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#ifndef PPAPI_SIMPLE_PS_CONTEXT_2D_H_
#define PPAPI_SIMPLE_PS_CONTEXT_2D_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_image_data.h"

#include "ppapi_simple/ps_event.h"

#include "sdk_util/macros.h"

EXTERN_C_BEGIN

typedef struct {
  int bound;
  int32_t width;
  int32_t height;
  uint32_t stride;
  uint32_t* data;
  PP_ImageDataFormat format;
  PP_Resource graphic_2d;
  PP_Resource image;
} PSContext2D_t;


PSContext2D_t* PSContext2DAllocate(PP_ImageDataFormat format);
void PSContext2DFree(PSContext2D_t* ctx);

PP_ImageDataFormat PSContext2DGetNativeImageDataFormat();

int PSContext2DHandleEvent(PSContext2D_t* ctx, PSEvent* event);

int PSContext2DGetBuffer(PSContext2D_t* ctx);

int PSContext2DSwapBuffer(PSContext2D_t* ctx);

EXTERN_C_END

#endif  
