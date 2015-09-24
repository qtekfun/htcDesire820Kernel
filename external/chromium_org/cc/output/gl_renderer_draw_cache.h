// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_GL_RENDERER_DRAW_CACHE_H_
#define CC_OUTPUT_GL_RENDERER_DRAW_CACHE_H_

#include <vector>

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"

namespace cc {

struct Float4 {
  float data[4];
};

struct Float16 {
  float data[16];
};

struct TexturedQuadDrawCache {
  TexturedQuadDrawCache();
  ~TexturedQuadDrawCache();

  
  int program_id;
  int resource_id;
  bool needs_blending;
  SkColor background_color;

  
  int uv_xform_location;
  int background_color_location;
  int vertex_opacity_location;
  int matrix_location;
  int sampler_location;

  
  std::vector<Float4> uv_xform_data;
  std::vector<float> vertex_opacity_data;
  std::vector<Float16> matrix_data;

 private:
  DISALLOW_COPY_AND_ASSIGN(TexturedQuadDrawCache);
};

}  

#endif  
