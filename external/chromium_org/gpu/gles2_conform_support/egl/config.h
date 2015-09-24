// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_GLES2_CONFORM_TEST_CONFIG_H_
#define GPU_GLES2_CONFORM_TEST_CONFIG_H_

#include <EGL/egl.h>

#include "base/basictypes.h"

namespace egl {

class Config {
 public:
  Config();
  ~Config();

  bool GetAttrib(EGLint attribute, EGLint* value) const;

 private:
  
  EGLint buffer_size_;
  
  EGLint red_size_;
  
  EGLint green_size_;
  
  EGLint blue_size_;
  
  EGLint luminance_size_;
  
  EGLint alpha_size_;
  
  EGLint alpha_mask_size_;
  
  EGLBoolean bind_to_texture_rgb_;
  
  EGLBoolean bind_to_texture_rgba_;
  
  EGLenum color_buffer_type_;
  
  EGLenum config_caveat_;
  
  EGLint config_id_;
  
  EGLint conformant_;
  
  EGLint depth_size_;
  
  EGLint level_;
  
  EGLint max_pbuffer_width_;
  
  EGLint max_pbuffer_height_;
  
  EGLint max_pbuffer_pixels_;
  
  EGLint min_swap_interval_;
  
  EGLint max_swap_interval_;
  
  EGLBoolean native_renderable_;
  
  EGLint native_visual_id_;
  
  EGLint native_visual_type_;
  
  EGLint renderable_type_;
  
  EGLint sample_buffers_;
  
  EGLint samples_;
  
  EGLint stencil_size_;
  
  EGLint surface_type_;
  
  EGLenum transparent_type_;
  
  EGLint transparent_red_value_;
  
  EGLint transparent_green_value_;
  
  EGLint transparent_blue_value_;

  DISALLOW_COPY_AND_ASSIGN(Config);
};

}  

#endif  
