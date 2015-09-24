// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIN_HWND_MESSAGE_HANDLER_H_
#define UI_VIEWS_WIN_HWND_MESSAGE_HANDLER_H_

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string16.h"
#include "base/win/scoped_gdi_object.h"
#include "base/win/win_util.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/ui_base_types.h"
#include "ui/events/event.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/sequential_id_generator.h"
#include "ui/gfx/win/window_impl.h"
#include "ui/views/ime/input_method_delegate.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class ImageSkia;
class Insets;
}

namespace views {

class FullscreenHandler;
class HWNDMessageHandlerDelegate;
class InputMethod;

const int WM_NCUAHDRAWCAPTION = 0xAE;
const int WM_NCUAHDRAWFRAME = 0xAF;

#define IsMsgHandled() !ref.get() || msg_handled_

#define BEGIN_SAFE_MSG_MAP_EX(the_class) \
 private: \
  base::WeakPtrFactory<the_class> weak_factory_; \
  BOOL msg_handled_; \
\
 public: \
   \
  void SetMsgHandled(BOOL handled) { \
    msg_handled_ = handled; \
  } \
  BOOL ProcessWindowMessage(HWND hwnd, \
                            UINT msg, \
                            WPARAM w_param, \
                            LPARAM l_param, \
                            LRESULT& l_result, \
                            DWORD msg_map_id = 0) { \
    BOOL old_msg_handled = msg_handled_; \
    BOOL ret = _ProcessWindowMessage(hwnd, msg, w_param, l_param, l_result, \
                                     msg_map_id); \
    msg_handled_ = old_msg_handled; \
    return ret; \
  } \
  BOOL _ProcessWindowMessage(HWND hWnd, \
                             UINT uMsg, \
                             WPARAM wParam, \
                             LPARAM lParam, \
                             LRESULT& lResult, \
                             DWORD dwMsgMapID) { \
    base::WeakPtr<HWNDMessageHandler> ref(weak_factory_.GetWeakPtr()); \
    BOOL bHandled = TRUE; \
    hWnd; \
    uMsg; \
    wParam; \
    lParam; \
    lResult; \
    bHandled; \
    switch(dwMsgMapID) { \
      case 0:

class VIEWS_EXPORT HWNDMessageHandler :
    public gfx::WindowImpl,
    public internal::InputMethodDelegate,
    public base::MessageLoopForUI::Observer {
 public:
  explicit HWNDMessageHandler(HWNDMessageHandlerDelegate* delegate);
  ~HWNDMessageHandler();

  void Init(HWND parent, const gfx::Rect& bounds);
  void InitModalType(ui::ModalType modal_type);

  void Close();
  void CloseNow();

  gfx::Rect GetWindowBoundsInScreen() const;
  gfx::Rect GetClientAreaBoundsInScreen() const;
  gfx::Rect GetRestoredBounds() const;
  
  gfx::Rect GetClientAreaBounds() const;

  void GetWindowPlacement(gfx::Rect* bounds,
                          ui::WindowShowState* show_state) const;

  void SetBounds(const gfx::Rect& bounds_in_pixels);
  void SetSize(const gfx::Size& size);
  void CenterWindow(const gfx::Size& size);

  void SetRegion(HRGN rgn);

  void StackAbove(HWND other_hwnd);
  void StackAtTop();

  void Show();
  void ShowWindowWithState(ui::WindowShowState show_state);
  
  void Show(int show_state);
  void ShowMaximizedWithBounds(const gfx::Rect& bounds);
  void Hide();

  void Maximize();
  void Minimize();
  void Restore();

  void Activate();
  void Deactivate();

  void SetAlwaysOnTop(bool on_top);

  bool IsVisible() const;
  bool IsActive() const;
  bool IsMinimized() const;
  bool IsMaximized() const;
  bool IsAlwaysOnTop() const;

  bool RunMoveLoop(const gfx::Vector2d& drag_offset, bool hide_on_escape);
  void EndMoveLoop();

  
  void SendFrameChanged();

  void FlashFrame(bool flash);

  void ClearNativeFocus();

  void SetCapture();
  void ReleaseCapture();
  bool HasCapture() const;

  FullscreenHandler* fullscreen_handler() { return fullscreen_handler_.get(); }

  void SetVisibilityChangedAnimationsEnabled(bool enabled);

  
  bool SetTitle(const string16& title);

  void SetCursor(HCURSOR cursor);

  void FrameTypeChanged();

  void SchedulePaintInRect(const gfx::Rect& rect);
  void SetOpacity(BYTE opacity);

  void SetWindowIcons(const gfx::ImageSkia& window_icon,
                      const gfx::ImageSkia& app_icon);

  void set_remove_standard_frame(bool remove_standard_frame) {
    remove_standard_frame_ = remove_standard_frame;
  }

  void set_use_system_default_icon(bool use_system_default_icon) {
    use_system_default_icon_ = use_system_default_icon;
  }

 private:
  typedef std::set<DWORD> TouchIDs;

  
  virtual void DispatchKeyEventPostIME(const ui::KeyEvent& key) OVERRIDE;

  
  virtual HICON GetDefaultWindowIcon() const OVERRIDE;
  virtual LRESULT OnWndProc(UINT message,
                            WPARAM w_param,
                            LPARAM l_param) OVERRIDE;

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;

  
  
  int GetAppbarAutohideEdges(HMONITOR monitor);

  
  void OnAppbarAutohideEdgesChanged();

  
  
  void SetInitialFocus();

  
  
  void PostProcessActivateMessage(int activation_state, bool minimized);

  
  
  void RestoreEnabledIfNecessary();

  
  void ExecuteSystemMenuCommand(int command);

  
  
  void TrackMouseEvents(DWORD mouse_tracking_flags);

  
  
  void ClientAreaSizeChanged();

  
  
  bool GetClientAreaInsets(gfx::Insets* insets) const;

  
  
  
  void ResetWindowRegion(bool force, bool redraw);

  
  
  
  void UpdateDwmNcRenderingPolicy();

  
  
  
  LRESULT DefWindowProcWithRedrawLock(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param);

  
  void NotifyOwnedWindowsParentClosing();

  
  
  class ScopedRedrawLock;
  void LockUpdates(bool force);
  void UnlockUpdates(bool force);

  
  void StopIgnoringPosChanges() { ignore_window_pos_changes_ = false; }

  
  void RedrawInvalidRect();

  
  
  void RedrawLayeredWindowContents();

  
  
  void ForceRedrawWindow(int attempts);

  

  BEGIN_SAFE_MSG_MAP_EX(HWNDMessageHandler)
    
    MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseRange)
    MESSAGE_RANGE_HANDLER_EX(WM_NCMOUSEMOVE, WM_NCXBUTTONDBLCLK, OnMouseRange)

    
    MESSAGE_HANDLER_EX(WM_NCUAHDRAWCAPTION, OnNCUAHDrawCaption)
    MESSAGE_HANDLER_EX(WM_NCUAHDRAWFRAME, OnNCUAHDrawFrame)

    
    MESSAGE_HANDLER_EX(WM_DWMCOMPOSITIONCHANGED, OnDwmCompositionChanged)

    
    MESSAGE_HANDLER_EX(WM_GETOBJECT, OnGetObject)

    
    MESSAGE_HANDLER_EX(WM_MOUSEACTIVATE, OnMouseActivate)
    MESSAGE_HANDLER_EX(WM_MOUSELEAVE, OnMouseRange)
    MESSAGE_HANDLER_EX(WM_NCMOUSELEAVE, OnMouseRange)
    MESSAGE_HANDLER_EX(WM_SETCURSOR, OnSetCursor);

    
    MESSAGE_HANDLER_EX(WM_KEYDOWN, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_KEYUP, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_SYSKEYDOWN, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_SYSKEYUP, OnKeyEvent)

    
    MESSAGE_HANDLER_EX(WM_IME_SETCONTEXT, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_IME_STARTCOMPOSITION, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_IME_ENDCOMPOSITION, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_IME_REQUEST, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_IME_NOTIFY, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_CHAR, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_SYSCHAR, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_DEADCHAR, OnImeMessages)
    MESSAGE_HANDLER_EX(WM_SYSDEADCHAR, OnImeMessages)

    
    MESSAGE_HANDLER_EX(WM_VSCROLL, OnScrollMessage)
    MESSAGE_HANDLER_EX(WM_HSCROLL, OnScrollMessage)

    
    MESSAGE_HANDLER_EX(WM_TOUCH, OnTouchEvent)

    
    
    
    MESSAGE_HANDLER_EX(WM_NCACTIVATE, OnNCActivate)

    
    MSG_WM_ACTIVATEAPP(OnActivateApp)
    MSG_WM_APPCOMMAND(OnAppCommand)
    MSG_WM_CANCELMODE(OnCancelMode)
    MSG_WM_CAPTURECHANGED(OnCaptureChanged)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_COMMAND(OnCommand)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
    MSG_WM_DISPLAYCHANGE(OnDisplayChange)
    MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove)
    MSG_WM_ERASEBKGND(OnEraseBkgnd)
    MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
    MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
    MSG_WM_INITMENU(OnInitMenu)
    MSG_WM_INPUTLANGCHANGE(OnInputLangChange)
    MSG_WM_KILLFOCUS(OnKillFocus)
    MSG_WM_MOVE(OnMove)
    MSG_WM_MOVING(OnMoving)
    MSG_WM_NCCALCSIZE(OnNCCalcSize)
    MSG_WM_NCHITTEST(OnNCHitTest)
    MSG_WM_NCPAINT(OnNCPaint)
    MSG_WM_NOTIFY(OnNotify)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_SETFOCUS(OnSetFocus)
    MSG_WM_SETICON(OnSetIcon)
    MSG_WM_SETTEXT(OnSetText)
    MSG_WM_SETTINGCHANGE(OnSettingChange)
    MSG_WM_SIZE(OnSize)
    MSG_WM_SYSCOMMAND(OnSysCommand)
    MSG_WM_THEMECHANGED(OnThemeChanged)
    MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
    MSG_WM_WINDOWPOSCHANGING(OnWindowPosChanging)
    MSG_WM_WTSSESSION_CHANGE(OnSessionChange)
  END_MSG_MAP()

  
  
  
  
  void OnActivateApp(BOOL active, DWORD thread_id);
  
  
  BOOL OnAppCommand(HWND window, short command, WORD device, int keystate);
  void OnCancelMode();
  void OnCaptureChanged(HWND window);
  void OnClose();
  void OnCommand(UINT notification_code, int command, HWND window);
  LRESULT OnCreate(CREATESTRUCT* create_struct);
  void OnDestroy();
  void OnDisplayChange(UINT bits_per_pixel, const CSize& screen_size);
  LRESULT OnDwmCompositionChanged(UINT msg, WPARAM w_param, LPARAM l_param);
  void OnEnterSizeMove();
  LRESULT OnEraseBkgnd(HDC dc);
  void OnExitSizeMove();
  void OnGetMinMaxInfo(MINMAXINFO* minmax_info);
  LRESULT OnGetObject(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnImeMessages(UINT message, WPARAM w_param, LPARAM l_param);
  void OnInitMenu(HMENU menu);
  void OnInputLangChange(DWORD character_set, HKL input_language_id);
  LRESULT OnKeyEvent(UINT message, WPARAM w_param, LPARAM l_param);
  void OnKillFocus(HWND focused_window);
  LRESULT OnMouseActivate(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnMouseRange(UINT message, WPARAM w_param, LPARAM l_param);
  void OnMove(const CPoint& point);
  void OnMoving(UINT param, const RECT* new_bounds);
  LRESULT OnNCActivate(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnNCCalcSize(BOOL mode, LPARAM l_param);
  LRESULT OnNCHitTest(const CPoint& point);
  void OnNCPaint(HRGN rgn);
  LRESULT OnNCUAHDrawCaption(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnNCUAHDrawFrame(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnNotify(int w_param, NMHDR* l_param);
  void OnPaint(HDC dc);
  LRESULT OnReflectedMessage(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnScrollMessage(UINT message, WPARAM w_param, LPARAM l_param);
  void OnSessionChange(WPARAM status_code, PWTSSESSION_NOTIFICATION session_id);
  LRESULT OnSetCursor(UINT message, WPARAM w_param, LPARAM l_param);
  void OnSetFocus(HWND last_focused_window);
  LRESULT OnSetIcon(UINT size_type, HICON new_icon);
  LRESULT OnSetText(const wchar_t* text);
  void OnSettingChange(UINT flags, const wchar_t* section);
  void OnSize(UINT param, const CSize& size);
  void OnSysCommand(UINT notification_code, const CPoint& point);
  void OnThemeChanged();
  LRESULT OnTouchEvent(UINT message, WPARAM w_param, LPARAM l_param);
  void OnWindowPosChanging(WINDOWPOS* window_pos);
  void OnWindowPosChanged(WINDOWPOS* window_pos);

  typedef std::vector<ui::TouchEvent> TouchEvents;
  
  
  
  void HandleTouchEvents(const TouchEvents& touch_events);

  HWNDMessageHandlerDelegate* delegate_;

  scoped_ptr<FullscreenHandler> fullscreen_handler_;

  
  bool waiting_for_close_now_;

  bool remove_standard_frame_;

  bool use_system_default_icon_;

  
  
  bool restore_focus_when_enabled_;

  
  
  bool restored_enabled_;

  
  HCURSOR current_cursor_;

  
  
  HCURSOR previous_cursor_;

  

  
  
  
  DWORD active_mouse_tracking_flags_;

  
  
  bool is_right_mouse_pressed_on_caption_;

  
  TouchIDs touch_ids_;

  

  
  
  int lock_updates_count_;

  

  
  
  
  bool ignore_window_pos_changes_;

  
  
  HMONITOR last_monitor_;
  gfx::Rect last_monitor_rect_, last_work_area_;

  

  
  
  
  
  
  
  
  
  
  
  bool use_layered_buffer_;

  
  BYTE layered_alpha_;

  
  
  scoped_ptr<gfx::Canvas> layered_window_contents_;

  
  
  
  
  
  
  
  gfx::Rect invalid_rect_;

  
  bool waiting_for_redraw_layered_window_contents_;

  
  bool is_first_nccalc_;

  
  base::win::ScopedRegion custom_window_region_;

  
  base::WeakPtrFactory<HWNDMessageHandler> autohide_factory_;

  
  ui::SequentialIDGenerator id_generator_;

  
  bool needs_scroll_styles_;

  
  bool in_size_loop_;

  DISALLOW_COPY_AND_ASSIGN(HWNDMessageHandler);
};

const wchar_t kIgnoreTouchMouseActivateForWindow[] =
    L"Chrome.IgnoreMouseActivate";

}  

#endif  
