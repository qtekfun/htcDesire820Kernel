// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_WEBPLUGIN_DELEGATE_IMPL_H_
#define CONTENT_CHILD_NPAPI_WEBPLUGIN_DELEGATE_IMPL_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/timer/timer.h"
#include "build/build_config.h"
#include "content/child/npapi/webplugin_delegate.h"
#include "third_party/npapi/bindings/npapi.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "webkit/common/cursors/webcursor.h"

#if defined(USE_X11)
#include "ui/base/x/x11_util.h"

typedef struct _GdkDrawable GdkPixmap;
#endif

namespace base {
class FilePath;
}

#if defined(OS_MACOSX)
#ifdef __OBJC__
@class CALayer;
@class CARenderer;
#else
class CALayer;
class CARenderer;
#endif
#endif

namespace content {
class PluginInstance;
class WebPlugin;

#if defined(OS_MACOSX)
class WebPluginAcceleratedSurface;
class ExternalDragTracker;
#endif  

#if defined(OS_WIN)
class WebPluginIMEWin;
#endif  

class WebPluginDelegateImpl : public WebPluginDelegate {
 public:
  enum PluginQuirks {
    PLUGIN_QUIRK_SETWINDOW_TWICE = 1,  
    PLUGIN_QUIRK_THROTTLE_WM_USER_PLUS_ONE = 2,  
    PLUGIN_QUIRK_DONT_CALL_WND_PROC_RECURSIVELY = 4,  
    PLUGIN_QUIRK_DONT_SET_NULL_WINDOW_HANDLE_ON_DESTROY = 8,  
    PLUGIN_QUIRK_DONT_ALLOW_MULTIPLE_INSTANCES = 16,  
    PLUGIN_QUIRK_DIE_AFTER_UNLOAD = 32,  
    PLUGIN_QUIRK_PATCH_SETCURSOR = 64,  
    PLUGIN_QUIRK_BLOCK_NONSTANDARD_GETURL_REQUESTS = 128,  
    PLUGIN_QUIRK_WINDOWLESS_OFFSET_WINDOW_TO_DRAW = 256,  
    PLUGIN_QUIRK_WINDOWLESS_INVALIDATE_AFTER_SET_WINDOW = 512,  
    PLUGIN_QUIRK_NO_WINDOWLESS = 1024,  
    PLUGIN_QUIRK_PATCH_REGENUMKEYEXW = 2048,  
    PLUGIN_QUIRK_ALWAYS_NOTIFY_SUCCESS = 4096,  
    PLUGIN_QUIRK_HANDLE_MOUSE_CAPTURE = 16384,  
    PLUGIN_QUIRK_WINDOWLESS_NO_RIGHT_CLICK = 32768,  
    PLUGIN_QUIRK_IGNORE_FIRST_SETWINDOW_CALL = 65536,  
    PLUGIN_QUIRK_EMULATE_IME = 131072,  
    PLUGIN_QUIRK_FAKE_WINDOW_FROM_POINT = 262144,  
    PLUGIN_QUIRK_COPY_STREAM_DATA = 524288,  
  };

  static WebPluginDelegateImpl* Create(WebPlugin* plugin,
                                       const base::FilePath& filename,
                                       const std::string& mime_type);

  
  virtual bool Initialize(const GURL& url,
                          const std::vector<std::string>& arg_names,
                          const std::vector<std::string>& arg_values,
                          bool load_manually) OVERRIDE;
  virtual void PluginDestroyed() OVERRIDE;
  virtual void UpdateGeometry(const gfx::Rect& window_rect,
                              const gfx::Rect& clip_rect) OVERRIDE;
  virtual void Paint(SkCanvas* canvas, const gfx::Rect& rect) OVERRIDE;
  virtual void SetFocus(bool focused) OVERRIDE;
  virtual bool HandleInputEvent(const blink::WebInputEvent& event,
                                WebCursor::CursorInfo* cursor_info) OVERRIDE;
  virtual NPObject* GetPluginScriptableObject() OVERRIDE;
  virtual NPP GetPluginNPP() OVERRIDE;
  virtual bool GetFormValue(base::string16* value) OVERRIDE;
  virtual void DidFinishLoadWithReason(const GURL& url,
                                       NPReason reason,
                                       int notify_id) OVERRIDE;
  virtual int GetProcessId() OVERRIDE;
  virtual void SendJavaScriptStream(const GURL& url,
                                    const std::string& result,
                                    bool success,
                                    int notify_id) OVERRIDE;
  virtual void DidReceiveManualResponse(const GURL& url,
                                        const std::string& mime_type,
                                        const std::string& headers,
                                        uint32 expected_length,
                                        uint32 last_modified) OVERRIDE;
  virtual void DidReceiveManualData(const char* buffer, int length) OVERRIDE;
  virtual void DidFinishManualLoading() OVERRIDE;
  virtual void DidManualLoadFail() OVERRIDE;
  virtual WebPluginResourceClient* CreateResourceClient(
      unsigned long resource_id, const GURL& url, int notify_id) OVERRIDE;
  virtual WebPluginResourceClient* CreateSeekableResourceClient(
      unsigned long resource_id, int range_request_id) OVERRIDE;
  virtual void FetchURL(unsigned long resource_id,
                        int notify_id,
                        const GURL& url,
                        const GURL& first_party_for_cookies,
                        const std::string& method,
                        const char* buf,
                        unsigned int len,
                        const GURL& referrer,
                        bool notify_redirects,
                        bool is_plugin_src_load,
                        int origin_pid,
                        int render_view_id) OVERRIDE;
  

  gfx::PluginWindowHandle windowed_handle() const { return windowed_handle_; }
  bool IsWindowless() const { return windowless_; }
  PluginInstance* instance() { return instance_.get(); }
  gfx::Rect GetRect() const { return window_rect_; }
  gfx::Rect GetClipRect() const { return clip_rect_; }

  
  base::FilePath GetPluginPath();

  
  int GetQuirks() const { return quirks_; }

  
  void SetContentAreaHasFocus(bool has_focus);

#if defined(OS_WIN)
  
  void ImeCompositionUpdated(const base::string16& text,
                             const std::vector<int>& clauses,
                             const std::vector<int>& target,
                             int cursor_position);

  
  
  void ImeCompositionCompleted(const base::string16& text);

  
  bool GetIMEStatus(int* input_type, gfx::Rect* caret_rect);
#endif

#if defined(OS_MACOSX) && !defined(USE_AURA)
  
  
  void UpdateGeometryAndContext(const gfx::Rect& window_rect,
                                const gfx::Rect& clip_rect,
                                gfx::NativeDrawingContext context);
  
  
  void PluginDidInvalidate();
  
  static WebPluginDelegateImpl* GetActiveDelegate();
  
  void SetWindowHasFocus(bool has_focus);
  
  void SetContainerVisibility(bool is_visible);
  
  
  void WindowFrameChanged(const gfx::Rect& window_frame,
                          const gfx::Rect& view_frame);
  
  
  void ImeCompositionCompleted(const base::string16& text);
  
  void SetNSCursor(NSCursor* cursor);

  
  
  void SetNoBufferContext();

  
  
  
  void CGPaint(CGContextRef context, const gfx::Rect& rect);
#endif  

#if defined(USE_X11)
  void SetWindowlessShmPixmap(XID shm_pixmap) {
    windowless_shm_pixmap_ = shm_pixmap;
  }
#endif

 private:
  friend class base::DeleteHelper<WebPluginDelegateImpl>;
  friend class WebPluginDelegate;

  WebPluginDelegateImpl(WebPlugin* plugin, PluginInstance* instance);
  virtual ~WebPluginDelegateImpl();

  
  
  bool PlatformInitialize();

  
  void PlatformDestroyInstance();

  
  
  void WindowedUpdateGeometry(const gfx::Rect& window_rect,
                              const gfx::Rect& clip_rect);
  
  
  
  bool WindowedCreatePlugin();

  
  void WindowedDestroyWindow();

  
  
  bool WindowedReposition(const gfx::Rect& window_rect,
                          const gfx::Rect& clip_rect);

  
  
  void WindowedSetWindow();

#if defined(OS_WIN)
  
  ATOM RegisterNativeWindowClass();

  
  static LRESULT CALLBACK WrapperWindowProc(
      HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK NativeWndProc(
      HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT CALLBACK FlashWindowlessWndProc(
      HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT CALLBACK DummyWindowProc(
      HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  
  static void ClearThrottleQueueForWindow(HWND window);
  static void OnThrottleMessage();
  static void ThrottleMessage(WNDPROC proc, HWND hwnd, UINT message,
      WPARAM wParam, LPARAM lParam);
#endif

  
  
  void WindowlessUpdateGeometry(const gfx::Rect& window_rect,
                                const gfx::Rect& clip_rect);
  void WindowlessPaint(gfx::NativeDrawingContext hdc, const gfx::Rect& rect);

  
  
  void WindowlessSetWindow();

  
  
  void SetPluginHasFocus(bool focused);

  
  
  bool PlatformSetPluginHasFocus(bool focused);

  
  

  
  
  bool PlatformHandleInputEvent(const blink::WebInputEvent& event,
                                WebCursor::CursorInfo* cursor_info);

  
  void DestroyInstance();


  
  
  
  
  
  gfx::PluginWindowHandle windowed_handle_;
  gfx::Rect windowed_last_pos_;

  bool windowed_did_set_window_;

  
  bool windowless_;

  WebPlugin* plugin_;
  scoped_refptr<PluginInstance> instance_;

#if defined(OS_WIN)
  
  WNDPROC plugin_wnd_proc_;

  
  uint32 last_message_;
  bool is_calling_wndproc;

  
  
  scoped_ptr<WebPluginIMEWin> plugin_ime_;
#endif  

#if defined(USE_X11)
  
  XID windowless_shm_pixmap_;
#endif

#if defined(TOOLKIT_GTK)
  
  GdkPixmap* pixmap_;
  double first_event_time_;

  
  
  
  
  GtkWidget* plug_;
  GtkWidget* socket_;

  
  void EnsurePixmapAtLeastSize(int width, int height);
#endif

  NPWindow window_;
  gfx::Rect window_rect_;
  gfx::Rect clip_rect_;
  int quirks_;

#if defined(OS_WIN)
  
  
  
  
  
  
  
  
  
  

  HWND dummy_window_for_activation_;
  HWND dummy_window_parent_;
  WNDPROC old_dummy_window_proc_;
  bool CreateDummyWindowForActivation();

  
  
  static bool ShouldTrackEventForModalLoops(NPEvent* event);

  
  
  static LRESULT CALLBACK HandleEventMessageFilterHook(int code, WPARAM wParam,
                                                       LPARAM lParam);

  
  
  static BOOL WINAPI TrackPopupMenuPatch(HMENU menu, unsigned int flags, int x,
                                         int y, int reserved, HWND window,
                                         const RECT* rect);

  
  static HCURSOR WINAPI SetCursorPatch(HCURSOR cursor);

  
  static LONG WINAPI RegEnumKeyExWPatch(
      HKEY key, DWORD index, LPWSTR name, LPDWORD name_size, LPDWORD reserved,
      LPWSTR class_name, LPDWORD class_size, PFILETIME last_write_time);

  
  static FARPROC WINAPI GetProcAddressPatch(HMODULE module, LPCSTR name);

#if defined(USE_AURA)
  
  
  
  
  static HWND WINAPI WindowFromPointPatch(POINT point);
#endif

  
  static LRESULT CALLBACK MouseHookProc(int code, WPARAM wParam,
                                        LPARAM lParam);

  
  static void HandleCaptureForMessage(HWND window, UINT message);

#elif defined(OS_MACOSX) && !defined(USE_AURA)
  
  void SetPluginRect(const gfx::Rect& rect);
  
  void SetContentAreaOrigin(const gfx::Point& origin);
  
  void PluginScreenLocationChanged();
  
  void PluginVisibilityChanged();

  
  void StartIme();

  
  void UpdateAcceleratedSurface();

  
  void DrawLayerInSurface();

  bool use_buffer_context_;
  CGContextRef buffer_context_;  

  CALayer* layer_;  
  WebPluginAcceleratedSurface* surface_;  
  CARenderer* renderer_;  
  scoped_ptr<base::RepeatingTimer<WebPluginDelegateImpl> > redraw_timer_;

  
  
  gfx::Point content_area_origin_;

  bool containing_window_has_focus_;
  bool initial_window_focus_;
  bool container_is_visible_;
  bool have_called_set_window_;

  gfx::Rect cached_clip_rect_;

  bool ime_enabled_;
  int keyup_ignore_count_;

  scoped_ptr<ExternalDragTracker> external_drag_tracker_;
#endif  

  
  void OnModalLoopEntered();

  
  static bool IsUserGesture(const blink::WebInputEvent& event);

  
  std::string plugin_url_;

#if defined(OS_WIN)
  
  void OnUserGestureEnd();

  
  HHOOK handle_event_message_filter_hook_;

  
  
  HANDLE handle_event_pump_messages_event_;

  
  bool user_gesture_message_posted_;

  
  
  base::WeakPtrFactory<WebPluginDelegateImpl> user_gesture_msg_factory_;

  
  
  HHOOK mouse_hook_;
#endif

  
  int handle_event_depth_;

  
  WebCursor current_windowless_cursor_;

  
  
  bool first_set_window_call_;

  
  bool plugin_has_focus_;
  
  
  bool has_webkit_focus_;
  
  
  
  
  bool containing_view_has_focus_;

  
  bool creation_succeeded_;

  DISALLOW_COPY_AND_ASSIGN(WebPluginDelegateImpl);
};

}  

#endif  
