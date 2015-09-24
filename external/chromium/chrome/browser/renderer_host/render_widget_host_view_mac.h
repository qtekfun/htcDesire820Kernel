// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_H_
#define CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "base/time.h"
#include "chrome/browser/accessibility/browser_accessibility_delegate_mac.h"
#include "chrome/browser/accessibility/browser_accessibility_manager.h"
#include "chrome/browser/ui/cocoa/base_view.h"
#include "content/browser/renderer_host/accelerated_surface_container_manager_mac.h"
#include "content/browser/renderer_host/render_widget_host_view.h"
#include "content/common/edit_command.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebCompositionUnderline.h"
#include "webkit/glue/webcursor.h"

@class AcceleratedPluginView;
class RenderWidgetHostViewMac;
class RWHVMEditCommandHelper;
@class ToolTip;

@protocol RenderWidgetHostViewMacOwner
- (RenderWidgetHostViewMac*)renderWidgetHostViewMac;
@end

@interface RenderWidgetHostViewCocoa
    : BaseView <RenderWidgetHostViewMacOwner,
                NSTextInput,
                NSChangeSpelling,
                BrowserAccessibilityDelegateCocoa> {
 @private
  scoped_ptr<RenderWidgetHostViewMac> renderWidgetHostView_;
  BOOL canBeKeyView_;
  BOOL takesFocusOnlyOnMouseDown_;
  BOOL closeOnDeactivate_;
  scoped_ptr<RWHVMEditCommandHelper> editCommand_helper_;

  
  id trackingRectOwner_;              
  void *trackingRectUserData_;
  NSTrackingRectTag lastToolTipTag_;
  scoped_nsobject<NSString> toolTip_;

  
  BOOL hasOpenMouseDown_;

  NSWindow* lastWindow_;  

  
  
  
  
  
  
  
  
  
  

  
  scoped_nsobject<NSArray> validAttributesForMarkedText_;

  
  
  
  
  NSRect caretRect_;

  
  BOOL handlingKeyDown_;

  
  BOOL hasMarkedText_;

  
  
  BOOL unmarkTextCalled_;

  
  
  
  
  NSRange markedRange_;

  
  
  
  
  
  NSRange selectedRange_;

  
  string16 textToBeInserted_;

  
  string16 markedText_;

  
  std::vector<WebKit::WebCompositionUnderline> underlines_;

  
  
  BOOL hasEditCommands_;

  
  
  
  EditCommands editCommands_;

  
  int focusedPluginIdentifier_;

  
  BOOL pluginImeActive_;

  
  BOOL mouseEventWasIgnored_;
}

@property(assign, nonatomic) NSRect caretRect;

- (void)setCanBeKeyView:(BOOL)can;
- (void)setTakesFocusOnlyOnMouseDown:(BOOL)b;
- (void)setCloseOnDeactivate:(BOOL)b;
- (void)setToolTipAtMousePoint:(NSString *)string;
- (void)setFrameWithDeferredUpdate:(NSRect)frame;
- (void)renderWidgetHostWasResized;
- (void)cancelComposition;
- (void)confirmComposition;
- (void)setPluginImeActive:(BOOL)active;
- (void)pluginFocusChanged:(BOOL)focused forPlugin:(int)pluginId;
- (BOOL)postProcessEventForPluginIme:(NSEvent*)event;

@end

class RenderWidgetHostViewMac : public RenderWidgetHostView {
 public:
  
  
  
  explicit RenderWidgetHostViewMac(RenderWidgetHost* widget);
  virtual ~RenderWidgetHostViewMac();

  RenderWidgetHostViewCocoa* native_view() const { return cocoa_view_; }

  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos);
  virtual void InitAsFullscreen();
  virtual RenderWidgetHost* GetRenderWidgetHost() const;
  virtual void DidBecomeSelected();
  virtual void WasHidden();
  virtual void SetSize(const gfx::Size& size);
  virtual void SetBounds(const gfx::Rect& rect);
  virtual gfx::NativeView GetNativeView();
  virtual void MovePluginWindows(
      const std::vector<webkit::npapi::WebPluginGeometry>& moves);
  virtual void Focus();
  virtual void Blur();
  virtual bool HasFocus();
  virtual void Show();
  virtual void Hide();
  virtual bool IsShowing();
  virtual gfx::Rect GetViewBounds() const;
  virtual void UpdateCursor(const WebCursor& cursor);
  virtual void SetIsLoading(bool is_loading);
  virtual void ImeUpdateTextInputState(WebKit::WebTextInputType state,
                                       const gfx::Rect& caret_rect);
  virtual void ImeCancelComposition();
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect, int scroll_dx, int scroll_dy,
      const std::vector<gfx::Rect>& copy_rects);
  virtual void RenderViewGone(base::TerminationStatus status,
                              int error_code);
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh) {};
  virtual void Destroy();
  virtual void SetTooltipText(const std::wstring& tooltip_text);
  virtual void SelectionChanged(const std::string& text);
  virtual BackingStore* AllocBackingStore(const gfx::Size& size);
  virtual void SetTakesFocusOnlyOnMouseDown(bool flag);
  
  virtual gfx::Rect GetViewCocoaBounds() const;
  virtual gfx::Rect GetRootWindowRect();
  virtual void SetActive(bool active);
  virtual void SetWindowVisibility(bool visible);
  virtual void WindowFrameChanged();
  virtual void SetBackground(const SkBitmap& background);
  virtual bool ContainsNativeView(gfx::NativeView native_view) const;

  virtual void OnAccessibilityNotifications(
      const std::vector<ViewHostMsg_AccessibilityNotification_Params>& params);

  virtual void PluginFocusChanged(bool focused, int plugin_id);
  virtual void StartPluginIme();
  virtual bool PostProcessEventForPluginIme(
      const NativeWebKeyboardEvent& event);

  
  
  virtual gfx::PluginWindowHandle AllocateFakePluginWindowHandle(bool opaque,
                                                                 bool root);
  virtual void DestroyFakePluginWindowHandle(gfx::PluginWindowHandle window);

  
  AcceleratedPluginView* ViewForPluginWindowHandle(
      gfx::PluginWindowHandle window);

  
  
  
  
  void DeallocFakePluginWindowHandle(gfx::PluginWindowHandle window);

  virtual void AcceleratedSurfaceSetIOSurface(gfx::PluginWindowHandle window,
                                              int32 width,
                                              int32 height,
                                              uint64 io_surface_identifier);
  virtual void AcceleratedSurfaceSetTransportDIB(
      gfx::PluginWindowHandle window,
      int32 width,
      int32 height,
      TransportDIB::Handle transport_dib);
  virtual void AcceleratedSurfaceBuffersSwapped(
      gfx::PluginWindowHandle window,
      uint64 surface_id,
      int renderer_id,
      int32 route_id,
      int gpu_host_id,
      uint64 swap_buffers_count);
  virtual void GpuRenderingStateDidChange();

  virtual gfx::PluginWindowHandle GetCompositingSurface();

  void DrawAcceleratedSurfaceInstance(
      CGLContextObj context,
      gfx::PluginWindowHandle plugin_handle,
      NSSize size);
  
  
  void ForceTextureReload();

  virtual void SetVisuallyDeemphasized(const SkColor* color, bool animate);

  void KillSelf();

  void SetTextInputActive(bool active);

  
  void PluginImeCompositionCompleted(const string16& text, int plugin_id);

  const std::string& selected_text() const { return selected_text_; }

  void UpdateRootGpuViewVisibility(bool show_gpu_widget);

  
  
  
  
  void HandleDelayedGpuViewHiding();

  
  
  void AcknowledgeSwapBuffers(int renderer_id,
                              int32 route_id,
                              int gpu_host_id,
                              uint64 swap_buffers_count);

  
  

  
  
  RenderWidgetHost* render_widget_host_;

  
  
  bool about_to_validate_and_paint_;

  scoped_ptr<BrowserAccessibilityManager> browser_accessibility_manager_;

  
  
  
  bool call_set_needs_display_in_rect_pending_;

  
  
  
  NSRect invalid_rect_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks tab_switch_paint_time_;

  
  WebKit::WebTextInputType text_input_type_;

  typedef std::map<gfx::PluginWindowHandle, AcceleratedPluginView*>
      PluginViewMap;
  PluginViewMap plugin_views_;  

  
  AcceleratedSurfaceContainerManagerMac plugin_container_manager_;

 private:
  
  bool IsPopup() const;

  
  
  void UpdateCursorIfOverSelf();

  
  
  void ShutdownHost();

  
  bool IsVoiceOverRunning();

  
  
  RenderWidgetHostViewCocoa* cocoa_view_;

  
  WebCursor current_cursor_;

  
  bool is_loading_;

  
  bool is_hidden_;

  
  std::wstring tooltip_text_;

  
  ScopedRunnableMethodFactory<RenderWidgetHostViewMac> shutdown_factory_;

  
  std::string selected_text_;

  
  
  
  bool needs_gpu_visibility_update_after_repaint_;

  gfx::PluginWindowHandle compositing_surface_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewMac);
};

#endif  
