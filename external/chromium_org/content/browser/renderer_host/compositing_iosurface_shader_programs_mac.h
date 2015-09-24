// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_SHADER_PROGRAMS_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_SHADER_PROGRAMS_MAC_H_

#include <OpenGL/gl.h>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

namespace content {

class CompositingIOSurfaceShaderPrograms {
 public:
  CompositingIOSurfaceShaderPrograms();
  ~CompositingIOSurfaceShaderPrograms();

  
  
  
  void Reset();

  
  
  bool UseBlitProgram();

  
  
  bool UseSolidWhiteProgram();

  
  
  
  
  bool UseRGBToYV12Program(int pass_number, float texel_scale_x);

  
  
  GLenum rgb_to_yv12_output_format() const {
    return rgb_to_yv12_output_format_;
  }

 protected:
  FRIEND_TEST_ALL_PREFIXES(CompositingIOSurfaceTransformerTest,
                           TransformsRGBToYV12);

  
  void SetOutputFormatForTesting(GLenum format);

 private:
  enum { kNumShaderPrograms = 4 };

  
  GLuint GetShaderProgram(int which);
  void BindUniformTextureVariable(int which, int texture_unit_offset);
  void BindUniformTexelScaleXVariable(int which, float texel_scale_x);

  
  
  GLuint shader_programs_[kNumShaderPrograms];
  GLint texture_var_locations_[kNumShaderPrograms];
  GLint texel_scale_x_var_locations_[kNumShaderPrograms];

  
  
  GLenum rgb_to_yv12_output_format_;

  DISALLOW_COPY_AND_ASSIGN(CompositingIOSurfaceShaderPrograms);
};

}  

#endif  
