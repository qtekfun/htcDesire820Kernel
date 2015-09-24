// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_WGL_API_IMPLEMENTATION_H_
#define UI_GL_WGL_API_IMPLEMENTATION_H_

#include "base/compiler_specific.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class GLContext;
struct GLWindowSystemBindingInfo;

void InitializeGLBindingsWGL();
void InitializeGLExtensionBindingsWGL(GLContext* context);
void InitializeDebugGLBindingsWGL();
void ClearGLBindingsWGL();
bool GetGLWindowSystemBindingInfoWGL(GLWindowSystemBindingInfo* info);

class GL_EXPORT WGLApiBase : public WGLApi {
 public:
  
  
  
  #include "gl_bindings_api_autogen_wgl.h"

 protected:
  WGLApiBase();
  virtual ~WGLApiBase();
  void InitializeBase(DriverWGL* driver);

  DriverWGL* driver_;
};

class GL_EXPORT RealWGLApi : public WGLApiBase {
 public:
  RealWGLApi();
  virtual ~RealWGLApi();
  void Initialize(DriverWGL* driver);
};

class GL_EXPORT TraceWGLApi : public WGLApi {
 public:
  TraceWGLApi(WGLApi* wgl_api) : wgl_api_(wgl_api) { }
  virtual ~TraceWGLApi();

  
  
  
  #include "gl_bindings_api_autogen_wgl.h"

 private:
  WGLApi* wgl_api_;
};

}  

#endif  



