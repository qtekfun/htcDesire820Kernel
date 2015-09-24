// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_SYSTEM_TRAY_ITEM_H_
#define ASH_SYSTEM_TRAY_SYSTEM_TRAY_ITEM_H_

#include "ash/ash_export.h"
#include "ash/shelf/shelf_types.h"
#include "ash/system/user/login_status.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace views {
class View;
}

namespace ash {

class SystemTray;

namespace internal {
class TrayItemView;
}

class ASH_EXPORT SystemTrayItem {
 public:
  explicit SystemTrayItem(SystemTray* system_tray);
  virtual ~SystemTrayItem();

  
  

  
  
  
  
  
  
  virtual views::View* CreateTrayView(user::LoginStatus status);

  
  
  
  virtual views::View* CreateDefaultView(user::LoginStatus status);

  
  virtual views::View* CreateDetailedView(user::LoginStatus status);

  
  
  virtual views::View* CreateNotificationView(user::LoginStatus status);

  
  
  
  virtual void DestroyTrayView();
  virtual void DestroyDefaultView();
  virtual void DestroyDetailedView();
  virtual void DestroyNotificationView();

  
  
  
  
  virtual void UpdateAfterLoginStatusChange(user::LoginStatus status);

  
  
  virtual void UpdateAfterShelfAlignmentChange(ShelfAlignment alignment);

  
  
  
  
  
  
  void TransitionDetailedView();

  
  
  
  
  
  void PopupDetailedView(int for_seconds, bool activate);

  
  
  
  void SetDetailedViewCloseDelay(int for_seconds);

  
  void HideDetailedView();

  
  void ShowNotificationView();

  
  void HideNotificationView();

  
  virtual bool ShouldHideArrow() const;

  
  
  virtual bool ShouldShowLauncher() const;

  
  SystemTray* system_tray() const { return system_tray_; }

  bool restore_focus() const { return restore_focus_; }
  void set_restore_focus(bool restore_focus) {
    restore_focus_ = restore_focus;
  }

 private:
  SystemTray* system_tray_;
  bool restore_focus_;

  DISALLOW_COPY_AND_ASSIGN(SystemTrayItem);
};

}  

#endif  
