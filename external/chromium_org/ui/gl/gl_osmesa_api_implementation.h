// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_OSMESA_API_IMPLEMENTATION_H_
#define UI_GL_OSMESA_API_IMPLEMENTATION_H_

#include "base/compiler_specific.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class GLContext;

void InitializeGLBindingsOSMESA();
void InitializeGLExtensionBindingsOSMESA(GLContext* context);
void InitializeDebugGLBindingsOSMESA();
void ClearGLBindingsOSMESA();

class GL_EXPORT OSMESAApiBase : public OSMESAApi {
 public:
  
  
  
  #include "gl_bindings_api_autogen_osmesa.h"

 protected:
  OSMESAApiBase();
  virtual ~OSMESAApiBase();
  void InitializeBase(DriverOSMESA* driver);

  DriverOSMESA* driver_;
};

class GL_EXPORT RealOSMESAApi : public OSMESAApiBase {
 public:
  RealOSMESAApi();
  virtual ~RealOSMESAApi();
  void Initialize(DriverOSMESA* driver);
};

class GL_EXPORT TraceOSMESAApi : public OSMESAApi {
 public:
  TraceOSMESAApi(OSMESAApi* osmesa_api) : osmesa_api_(osmesa_api) { }
  virtual ~TraceOSMESAApi();

  
  
  
  #include "gl_bindings_api_autogen_osmesa.h"

 private:
  OSMESAApi* osmesa_api_;
};

}  

#endif  



