// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_RENDERING_HELPER_H_
#define CONTENT_COMMON_GPU_MEDIA_RENDERING_HELPER_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_bindings.h"

namespace base {
class MessageLoop;
class WaitableEvent;
}

#if !defined(OS_WIN) && defined(ARCH_CPU_X86_FAMILY)
#define GL_VARIANT_GLX 1
typedef GLXContext NativeContextType;
#else
#define GL_VARIANT_EGL 1
typedef EGLContext NativeContextType;
#endif

namespace content {

struct RenderingHelperParams {
  RenderingHelperParams();
  ~RenderingHelperParams();

  bool suppress_swap_to_display;
  int num_windows;
  
  
  std::vector<gfx::Size> window_dimensions;
  
  std::vector<gfx::Size> frame_dimensions;
  
  
  bool render_as_thumbnails;
  
  gfx::Size thumbnails_page_size;
  
  gfx::Size thumbnail_size;
};

class RenderingHelper {
 public:
  RenderingHelper();
  ~RenderingHelper();

  
  void Initialize(const RenderingHelperParams& params,
                  base::WaitableEvent* done);

  
  void UnInitialize(base::WaitableEvent* done);

  
  
  void CreateTexture(int window_id,
                     uint32 texture_target,
                     uint32* texture_id,
                     base::WaitableEvent* done);

  
  void RenderTexture(uint32 texture_target, uint32 texture_id);

  
  void DeleteTexture(uint32 texture_id);

  
  void* GetGLContext();

  
  void* GetGLDisplay();

  
  
  void GetThumbnailsAsRGB(std::vector<unsigned char>* rgb,
                          bool* alpha_solid,
                          base::WaitableEvent* done);

 private:
  void Clear();

  
  
  void MakeCurrent(int window_id);

  base::MessageLoop* message_loop_;
  std::vector<gfx::Size> window_dimensions_;
  std::vector<gfx::Size> frame_dimensions_;

  NativeContextType gl_context_;
  std::map<uint32, int> texture_id_to_surface_index_;

#if defined(GL_VARIANT_EGL)
  EGLDisplay gl_display_;
  std::vector<EGLSurface> gl_surfaces_;
#else
  XVisualInfo* x_visual_;
#endif

#if defined(OS_WIN)
  std::vector<HWND> windows_;
#else
  Display* x_display_;
  std::vector<Window> x_windows_;
#endif

  bool render_as_thumbnails_;
  int frame_count_;
  GLuint thumbnails_fbo_id_;
  GLuint thumbnails_texture_id_;
  gfx::Size thumbnails_fbo_size_;
  gfx::Size thumbnail_size_;
  GLuint program_;

  DISALLOW_COPY_AND_ASSIGN(RenderingHelper);
};

}  

#endif  
