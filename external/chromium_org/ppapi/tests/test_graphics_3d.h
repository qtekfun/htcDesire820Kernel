// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_GRAPHICS_3D_H_
#define PAPPI_TESTS_TEST_GRAPHICS_3D_H_

#include <string>
#include "ppapi/tests/test_case.h"

struct PPB_OpenGLES2;

namespace pp {
class Graphics3D;
}  

class TestGraphics3D : public TestCase {
 public:
  TestGraphics3D(TestingInstance* instance) : TestCase(instance) {}

  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

 private:
  
  std::string TestExtensionsGL();
  std::string TestFrameGL();
  std::string TestFramePPAPI();
  std::string TestBadResource();

  
  int32_t SwapBuffersSync(pp::Graphics3D* context);
  std::string CheckPixelPPAPI(pp::Graphics3D* context,
                             int x, int y, const uint8_t expected_color[4]);
  std::string CheckPixelGL(int x, int y, const uint8_t expected_color[4]);

  
  const PPB_OpenGLES2* opengl_es2_;
};

#endif  
