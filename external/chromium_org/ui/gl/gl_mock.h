// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_GL_GL_MOCK_H_
#define UI_GL_GL_MOCK_H_

#include "testing/gmock/include/gmock/gmock.h"
#include "ui/gl/gl_interface.h"

namespace gfx {

class MockGLInterface : public GLInterface {
 public:
  MockGLInterface();
  virtual ~MockGLInterface();

  
  
  
  #include "gl_mock_autogen_gl.h"
};

}  

#endif  
