// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_WIN_H_
#define CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_WIN_H_
#pragma once

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlmisc.h>

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/task.h"
#include "base/win/scoped_comptr.h"
#include "chrome/browser/accessibility/browser_accessibility_manager.h"
#include "content/browser/renderer_host/render_widget_host_view.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/win/ime_input.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/glue/webcursor.h"

class BackingStore;
class RenderWidgetHost;

namespace gfx {
class Size;
class Rect;
}

namespace IPC {
class Message;
}

namespace ui {
class ViewProp;
}

typedef CWinTraits<WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0>
    RenderWidgetHostHWNDTraits;

extern const wchar_t kRenderWidgetHostHWNDClass[];

class RenderWidgetHostViewWin
    : public CWindowImpl<RenderWidgetHostViewWin,
                         CWindow,
                         RenderWidgetHostHWNDTraits>,
      public RenderWidgetHostView,
      public NotificationObserver,
      public BrowserAccessibilityDelegate {
 public:
  
  explicit RenderWidgetHostViewWin(RenderWidgetHost* widget);
  virtual ~RenderWidgetHostViewWin();

  void CreateWnd(HWND parent);

  DECLARE_WND_CLASS_EX(kRenderWidgetHostHWNDClass, CS_DBLCLKS, 0);

  BEGIN_MSG_MAP(RenderWidgetHostHWND)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_ACTIVATE(OnActivate)
    MSG_WM_DESTROY(OnDestroy)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_NCPAINT(OnNCPaint)
    MSG_WM_ERASEBKGND(OnEraseBkgnd)
    MSG_WM_SETCURSOR(OnSetCursor)
    MSG_WM_SETFOCUS(OnSetFocus)
    MSG_WM_KILLFOCUS(OnKillFocus)
    MSG_WM_CAPTURECHANGED(OnCaptureChanged)
    MSG_WM_CANCELMODE(OnCancelMode)
    MSG_WM_INPUTLANGCHANGE(OnInputLangChange)
    MSG_WM_THEMECHANGED(OnThemeChanged)
    MSG_WM_NOTIFY(OnNotify)
    MESSAGE_HANDLER(WM_IME_SETCONTEXT, OnImeSetContext)
    MESSAGE_HANDLER(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
    MESSAGE_HANDLER(WM_IME_COMPOSITION, OnImeComposition)
    MESSAGE_HANDLER(WM_IME_ENDCOMPOSITION, OnImeEndComposition)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseEvent)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseEvent)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseEvent)
    MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMouseEvent)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnMouseEvent)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnMouseEvent)
    MESSAGE_HANDLER(WM_MBUTTONUP, OnMouseEvent)
    MESSAGE_HANDLER(WM_RBUTTONUP, OnMouseEvent)
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnMouseEvent)
    MESSAGE_HANDLER(WM_MBUTTONDBLCLK, OnMouseEvent)
    MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnMouseEvent)
    MESSAGE_HANDLER(WM_SYSKEYDOWN, OnKeyEvent)
    MESSAGE_HANDLER(WM_SYSKEYUP, OnKeyEvent)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyEvent)
    MESSAGE_HANDLER(WM_KEYUP, OnKeyEvent)
    MESSAGE_HANDLER(WM_MOUSEWHEEL, OnWheelEvent)
    MESSAGE_HANDLER(WM_MOUSEHWHEEL, OnWheelEvent)
    MESSAGE_HANDLER(WM_HSCROLL, OnWheelEvent)
    MESSAGE_HANDLER(WM_VSCROLL, OnWheelEvent)
    MESSAGE_HANDLER(WM_CHAR, OnKeyEvent)
    MESSAGE_HANDLER(WM_SYSCHAR, OnKeyEvent)
    MESSAGE_HANDLER(WM_IME_CHAR, OnKeyEvent)
    MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
    MESSAGE_HANDLER(WM_GETOBJECT, OnGetObject)
    MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
  END_MSG_MAP()

  
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
  virtual void ImeUpdateTextInputState(WebKit::WebTextInputType type,
                                       const gfx::Rect& caret_rect);
  virtual void ImeCancelComposition();
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect, int scroll_dx, int scroll_dy,
      const std::vector<gfx::Rect>& copy_rects);
  virtual void RenderViewGone(base::TerminationStatus status,
                              int error_code);
  virtual void WillWmDestroy();  
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh);
  virtual void Destroy();
  virtual void SetTooltipText(const std::wstring& tooltip_text);
  virtual BackingStore* AllocBackingStore(const gfx::Size& size);
  virtual void SetBackground(const SkBitmap& background);
  virtual bool ContainsNativeView(gfx::NativeView native_view) const;
  virtual void SetVisuallyDeemphasized(const SkColor* color, bool animate);

  virtual gfx::PluginWindowHandle GetCompositingSurface();
  virtual void ShowCompositorHostWindow(bool show);

  virtual void OnAccessibilityNotifications(
      const std::vector<ViewHostMsg_AccessibilityNotification_Params>& params);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void SetAccessibilityFocus(int acc_obj_id);
  virtual void AccessibilityDoDefaultAction(int acc_obj_id);

 protected:
  
  LRESULT OnCreate(CREATESTRUCT* create_struct);
  void OnActivate(UINT, BOOL, HWND);
  void OnDestroy();
  void OnPaint(HDC unused_dc);
  void OnNCPaint(HRGN update_region);
  LRESULT OnEraseBkgnd(HDC dc);
  LRESULT OnSetCursor(HWND window, UINT hittest_code, UINT mouse_message_id);
  void OnSetFocus(HWND window);
  void OnKillFocus(HWND window);
  void OnCaptureChanged(HWND window);
  void OnCancelMode();
  void OnInputLangChange(DWORD character_set, HKL input_language_id);
  void OnThemeChanged();
  LRESULT OnNotify(int w_param, NMHDR* header);
  LRESULT OnImeSetContext(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnImeStartComposition(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnImeComposition(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnImeEndComposition(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnMouseEvent(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnKeyEvent(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnWheelEvent(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnMouseActivate(UINT message,
                          WPARAM wparam,
                          LPARAM lparam,
                          BOOL& handled);
  
  LRESULT OnGetObject(UINT message, WPARAM wparam, LPARAM lparam,
                      BOOL& handled);
  
  LRESULT OnVScroll(int code, short position, HWND scrollbar_control);
  
  LRESULT OnHScroll(int code, short position, HWND scrollbar_control);

  LRESULT OnParentNotify(UINT message, WPARAM wparam, LPARAM lparam,
                         BOOL& handled);

  void OnFinalMessage(HWND window);

 private:
  
  
  void UpdateCursorIfOverSelf();

  
  void TrackMouseLeave(bool start_tracking);

  
  bool Send(IPC::Message* message);

  
  
  void EnsureTooltip();

  
  
  void ResetTooltip();

  
  void ForwardMouseEventToRenderer(UINT message, WPARAM wparam, LPARAM lparam);

  
  LRESULT SynthesizeMouseWheel(bool is_vertical, int scroll_code,
                               short scroll_position);

  
  
  void ShutdownHost();

  
  
  void Redraw();

  
  
  
  void DrawBackground(const RECT& rect, CPaintDC* dc);

  
  HWND ReparentWindow(HWND window);

  
  void CleanupCompositorWindow();

  
  bool IsActivatable() const;

  
  RenderWidgetHost* render_widget_host_;

  
  HWND compositor_host_window_;

  
  
  bool hide_compositor_window_at_next_paint_;

  
  WebCursor current_cursor_;

  
  bool is_loading_;

  
  bool track_mouse_leave_;

  
  
  ui::ImeInput ime_input_;

  
  
  bool ime_notification_;

  
  bool capture_enter_key_;

  
  bool is_hidden_;

  
  
  
  bool about_to_validate_and_paint_;

  
  
  bool close_on_deactivate_;

  
  
  
  bool being_destroyed_;

  
  
  std::wstring tooltip_text_;
  
  HWND tooltip_hwnd_;
  
  
  
  bool tooltip_showing_;

  
  ScopedRunnableMethodFactory<RenderWidgetHostViewWin> shutdown_factory_;

  
  
  
  HWND parent_hwnd_;

  
  
  scoped_ptr<BrowserAccessibilityManager> browser_accessibility_manager_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks tab_switch_paint_time_;

  
  
  SkColor overlay_color_;

  
  NotificationRegistrar registrar_;

  
  
  WebKit::WebTextInputType text_input_type_;

  ScopedVector<ui::ViewProp> props_;

  scoped_ptr<ui::ViewProp> accessibility_prop_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewWin);
};

#endif  
