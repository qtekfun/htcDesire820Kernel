// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_TRANSFORMER_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_TRANSFORMER_MAC_H_

#include <OpenGL/gl.h>

#include "base/basictypes.h"
#include "content/browser/renderer_host/compositing_iosurface_shader_programs_mac.h"
#include "ui/gfx/size.h"

namespace gfx {
class Rect;
}  

namespace content {

class CompositingIOSurfaceTransformer {
 public:
  
  
  
  
  
  
  CompositingIOSurfaceTransformer(
      GLenum texture_target, bool src_texture_needs_y_flip,
      CompositingIOSurfaceShaderPrograms* shader_program_cache);

  ~CompositingIOSurfaceTransformer();

  
  
  void ReleaseCachedGLObjects();

  
  
  
  
  
  
  
  
  
  
  
  bool ResizeBilinear(GLuint src_texture, const gfx::Rect& src_subrect,
                      const gfx::Size& dst_size, GLuint* texture);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool TransformRGBToYV12(
      GLuint src_texture, const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      GLuint* texture_y, GLuint* texture_u, GLuint* texture_v,
      gfx::Size* packed_y_size, gfx::Size* packed_uv_size);

 private:
  enum CachedTexture {
    RGBA_OUTPUT = 0,
    Y_PLANE_OUTPUT,
    UUVV_INTERMEDIATE,
    U_PLANE_OUTPUT,
    V_PLANE_OUTPUT,
    NUM_CACHED_TEXTURES
  };

  
  void PrepareTexture(CachedTexture which, const gfx::Size& size);

  
  
  void PrepareFramebuffer();

  
  
  
  const GLenum texture_target_;
  const bool src_texture_needs_y_flip_;
  CompositingIOSurfaceShaderPrograms* const shader_program_cache_;

  
  GLuint textures_[NUM_CACHED_TEXTURES];
  gfx::Size texture_sizes_[NUM_CACHED_TEXTURES];
  GLuint frame_buffer_;

  
  bool system_supports_multiple_draw_buffers_;

  DISALLOW_COPY_AND_ASSIGN(CompositingIOSurfaceTransformer);
};

}  

#endif  
