// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_CHROME_APP_VIEW_H_
#define WIN8_METRO_DRIVER_CHROME_APP_VIEW_H_

#include <windows.applicationmodel.core.h>
#include <windows.ui.core.h>
#include <windows.ui.input.h>
#include <windows.ui.viewmanagement.h>

#include <list>
#include <map>
#include <string>
#include <utility>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/lock.h"
#include "win8/metro_driver/chrome_url_launch_handler.h"
#include "win8/metro_driver/devices_handler.h"
#include "win8/metro_driver/metro_dialog_box.h"
#include "win8/metro_driver/settings_handler.h"
#include "win8/metro_driver/toast_notification_handler.h"

namespace IPC {
  class Listener;
  class ChannelProxy;
}

class ChromeAppView
    : public mswr::RuntimeClass<winapp::Core::IFrameworkView> {
 public:
  ChromeAppView();
  ~ChromeAppView();

  
  IFACEMETHOD(Initialize)(winapp::Core::ICoreApplicationView* view);
  IFACEMETHOD(SetWindow)(winui::Core::ICoreWindow* window);
  IFACEMETHOD(Load)(HSTRING entryPoint);
  IFACEMETHOD(Run)();
  IFACEMETHOD(Uninitialize)();

  static LRESULT CALLBACK CoreWindowProc(HWND window, UINT message, WPARAM wp,
                                         LPARAM lp);

  bool osk_visible_notification_received() const {
    return osk_visible_notification_received_;
  }

  
  void DisplayNotification(
      const ToastNotificationHandler::DesktopNotification& notification);

  
  void CancelNotification(const std::string& notification);

  
  bool IsValidNotification(const std::string& notification);

  
  void ShowDialogBox(const MetroDialogBox::DialogBoxInfo& dialog_box_info);
  
  void DismissDialogBox();

  
  
  static HRESULT Unsnap();

  
  void SetFullscreen(bool fullscreen);

  
  winui::ViewManagement::ApplicationViewState GetViewState();

  HWND core_window_hwnd() { return core_window_hwnd_; }

 private:
  HRESULT OnActivate(winapp::Core::ICoreApplicationView* view,
                     winapp::Activation::IActivatedEventArgs* args);

  HRESULT OnSizeChanged(winui::Core::ICoreWindow* sender,
                        winui::Core::IWindowSizeChangedEventArgs* args);

  HRESULT OnEdgeGestureCompleted(winui::Input::IEdgeGesture* gesture,
                                 winui::Input::IEdgeGestureEventArgs* args);

  HRESULT OnShareDataRequested(
      winapp::DataTransfer::IDataTransferManager* data_transfer_mgr,
      winapp::DataTransfer::IDataRequestedEventArgs* event_args);

  HRESULT OnInputPaneVisible(
      winui::ViewManagement::IInputPane* input_pane,
      winui::ViewManagement::IInputPaneVisibilityEventArgs* event_args);

  HRESULT OnInputPaneHiding(
    winui::ViewManagement::IInputPane* input_pane,
    winui::ViewManagement::IInputPaneVisibilityEventArgs* event_args);

  HRESULT OnPositionChanged(int x, int y);

  void CheckForOSKActivation();

  HRESULT RegisterInputPaneNotifications();

  void HandleInputPaneVisible(const RECT& osk_rect);
  void HandleInputPaneHidden(const RECT& osk_rect);

  mswr::ComPtr<winui::Core::ICoreWindow> window_;
  mswr::ComPtr<winapp::Core::ICoreApplicationView> view_;
  EventRegistrationToken activated_token_;
  EventRegistrationToken edgeevent_token_;
  EventRegistrationToken sizechange_token_;
  EventRegistrationToken share_data_requested_token_;
  EventRegistrationToken input_pane_visible_token_;
  EventRegistrationToken input_pane_hiding_token_;
  EventRegistrationToken app_exit_token_;

  
  HWND core_window_hwnd_;

  ChromeUrlLaunchHandler url_launch_handler_;
  metro_driver::DevicesHandler devices_handler_;
  SettingsHandler settings_handler_;
  mswr::ComPtr<winui::ViewManagement::IInputPane> input_pane_;
  mswr::ComPtr<winui::ViewManagement::IApplicationViewStatics> app_view_;

  bool osk_visible_notification_received_;

  
  typedef std::map<std::string, scoped_ptr<ToastNotificationHandler> >
      NotificationMap;
  NotificationMap notification_map_;

  
  base::Lock notification_lock_;

  
  
  
  int osk_offset_adjustment_;

  MetroDialogBox dialog_box_;
};

struct Globals {
  LPTHREAD_START_ROUTINE host_main;
  void* host_context;
  
  
  
  std::list<std::pair<HWND, bool> > host_windows;
  HANDLE host_thread;
  ChromeAppView* view;
  WNDPROC g_core_proc;
  string16 navigation_url;
  string16 search_string;
  winapp::Activation::ApplicationExecutionState previous_state;
  winapp::Activation::ActivationKind initial_activation_kind;
  bool is_initial_activation;
  
  
  base::MessageLoopProxy* appview_msg_loop;
  winapp::Core::ICoreApplicationExit* app_exit;
  string16 metro_command_line_switches;
};

extern Globals globals;

#endif  
