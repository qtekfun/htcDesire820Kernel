// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_BASE_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_BASE_H_

#if defined(OS_MACOSX)
#include <OpenGL/OpenGL.h>
#endif

#if defined(TOOLKIT_GTK)
#include <gdk/gdk.h>
#endif

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/callback_forward.h"
#include "base/timer/timer.h"
#include "content/common/content_export.h"
#include "content/port/browser/render_widget_host_view_port.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/rect.h"

namespace content {

class RenderWidgetHostImpl;

// that are deriving from it, and code that is written specifically to
class CONTENT_EXPORT RenderWidgetHostViewBase
    : public RenderWidgetHostViewPort {
 public:
  virtual ~RenderWidgetHostViewBase();

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void SelectionChanged(const base::string16& text,
                                size_t offset,
                                const gfx::Range& range) OVERRIDE;
  virtual void SetBackground(const SkBitmap& background) OVERRIDE;
  virtual const SkBitmap& GetBackground() OVERRIDE;
  virtual gfx::Size GetPhysicalBackingSize() const OVERRIDE;
  virtual float GetOverdrawBottomHeight() const OVERRIDE;
  virtual bool IsShowingContextMenu() const OVERRIDE;
  virtual void SetShowingContextMenu(bool showing_menu) OVERRIDE;
  virtual base::string16 GetSelectedText() const OVERRIDE;
  virtual bool IsMouseLocked() OVERRIDE;
  virtual void UnhandledWheelEvent(
      const blink::WebMouseWheelEvent& event) OVERRIDE;
  virtual InputEventAckState FilterInputEvent(
      const blink::WebInputEvent& input_event) OVERRIDE;
  virtual void OnSetNeedsFlushInput() OVERRIDE;
  virtual void OnDidFlushInput() OVERRIDE;
  virtual void GestureEventAck(int gesture_event_type,
                               InputEventAckState ack_result) OVERRIDE;
  virtual void SetPopupType(blink::WebPopupType popup_type) OVERRIDE;
  virtual blink::WebPopupType GetPopupType() OVERRIDE;
  virtual BrowserAccessibilityManager*
      GetBrowserAccessibilityManager() const OVERRIDE;
  virtual void ProcessAckedTouchEvent(const TouchEventWithLatencyInfo& touch,
                                      InputEventAckState ack_result) OVERRIDE;
  virtual scoped_ptr<SyntheticGestureTarget> CreateSyntheticGestureTarget()
      OVERRIDE;
  virtual bool CanSubscribeFrame() const OVERRIDE;
  virtual void BeginFrameSubscription(
      scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber) OVERRIDE;
  virtual void EndFrameSubscription() OVERRIDE;
  virtual void OnSwapCompositorFrame(
      uint32 output_surface_id,
      scoped_ptr<cc::CompositorFrame> frame) OVERRIDE {}
  virtual void ResizeCompositingSurface(const gfx::Size&) OVERRIDE {}
  virtual void OnOverscrolled(gfx::Vector2dF accumulated_overscroll,
                              gfx::Vector2dF current_fling_velocity) OVERRIDE;
  virtual uint32 RendererFrameNumber() OVERRIDE;
  virtual void DidReceiveRendererFrame() OVERRIDE;

  void SetBrowserAccessibilityManager(BrowserAccessibilityManager* manager);

  
  void UpdateScreenInfo(gfx::NativeView view);

  
  
  bool HasDisplayPropertyChanged(gfx::NativeView view);

#if defined(OS_WIN)
  
  
  static void DetachPluginWindowsCallback(HWND window);
#endif

 protected:
  
  RenderWidgetHostViewBase();

#if defined(OS_WIN)
  
  static void MovePluginWindowsHelper(
      HWND parent,
      const std::vector<WebPluginGeometry>& moves);

  static void PaintPluginWindowsHelper(
      HWND parent,
      const gfx::Rect& damaged_screen_rect);

  
  
  static void DetachPluginsHelper(HWND parent);
#endif

  
  
  blink::WebPopupType popup_type_;

  
  
  SkBitmap background_;

  
  
  
  
  
  bool mouse_locked_;

  
  bool showing_context_menu_;

  
  base::string16 selection_text_;

  
  
  size_t selection_text_offset_;

  
  gfx::Range selection_range_;

protected:
  
  float current_device_scale_factor_;

 private:
  void FlushInput();

  
  scoped_ptr<BrowserAccessibilityManager> browser_accessibility_manager_;

  gfx::Rect current_display_area_;

  uint32 renderer_frame_number_;

  base::OneShotTimer<RenderWidgetHostViewBase> flush_input_timer_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewBase);
};

}  

#endif  
