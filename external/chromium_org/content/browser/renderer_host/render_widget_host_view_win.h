// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_WIN_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_WIN_H_

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/win/scoped_comptr.h"
#include "content/browser/accessibility/browser_accessibility_manager.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/common/content_export.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/ime/win/tsf_bridge.h"
#include "ui/events/gestures/gesture_recognizer.h"
#include "ui/events/gestures/gesture_types.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/surface/accelerated_surface_win.h"
#include "webkit/common/cursors/webcursor.h"

class SkRegion;

namespace gfx {
class Size;
class Rect;
}

namespace IPC {
class Message;
}

namespace ui {
class IMM32Manager;
class ViewProp;
}

namespace blink {
struct WebScreenInfo;
}

namespace content {
class BackingStore;
class RenderWidgetHost;
class WebTouchState;

typedef CWinTraits<WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0>
    RenderWidgetHostHWNDTraits;

CONTENT_EXPORT extern const wchar_t kRenderWidgetHostHWNDClass[];

class RenderWidgetHostViewWin
    : public CWindowImpl<RenderWidgetHostViewWin,
                         CWindow,
                         RenderWidgetHostHWNDTraits>,
      public RenderWidgetHostViewBase,
      public NotificationObserver,
      public BrowserAccessibilityDelegate,
      public ui::GestureConsumer,
      public ui::GestureEventHelper,
      public ui::TextInputClient {  
 public:
  virtual ~RenderWidgetHostViewWin();

  CONTENT_EXPORT void CreateWnd(HWND parent);

  void AcceleratedPaint(HDC dc);

  DECLARE_WND_CLASS_EX(kRenderWidgetHostHWNDClass, CS_DBLCLKS, 0);

  BEGIN_MSG_MAP(RenderWidgetHostHWND)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_ACTIVATE(OnActivate)
    MSG_WM_DESTROY(OnDestroy)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_NCPAINT(OnNCPaint)
    MSG_WM_NCHITTEST(OnNCHitTest)
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
    MESSAGE_HANDLER(WM_IME_REQUEST, OnImeRequest)
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
    MESSAGE_HANDLER(WM_TOUCH, OnTouchEvent)
    MESSAGE_HANDLER(WM_IME_CHAR, OnKeyEvent)
    MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
    MESSAGE_HANDLER(WM_GETOBJECT, OnGetObject)
    MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
    MESSAGE_HANDLER(WM_GESTURE, OnGestureEvent)
    MESSAGE_HANDLER(WM_MOVE, OnMoveOrSize)
    MESSAGE_HANDLER(WM_SIZE, OnMoveOrSize)
    MESSAGE_HANDLER(WM_WTSSESSION_CHANGE, OnSessionChange)
  END_MSG_MAP()

  
  virtual void InitAsChild(gfx::NativeView parent_view) OVERRIDE;
  virtual RenderWidgetHost* GetRenderWidgetHost() const OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual void SetBounds(const gfx::Rect& rect) OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeViewId GetNativeViewId() const OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;
  virtual bool HasFocus() const OVERRIDE;
  virtual bool IsSurfaceAvailableForCopy() const OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsShowing() OVERRIDE;
  virtual gfx::Rect GetViewBounds() const OVERRIDE;
  virtual void SetBackground(const SkBitmap& background) OVERRIDE;

  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos) OVERRIDE;
  virtual void InitAsFullscreen(
      RenderWidgetHostView* reference_host_view) OVERRIDE;
  virtual void WasShown() OVERRIDE;
  virtual void WasHidden() OVERRIDE;
  virtual void MovePluginWindows(
      const gfx::Vector2d& scroll_offset,
      const std::vector<WebPluginGeometry>& moves) OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void Blur() OVERRIDE;
  virtual void UpdateCursor(const WebCursor& cursor) OVERRIDE;
  virtual void SetIsLoading(bool is_loading) OVERRIDE;
  virtual void TextInputTypeChanged(ui::TextInputType type,
                                    ui::TextInputMode input_mode,
                                    bool can_compose_inline) OVERRIDE;
  virtual void SelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params) OVERRIDE;
  virtual void ScrollOffsetChanged() OVERRIDE;
  virtual void ImeCancelComposition() OVERRIDE;
  virtual void ImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds) OVERRIDE;
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect,
      const gfx::Vector2d& scroll_delta,
      const std::vector<gfx::Rect>& copy_rects,
      const ui::LatencyInfo& latency_info) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status,
                                 int error_code) OVERRIDE;
  virtual bool CanSubscribeFrame() const OVERRIDE;

  
  virtual void WillWmDestroy() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual void SetTooltipText(const base::string16& tooltip_text) OVERRIDE;
  virtual BackingStore* AllocBackingStore(const gfx::Size& size) OVERRIDE;
  virtual void CopyFromCompositingSurface(
      const gfx::Rect& src_subrect,
      const gfx::Size& dst_size,
      const base::Callback<void(bool, const SkBitmap&)>& callback) OVERRIDE;
  virtual void CopyFromCompositingSurfaceToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual bool CanCopyToVideoFrame() const OVERRIDE;
  virtual void OnAcceleratedCompositingStateChange() OVERRIDE;
  virtual void AcceleratedSurfaceInitialized(int host_id,
                                             int route_id) OVERRIDE;
  virtual void ProcessAckedTouchEvent(const TouchEventWithLatencyInfo& touch,
                                      InputEventAckState ack_result) OVERRIDE;
  virtual void SetHasHorizontalScrollbar(
      bool has_horizontal_scrollbar) OVERRIDE;
  virtual void SetScrollOffsetPinning(
      bool is_pinned_to_left, bool is_pinned_to_right) OVERRIDE;
  virtual void GetScreenInfo(blink::WebScreenInfo* results) OVERRIDE;
  virtual gfx::Rect GetBoundsInRootWindow() OVERRIDE;
  virtual gfx::GLSurfaceHandle GetCompositingSurface() OVERRIDE;
  virtual void ResizeCompositingSurface(const gfx::Size&) OVERRIDE;
  virtual void AcceleratedSurfaceBuffersSwapped(
      const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfacePostSubBuffer(
      const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params,
      int gpu_host_id) OVERRIDE;
  virtual void AcceleratedSurfaceSuspend() OVERRIDE;
  virtual void AcceleratedSurfaceRelease() OVERRIDE;
  virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) OVERRIDE;
  virtual void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params
      ) OVERRIDE;
  virtual bool LockMouse() OVERRIDE;
  virtual void UnlockMouse() OVERRIDE;
  virtual void SetClickthroughRegion(SkRegion* region) OVERRIDE;

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual void SetAccessibilityFocus(int acc_obj_id) OVERRIDE;
  virtual void AccessibilityDoDefaultAction(int acc_obj_id) OVERRIDE;
  virtual void AccessibilityScrollToMakeVisible(
      int acc_obj_id, gfx::Rect subfocus) OVERRIDE;
  virtual void AccessibilityScrollToPoint(
      int acc_obj_id, gfx::Point point) OVERRIDE;
  virtual void AccessibilitySetTextSelection(
      int acc_obj_id, int start_offset, int end_offset) OVERRIDE;
  virtual gfx::Point GetLastTouchEventLocation() const OVERRIDE;
  virtual void FatalAccessibilityTreeError() OVERRIDE;

  
  virtual bool CanDispatchToConsumer(ui::GestureConsumer* consumer) OVERRIDE;
  virtual void DispatchPostponedGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void DispatchCancelTouchEvent(ui::TouchEvent* event) OVERRIDE;

  
  
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const base::string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual gfx::NativeWindow GetAttachedWindow() const OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual ui::TextInputMode GetTextInputMode() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual gfx::Rect GetCaretBounds() const OVERRIDE;
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) const OVERRIDE;
  virtual bool HasCompositionText() const OVERRIDE;
  virtual bool GetTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetCompositionTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetSelectionRange(gfx::Range* range) const OVERRIDE;
  virtual bool SetSelectionRange(const gfx::Range& range) OVERRIDE;
  virtual bool DeleteRange(const gfx::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(const gfx::Range& range,
                                base::string16* text) const OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) OVERRIDE;
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE;
  virtual void OnCandidateWindowShown() OVERRIDE;
  virtual void OnCandidateWindowUpdated() OVERRIDE;
  virtual void OnCandidateWindowHidden() OVERRIDE;

 protected:
  friend class RenderWidgetHostView;

  
  
  
  explicit RenderWidgetHostViewWin(RenderWidgetHost* widget);

  
  LRESULT OnCreate(CREATESTRUCT* create_struct);
  void OnActivate(UINT, BOOL, HWND);
  void OnDestroy();
  void OnPaint(HDC unused_dc);
  void OnNCPaint(HRGN update_region);
  LRESULT OnNCHitTest(const CPoint& pt);
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
  LRESULT OnImeRequest(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnMouseEvent(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnKeyEvent(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnWheelEvent(
      UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
  LRESULT OnTouchEvent(
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

  
  LRESULT OnGestureEvent(UINT message, WPARAM wparam, LPARAM lparam,
                         BOOL& handled);
  LRESULT OnMoveOrSize(UINT message, WPARAM wparam, LPARAM lparam,
                       BOOL& handled);

  
  LRESULT OnSessionChange(UINT message,
                          WPARAM wparam,
                          LPARAM lparam,
                          BOOL& handled);

  void OnFinalMessage(HWND window);

 private:
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewWinBrowserTest,
                           TextInputTypeChanged);

  
  
  void UpdateCursorIfOverSelf();

  
  void TrackMouseLeave(bool start_tracking);

  
  bool Send(IPC::Message* message);

  
  
  void EnsureTooltip();

  
  
  void ResetTooltip();

  
  bool ForwardGestureEventToRenderer(
    ui::GestureEvent* gesture);

  
  void ProcessGestures(ui::GestureRecognizer::Gestures* gestures);

  
  void ForwardMouseEventToRenderer(UINT message, WPARAM wparam, LPARAM lparam);

  
  LRESULT SynthesizeMouseWheel(bool is_vertical, int scroll_code,
                               short scroll_position);

  
  
  void ShutdownHost();

  
  
  void Redraw();

  
  
  
  void DrawBackground(const RECT& rect, CPaintDC* dc);

  
  void CleanupCompositorWindow();

  
  bool IsActivatable() const;

  
  void DoPopupOrFullscreenInit(HWND parent_hwnd,
                               const gfx::Rect& pos,
                               DWORD ex_style);

  CPoint GetClientCenter() const;
  
  
  void MoveCursorToCenterIfNecessary();

  void HandleLockedMouseEvent(UINT message, WPARAM wparam, LPARAM lparam);

  LRESULT OnDocumentFeed(RECONVERTSTRING* reconv);
  LRESULT OnReconvertString(RECONVERTSTRING* reconv);
  LRESULT OnQueryCharPosition(IMECHARPOSITION* position);

  
  
  void UpdateDesiredTouchMode();

  
  
  void UpdateIMEState();

  
  gfx::Rect GetPixelBounds() const;

  
  
  
  
  void UpdateInputScopeIfNecessary(ui::TextInputType text_input_type);

  
  
  void CreateBrowserAccessibilityManagerIfNeeded();

  
  
  
  RenderWidgetHostImpl* render_widget_host_;

  
  HWND compositor_host_window_;

  
  
  scoped_ptr<AcceleratedSurface> accelerated_surface_;

  
  
  bool hide_compositor_window_at_next_paint_;

  
  WebCursor current_cursor_;

  
  bool is_loading_;

  
  bool track_mouse_leave_;

  
  
  scoped_ptr<ui::IMM32Manager> imm32_manager_;

  
  
  bool ime_notification_;

  
  bool capture_enter_key_;

  
  
  
  scoped_ptr<WebTouchState> touch_state_;

  
  
  
  bool about_to_validate_and_paint_;

  
  
  bool close_on_deactivate_;

  
  
  
  bool being_destroyed_;

  
  
  base::string16 tooltip_text_;
  
  HWND tooltip_hwnd_;
  
  
  
  bool tooltip_showing_;

  
  base::WeakPtrFactory<RenderWidgetHostViewWin> weak_factory_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks web_contents_switch_paint_time_;

  
  NotificationRegistrar registrar_;

  
  
  ui::TextInputType text_input_type_;
  ui::TextInputMode text_input_mode_;
  bool can_compose_inline_;

  ScopedVector<ui::ViewProp> props_;

  
  bool is_fullscreen_;

  
  struct {
    
    
    
    gfx::Point unlocked;
    
    gfx::Point unlocked_global;

    
    gfx::Point locked_global;
  } last_mouse_position_;

  
  
  
  struct {
    bool pending;
    
    gfx::Point target;
  } move_to_center_request_;

  
  
  bool ignore_mouse_movement_;

  gfx::Range composition_range_;

  
  std::vector<gfx::Rect> composition_character_bounds_;

  
  gfx::Rect caret_rect_;

  
  
  

  
  bool pointer_down_context_;

  
  
  
  gfx::Point last_touch_location_;

  
  scoped_ptr<SkRegion> transparent_region_;

  
  bool touch_events_enabled_;

  scoped_ptr<ui::GestureRecognizer> gesture_recognizer_;

  
  base::win::ScopedComPtr<IAccessible> window_iaccessible_;

  ui::LatencyInfo software_latency_info_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewWin);
};

}  

#endif  
