// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GLX_API_IMPLEMENTATION_H_
#define UI_GL_GLX_API_IMPLEMENTATION_H_

#include "base/compiler_specific.h"
#include "gl_bindings.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class GLContext;
struct GLWindowSystemBindingInfo;

void InitializeGLBindingsGLX();
void InitializeGLExtensionBindingsGLX(GLContext* context);
void InitializeDebugGLBindingsGLX();
void ClearGLBindingsGLX();
bool GetGLWindowSystemBindingInfoGLX(GLWindowSystemBindingInfo* info);

class GL_EXPORT GLXApiBase : public GLXApi {
 public:
  
  
  
  #include "gl_bindings_api_autogen_glx.h"

 protected:
  GLXApiBase();
  virtual ~GLXApiBase();
  void InitializeBase(DriverGLX* driver);

  DriverGLX* driver_;
};

class GL_EXPORT RealGLXApi : public GLXApiBase {
 public:
  RealGLXApi();
  virtual ~RealGLXApi();
  void Initialize(DriverGLX* driver);
};

class GL_EXPORT TraceGLXApi : public GLXApi {
 public:
  TraceGLXApi(GLXApi* glx_api) : glx_api_(glx_api) { }
  virtual ~TraceGLXApi();

  
  
  
  #include "gl_bindings_api_autogen_glx.h"

 private:
  GLXApi* glx_api_;
};

}  

#endif  



