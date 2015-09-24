// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_BROWSER_VIEW_RENDERER_H_
#define ANDROID_WEBVIEW_BROWSER_BROWSER_VIEW_RENDERER_H_

#include "base/android/scoped_java_ref.h"
#include "skia/ext/refptr.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d_f.h"

class SkPicture;
struct AwDrawGLInfo;
struct AwDrawSWFunctionTable;

namespace content {
class ContentViewCore;
}

namespace android_webview {

class BrowserViewRenderer {
 public:
  class Client {
   public:
    
    
    
    virtual bool RequestDrawGL(jobject canvas) = 0;

    
    
    virtual void OnNewPicture() = 0;

    
    virtual void PostInvalidate() = 0;

    
    virtual void UpdateGlobalVisibleRect() = 0;

    
    virtual gfx::Point GetLocationOnScreen() = 0;

    
    virtual void ScrollContainerViewTo(gfx::Vector2d new_value) = 0;

    
    virtual void SetMaxContainerViewScrollOffset(gfx::Vector2d new_value) = 0;

    
    virtual bool IsFlingActive() const = 0;

    
    virtual void SetPageScaleFactor(float page_scale_factor) = 0;

    
    virtual void SetContentsSize(gfx::SizeF contents_size_dip) = 0;

    
    virtual void DidOverscroll(gfx::Vector2d overscroll_delta) = 0;

   protected:
    virtual ~Client() {}
  };

  
  class JavaHelper {
   public:
    
    virtual base::android::ScopedJavaLocalRef<jobject> CreateBitmap(
        JNIEnv* env,
        int width,
        int height,
        const base::android::JavaRef<jobject>& jcanvas,
        void* owner_key) = 0;

    
    virtual void DrawBitmapIntoCanvas(
        JNIEnv* env,
        const base::android::JavaRef<jobject>& jbitmap,
        const base::android::JavaRef<jobject>& jcanvas,
        int x,
        int y) = 0;

    
    virtual base::android::ScopedJavaLocalRef<jobject> RecordBitmapIntoPicture(
        JNIEnv* env,
        const base::android::JavaRef<jobject>& jbitmap) = 0;

   protected:
    virtual ~JavaHelper() {}
  };

  
  static void SetAwDrawSWFunctionTable(AwDrawSWFunctionTable* table);
  static AwDrawSWFunctionTable* GetAwDrawSWFunctionTable();

  

  
  
  
  
  
  
  
  virtual bool OnDraw(jobject java_canvas,
                      bool is_hardware_canvas,
                      const gfx::Vector2d& scroll,
                      const gfx::Rect& clip) = 0;

  
  
  virtual void DrawGL(AwDrawGLInfo* draw_info) = 0;

  
  virtual void SetGlobalVisibleRect(const gfx::Rect& visible_rect) = 0;

  
  virtual skia::RefPtr<SkPicture> CapturePicture(int width, int height) = 0;
  virtual void EnableOnNewPicture(bool enabled) = 0;

  
  virtual void SetIsPaused(bool paused) = 0;
  virtual void SetViewVisibility(bool visible) = 0;
  virtual void SetWindowVisibility(bool visible) = 0;
  virtual void OnSizeChanged(int width, int height) = 0;
  virtual void OnAttachedToWindow(int width, int height) = 0;
  virtual void OnDetachedFromWindow() = 0;

  
  virtual void SetDipScale(float dip_scale) = 0;

  
  virtual void ScrollTo(gfx::Vector2d new_value) = 0;

  
  virtual bool IsAttachedToWindow() = 0;
  virtual bool IsVisible() = 0;
  virtual gfx::Rect GetScreenRect() = 0;

  
  virtual void TrimMemory(int level) = 0;

  virtual ~BrowserViewRenderer() {}
};

}  

#endif  
