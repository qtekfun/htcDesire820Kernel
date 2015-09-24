// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_STATUS_AREA_WIDGET_H_
#define ASH_SYSTEM_STATUS_AREA_WIDGET_H_

#include "ash/ash_export.h"
#include "ash/shelf/shelf_types.h"
#include "ash/system/user/login_status.h"
#include "ui/views/widget/widget.h"

namespace ash {

class ShellDelegate;
class SystemTray;
class WebNotificationTray;

namespace internal {

class LogoutButtonTray;
class StatusAreaWidgetDelegate;

class ASH_EXPORT StatusAreaWidget : public views::Widget {
 public:
  static const char kNativeViewName[];

  explicit StatusAreaWidget(aura::Window* status_container);
  virtual ~StatusAreaWidget();

  
  void CreateTrayViews();

  
  
  void Shutdown();

  
  void SetShelfAlignment(ShelfAlignment alignment);

  
  void SetHideSystemNotifications(bool hide);

  
  
  
  void UpdateAfterLoginStatusChange(user::LoginStatus login_status);

  internal::StatusAreaWidgetDelegate* status_area_widget_delegate() {
    return status_area_widget_delegate_;
  }
  SystemTray* system_tray() { return system_tray_; }
  WebNotificationTray* web_notification_tray() {
    return web_notification_tray_;
  }

  user::LoginStatus login_status() const { return login_status_; }

  
  
  
  bool ShouldShowLauncher() const;

  
  bool IsMessageBubbleShown() const;

  
  virtual void OnNativeWidgetActivationChanged(bool active) OVERRIDE;

 private:
  void AddSystemTray();
  void AddWebNotificationTray();
  void AddLogoutButtonTray();

  
  internal::StatusAreaWidgetDelegate* status_area_widget_delegate_;
  SystemTray* system_tray_;
  WebNotificationTray* web_notification_tray_;
  LogoutButtonTray* logout_button_tray_;
  user::LoginStatus login_status_;

  DISALLOW_COPY_AND_ASSIGN(StatusAreaWidget);
};

}  
}  

#endif  
