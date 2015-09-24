// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_INTERFACE_H_
#define UI_GL_GL_INTERFACE_H_


#include "ui/gl/gl_bindings.h"

namespace gfx {

class GL_EXPORT GLInterface {
 public:
  virtual ~GLInterface() {}

  static void SetGLInterface(GLInterface* gl_interface);

  static GLInterface* GetGLInterface();

  
  
  
  #include "gl_interface_autogen_gl.h"

 private:
  static GLInterface* interface_;
};

}  

#endif  
