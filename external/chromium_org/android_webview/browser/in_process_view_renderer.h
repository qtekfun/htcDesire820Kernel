// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_IN_PROCESS_VIEW_RENDERER_H_
#define ANDROID_WEBVIEW_BROWSER_IN_PROCESS_VIEW_RENDERER_H_

#include <string>

#include "android_webview/browser/browser_view_renderer.h"
#include "android_webview/browser/gl_view_renderer_manager.h"
#include "base/cancelable_callback.h"
#include "content/public/browser/android/synchronous_compositor.h"
#include "content/public/browser/android/synchronous_compositor_client.h"
#include "ui/gfx/vector2d_f.h"

namespace content {
class SynchronousCompositor;
class WebContents;
}

typedef void* EGLContext;
class SkCanvas;

namespace android_webview {

class AwGLSurface;

class InProcessViewRenderer : public BrowserViewRenderer,
                              public content::SynchronousCompositorClient {
 public:
  static void CalculateTileMemoryPolicy();

  InProcessViewRenderer(BrowserViewRenderer::Client* client,
                        JavaHelper* java_helper,
                        content::WebContents* web_contents);
  virtual ~InProcessViewRenderer();

  static InProcessViewRenderer* FromWebContents(
      content::WebContents* contents);

  
  typedef base::Callback<bool(SkCanvas*)> RenderMethod;
  static bool RenderViaAuxilaryBitmapIfNeeded(
      jobject java_canvas,
      JavaHelper* java_helper,
      const gfx::Vector2d& scroll_correction,
      const gfx::Rect& clip,
      RenderMethod render_source,
      void* owner_key);

  
  virtual bool OnDraw(jobject java_canvas,
                      bool is_hardware_canvas,
                      const gfx::Vector2d& scroll_,
                      const gfx::Rect& clip) OVERRIDE;
  virtual void DrawGL(AwDrawGLInfo* draw_info) OVERRIDE;
  virtual void SetGlobalVisibleRect(const gfx::Rect& visible_rect) OVERRIDE;
  virtual skia::RefPtr<SkPicture> CapturePicture(int width,
                                                 int height) OVERRIDE;
  virtual void EnableOnNewPicture(bool enabled) OVERRIDE;
  virtual void SetIsPaused(bool paused) OVERRIDE;
  virtual void SetViewVisibility(bool visible) OVERRIDE;
  virtual void SetWindowVisibility(bool visible) OVERRIDE;
  virtual void OnSizeChanged(int width, int height) OVERRIDE;
  virtual void ScrollTo(gfx::Vector2d new_value) OVERRIDE;
  virtual void OnAttachedToWindow(int width, int height) OVERRIDE;
  virtual void OnDetachedFromWindow() OVERRIDE;
  virtual void SetDipScale(float dip_scale) OVERRIDE;
  virtual bool IsAttachedToWindow() OVERRIDE;
  virtual bool IsVisible() OVERRIDE;
  virtual gfx::Rect GetScreenRect() OVERRIDE;
  virtual void TrimMemory(int level) OVERRIDE;

  
  virtual void DidInitializeCompositor(
      content::SynchronousCompositor* compositor) OVERRIDE;
  virtual void DidDestroyCompositor(
      content::SynchronousCompositor* compositor) OVERRIDE;
  virtual void SetContinuousInvalidate(bool invalidate) OVERRIDE;
  virtual void SetMaxRootLayerScrollOffset(gfx::Vector2dF new_value) OVERRIDE;
  virtual void SetTotalRootLayerScrollOffset(
      gfx::Vector2dF new_value_css) OVERRIDE;
  virtual void DidUpdateContent() OVERRIDE;
  virtual gfx::Vector2dF GetTotalRootLayerScrollOffset() OVERRIDE;
  virtual bool IsExternalFlingActive() const OVERRIDE;
  virtual void SetRootLayerPageScaleFactor(float page_scale_factor) OVERRIDE;
  virtual void SetRootLayerScrollableSize(gfx::SizeF scrollable_size) OVERRIDE;
  virtual void DidOverscroll(gfx::Vector2dF accumulated_overscroll,
                             gfx::Vector2dF latest_overscroll_delta,
                             gfx::Vector2dF current_fling_velocity) OVERRIDE;

  void WebContentsGone();
  bool RequestProcessGL();

 private:
  
  
  
  void EnsureContinuousInvalidation(
      AwDrawGLInfo* draw_info,
      bool invalidate_ignore_compositor);
  bool DrawSWInternal(jobject java_canvas,
                      const gfx::Rect& clip_bounds);
  bool CompositeSW(SkCanvas* canvas);

  void UpdateCachedGlobalVisibleRect();

  
  
  void FallbackTickFired();
  void ForceFakeCompositeSW();

  void NoLongerExpectsDrawGL();

  bool InitializeHwDraw();

  gfx::Vector2d max_scroll_offset() const;

  void SetMemoryPolicy(content::SynchronousCompositorMemoryPolicy& new_policy);

  
  
  std::string ToString(AwDrawGLInfo* draw_info) const;

  BrowserViewRenderer::Client* client_;
  BrowserViewRenderer::JavaHelper* java_helper_;
  content::WebContents* web_contents_;
  content::SynchronousCompositor* compositor_;

  bool is_paused_;
  bool view_visible_;
  bool window_visible_;  
  bool attached_to_window_;
  float dip_scale_;
  float page_scale_factor_;
  bool on_new_picture_enable_;

  
  
  
  
  bool compositor_needs_continuous_invalidate_;

  
  
  
  bool block_invalidates_;

  
  base::CancelableClosure fallback_tick_;

  int width_;
  int height_;

  bool hardware_initialized_;
  bool hardware_failed_;
  scoped_refptr<AwGLSurface> gl_surface_;

  
  
  EGLContext last_egl_context_;

  
  gfx::Rect cached_global_visible_rect_;

  
  gfx::Vector2d scroll_at_start_of_frame_;

  
  gfx::Vector2dF scroll_offset_dip_;

  
  gfx::Vector2dF max_scroll_offset_dip_;

  
  
  
  gfx::Vector2dF overscroll_rounding_error_;

  GLViewRendererManager::Key manager_key_;

  content::SynchronousCompositorMemoryPolicy memory_policy_;

  DISALLOW_COPY_AND_ASSIGN(InProcessViewRenderer);
};

}  

#endif  
