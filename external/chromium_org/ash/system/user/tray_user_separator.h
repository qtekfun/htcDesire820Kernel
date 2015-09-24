// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_USER_TRAY_USER_SEPARATOR_H_
#define ASH_SYSTEM_USER_TRAY_USER_SEPARATOR_H_

#include "ash/ash_export.h"
#include "ash/system/tray/system_tray_item.h"

namespace ash {
namespace internal {

class ASH_EXPORT TrayUserSeparator : public SystemTrayItem {
 public:
  explicit TrayUserSeparator(SystemTray* system_tray);
  virtual ~TrayUserSeparator() {}

  
  bool separator_shown() { return separator_shown_; }

 private:
  
  virtual views::View* CreateTrayView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDetailedView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyTrayView() OVERRIDE {}
  virtual void DestroyDefaultView() OVERRIDE;
  virtual void DestroyDetailedView() OVERRIDE {}
  virtual void UpdateAfterLoginStatusChange(
      user::LoginStatus status) OVERRIDE {}
  virtual void UpdateAfterShelfAlignmentChange(
      ShelfAlignment alignment) OVERRIDE {}

  
  bool separator_shown_;

  DISALLOW_COPY_AND_ASSIGN(TrayUserSeparator);
};

}  
}  

#endif  
