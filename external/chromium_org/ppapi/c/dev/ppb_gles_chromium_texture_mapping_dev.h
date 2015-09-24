// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_DEV_PPB_GLES_CHROMIUM_TEXTURE_MAPPING_DEV_H_
#define PPAPI_C_DEV_PPB_GLES_CHROMIUM_TEXTURE_MAPPING_DEV_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/ppb_opengles2.h"

#define PPB_GLES_CHROMIUM_TEXTURE_MAPPING_DEV_INTERFACE_0_1 \
    "PPB_GLESChromiumTextureMapping(Dev);0.1"
#define PPB_GLES_CHROMIUM_TEXTURE_MAPPING_DEV_INTERFACE \
    PPB_GLES_CHROMIUM_TEXTURE_MAPPING_DEV_INTERFACE_0_1


#define PPB_OPENGLES2_CHROMIUMMAPSUB_DEV_INTERFACE_1_0 \
    "PPB_OpenGLES2ChromiumMapSub(Dev);1.0"


struct PPB_GLESChromiumTextureMapping_Dev_0_1 {
  
  
  
  
  
  
  
  
  // means that all the values of the buffer must be written.
  void* (*MapTexSubImage2DCHROMIUM)(
     PP_Resource context,
     GLenum target,
     GLint level,
     GLint xoffset,
     GLint yoffset,
     GLsizei width,
     GLsizei height,
     GLenum format,
     GLenum type,
     GLenum access);

  
  
  
  void (*UnmapTexSubImage2DCHROMIUM)(PP_Resource context, const void* mem);
};

typedef struct PPB_GLESChromiumTextureMapping_Dev_0_1
    PPB_GLESChromiumTextureMapping_Dev;

#endif  
