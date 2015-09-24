// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_MAC_H_

#include <deque>
#include <vector>

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#include <QuartzCore/QuartzCore.h>

#include "base/callback.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "media/base/video_frame.h"
#include "ui/events/latency_info.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_conversions.h"
#include "ui/gfx/size.h"

class IOSurfaceSupport;
class SkBitmap;

namespace gfx {
class Rect;
}

namespace content {

class CompositingIOSurfaceContext;
class CompositingIOSurfaceShaderPrograms;
class CompositingIOSurfaceTransformer;
class RenderWidgetHostViewFrameSubscriber;
class RenderWidgetHostViewMac;

class CompositingIOSurfaceMac {
 public:
  
  static CompositingIOSurfaceMac* Create();
  ~CompositingIOSurfaceMac();

  
  bool SetIOSurfaceWithContextCurrent(
      scoped_refptr<CompositingIOSurfaceContext> current_context,
      uint64 io_surface_handle,
      const gfx::Size& size,
      float scale_factor,
      const ui::LatencyInfo& latency_info);

  
  int GetRendererID();

  
  
  
  
  bool DrawIOSurface(
      scoped_refptr<CompositingIOSurfaceContext> drawing_context,
      const gfx::Rect& window_rect,
      float window_scale_factor,
      bool flush_drawable);

  
  
  
  
  
  
  
  
  void CopyTo(const gfx::Rect& src_pixel_subrect,
              const gfx::Size& dst_pixel_size,
              const base::Callback<void(bool, const SkBitmap&)>& callback);

  
  
  void CopyToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback);

  
  
  void UnrefIOSurface();

  bool HasIOSurface() { return !!io_surface_.get(); }

  const gfx::Size& pixel_io_surface_size() const {
    return pixel_io_surface_size_;
  }
  
  const gfx::Size& dip_io_surface_size() const { return dip_io_surface_size_; }
  float scale_factor() const { return scale_factor_; }

  
  
  
  void GetVSyncParameters(base::TimeTicks* timebase,
                          uint32* interval_numerator,
                          uint32* interval_denominator);

  
  bool IsAsynchronousReadbackSupported();

 private:
  friend CVReturn DisplayLinkCallback(CVDisplayLinkRef,
                                      const CVTimeStamp*,
                                      const CVTimeStamp*,
                                      CVOptionFlags,
                                      CVOptionFlags*,
                                      void*);

  
  struct SurfaceVertex {
    SurfaceVertex() : x_(0.0f), y_(0.0f), tx_(0.0f), ty_(0.0f) { }
    void set(float x, float y, float tx, float ty) {
      x_ = x;
      y_ = y;
      tx_ = tx;
      ty_ = ty;
    }
    void set_position(float x, float y) {
      x_ = x;
      y_ = y;
    }
    void set_texcoord(float tx, float ty) {
      tx_ = tx;
      ty_ = ty;
    }
    float x_;
    float y_;
    float tx_;
    float ty_;
  };

  
  struct SurfaceQuad {
    void set_size(gfx::Size vertex_size, gfx::Size texcoord_size) {
      
      
      float vw = static_cast<float>(vertex_size.width());
      float vh = static_cast<float>(vertex_size.height());
      float tw = static_cast<float>(texcoord_size.width());
      float th = static_cast<float>(texcoord_size.height());
      verts_[0].set(0.0f, 0.0f, 0.0f, th);
      verts_[1].set(0.0f, vh, 0.0f, 0.0f);
      verts_[2].set(vw, vh, tw, 0.0f);
      verts_[3].set(vw, 0.0f, tw, th);
    }
    void set_rect(float x1, float y1, float x2, float y2) {
      verts_[0].set_position(x1, y1);
      verts_[1].set_position(x1, y2);
      verts_[2].set_position(x2, y2);
      verts_[3].set_position(x2, y1);
    }
    void set_texcoord_rect(float tx1, float ty1, float tx2, float ty2) {
      
      
      verts_[0].set_texcoord(tx1, ty2);
      verts_[1].set_texcoord(tx1, ty1);
      verts_[2].set_texcoord(tx2, ty1);
      verts_[3].set_texcoord(tx2, ty2);
    }
    SurfaceVertex verts_[4];
  };

  
  
  
  
  
  struct CopyContext {
    explicit CopyContext(const scoped_refptr<CompositingIOSurfaceContext>& ctx);
    ~CopyContext();

    
    
    void ReleaseCachedGLObjects();

    
    
    void PrepareReadbackFramebuffers();
    void PrepareForAsynchronousReadback();

    const scoped_ptr<CompositingIOSurfaceTransformer> transformer;
    GLenum output_readback_format;
    int num_outputs;
    GLuint output_textures[3];  
    
    
    gfx::Size output_texture_sizes[3];
    GLuint frame_buffers[3];
    GLuint pixel_buffers[3];
    GLuint fence;  
    int cycles_elapsed;
    base::Callback<bool(const void*, int)> map_buffer_callback;
    base::Callback<void(bool)> done_callback;
  };

  CompositingIOSurfaceMac(
      IOSurfaceSupport* io_surface_support,
      const scoped_refptr<CompositingIOSurfaceContext>& context);

  void SetupCVDisplayLink();

  
  
  
  void SwitchToContextOnNewWindow(NSView* view,
                                  int window_number);

  bool IsVendorIntel();

  
  bool MapIOSurfaceToTextureWithContextCurrent(
      const scoped_refptr<CompositingIOSurfaceContext>& current_context,
      uint64 io_surface_handle);

  void UnrefIOSurfaceWithContextCurrent();

  void DrawQuad(const SurfaceQuad& quad);

  
  void DisplayLinkTick(CVDisplayLinkRef display_link,
                       const CVTimeStamp* time);

  void CalculateVsyncParametersLockHeld(const CVTimeStamp* time);

  
  
  void RateLimitDraws();

  void StartOrContinueDisplayLink();
  void StopDisplayLink();

  
  
  
  
  
  base::Closure CopyToVideoFrameWithinContext(
      const gfx::Rect& src_subrect,
      bool called_within_draw,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback);

  
  // |video_frame_output| may be specified: Either ARGB is written to
  // |bitmap_output| or letter-boxed YV12 is written to |video_frame_output|.
  base::Closure CopyToSelectedOutputWithinContext(
      const gfx::Rect& src_pixel_subrect,
      const gfx::Rect& dst_pixel_rect,
      bool called_within_draw,
      const SkBitmap* bitmap_output,
      const scoped_refptr<media::VideoFrame>& video_frame_output,
      const base::Callback<void(bool)>& done_callback);

  
  void AsynchronousReadbackForCopy(
      const gfx::Rect& dst_pixel_rect,
      bool called_within_draw,
      CopyContext* copy_context,
      const SkBitmap* bitmap_output,
      const scoped_refptr<media::VideoFrame>& video_frame_output);
  bool SynchronousReadbackForCopy(
      const gfx::Rect& dst_pixel_rect,
      CopyContext* copy_context,
      const SkBitmap* bitmap_output,
      const scoped_refptr<media::VideoFrame>& video_frame_output);

  
  
  
  void CheckIfAllCopiesAreFinished(bool block_until_finished);
  void CheckIfAllCopiesAreFinishedWithinContext(
      bool block_until_finished,
      std::vector<base::Closure>* done_callbacks);

  void FailAllCopies();
  void DestroyAllCopyContextsWithinContext();

  
  
  GLenum GetAndSaveGLError();

  gfx::Rect IntersectWithIOSurface(const gfx::Rect& rect) const;

  
  IOSurfaceSupport* io_surface_support_;

  
  
  
  scoped_refptr<CompositingIOSurfaceContext> offscreen_context_;

  
  uint64 io_surface_handle_;
  base::ScopedCFTypeRef<CFTypeRef> io_surface_;

  
  gfx::Size pixel_io_surface_size_;  
  gfx::Size dip_io_surface_size_;  
  float scale_factor_;

  
  
  
  
  
  GLuint texture_;

  
  
  std::vector<CopyContext*> copy_context_pool_;

  
  std::deque<CopyContext*> copy_requests_;

  
  base::Timer finish_copy_timer_;

  
  CVDisplayLinkRef display_link_;

  
  base::DelayTimer<CompositingIOSurfaceMac> display_link_stop_timer_;

  
  base::Lock lock_;

  
  base::TimeTicks vsync_timebase_;
  uint32 vsync_interval_numerator_;
  uint32 vsync_interval_denominator_;

  
  GLint gl_error_;

  ui::LatencyInfo latency_info_;
};

}  

#endif  
