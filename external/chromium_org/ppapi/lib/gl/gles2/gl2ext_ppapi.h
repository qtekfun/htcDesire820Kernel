// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef PPAPI_LIB_GL_GLES2_GL2EXT_PPAPI_H_
#define PPAPI_LIB_GL_GLES2_GL2EXT_PPAPI_H_

#include <GLES2/gl2platform.h>

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_opengles2.h"
#include "ppapi/c/dev/ppb_opengles2ext_dev.h"

#ifdef __cplusplus
extern "C" {
#endif  

GL_APICALL GLboolean GL_APIENTRY glInitializePPAPI(
    PPB_GetInterface get_browser_interface);

GL_APICALL GLboolean GL_APIENTRY glTerminatePPAPI(void);

GL_APICALL void GL_APIENTRY glSetCurrentContextPPAPI(PP_Resource context);

GL_APICALL PP_Resource GL_APIENTRY glGetCurrentContextPPAPI(void);

GL_APICALL const struct PPB_OpenGLES2* GL_APIENTRY glGetInterfacePPAPI(void);
GL_APICALL const struct PPB_OpenGLES2InstancedArrays* GL_APIENTRY
    glGetInstancedArraysInterfacePPAPI(void);
GL_APICALL const struct PPB_OpenGLES2FramebufferBlit* GL_APIENTRY
    glGetFramebufferBlitInterfacePPAPI(void);
GL_APICALL const struct PPB_OpenGLES2FramebufferMultisample* GL_APIENTRY
    glGetFramebufferMultisampleInterfacePPAPI(void);
GL_APICALL const struct PPB_OpenGLES2ChromiumEnableFeature* GL_APIENTRY
    glGetChromiumEnableFeatureInterfacePPAPI(void);
GL_APICALL const struct PPB_OpenGLES2ChromiumMapSub* GL_APIENTRY
    glGetChromiumMapSubInterfacePPAPI(void);
GL_APICALL const struct PPB_OpenGLES2Query* GL_APIENTRY
    glGetQueryInterfacePPAPI(void);

#ifdef __cplusplus
}
#endif  

#endif  

