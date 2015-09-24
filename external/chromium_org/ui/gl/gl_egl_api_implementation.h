// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_EGL_API_IMPLEMENTATION_H_
#define UI_GL_EGL_API_IMPLEMENTATION_H_

#include "base/compiler_specific.h"
#include "gl_bindings.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class GLContext;
struct GLWindowSystemBindingInfo;

void InitializeGLBindingsEGL();
void InitializeGLExtensionBindingsEGL(GLContext* context);
void InitializeDebugGLBindingsEGL();
void ClearGLBindingsEGL();
bool GetGLWindowSystemBindingInfoEGL(GLWindowSystemBindingInfo* info);

class GL_EXPORT EGLApiBase : public EGLApi {
 public:
  
  
  
  #include "gl_bindings_api_autogen_egl.h"

 protected:
  EGLApiBase();
  virtual ~EGLApiBase();
  void InitializeBase(DriverEGL* driver);

  DriverEGL* driver_;
};

class GL_EXPORT RealEGLApi : public EGLApiBase {
 public:
  RealEGLApi();
  virtual ~RealEGLApi();
  void Initialize(DriverEGL* driver);
};


class GL_EXPORT TraceEGLApi : public EGLApi {
 public:
  TraceEGLApi(EGLApi* egl_api) : egl_api_(egl_api) { }
  virtual ~TraceEGLApi();

  
  
  
  #include "gl_bindings_api_autogen_egl.h"

 private:
  EGLApi* egl_api_;
};

}  

#endif  



