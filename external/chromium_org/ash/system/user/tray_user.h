// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_USER_TRAY_USER_H_
#define ASH_SYSTEM_USER_TRAY_USER_H_

#include "ash/ash_export.h"
#include "ash/session_state_delegate.h"
#include "ash/system/tray/system_tray_item.h"
#include "ash/system/user/user_observer.h"
#include "base/compiler_specific.h"

namespace gfx {
class Rect;
class Point;
}

namespace views {
class ImageView;
class Label;
}

namespace ash {
namespace internal {

namespace tray {
class UserView;
class RoundedImageView;
}

class ASH_EXPORT TrayUser : public SystemTrayItem,
                            public UserObserver {
 public:
  
  
  
  
  TrayUser(SystemTray* system_tray, MultiProfileIndex index);
  virtual ~TrayUser();

  
  enum TestState {
    HIDDEN,               
    SHOWN,                
    HOVERED,              
    ACTIVE,               
    ACTIVE_BUT_DISABLED   
  };
  TestState GetStateForTest() const;

  
  
  bool CanDropWindowHereToTransferToUser(const gfx::Point& point_in_screen);

  
  
  bool TransferWindowToUser(aura::Window* window);

  
  
  gfx::Rect GetUserPanelBoundsInScreenForTest() const;

 private:
  
  virtual views::View* CreateTrayView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDetailedView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyTrayView() OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;
  virtual void DestroyDetailedView() OVERRIDE;
  virtual void UpdateAfterLoginStatusChange(user::LoginStatus status) OVERRIDE;
  virtual void UpdateAfterShelfAlignmentChange(
      ShelfAlignment alignment) OVERRIDE;

  
  virtual void OnUserUpdate() OVERRIDE;
  virtual void OnUserAddedToSession() OVERRIDE;

  void UpdateAvatarImage(user::LoginStatus status);

  
  MultiProfileIndex GetTrayIndex();

  
  int GetTrayItemRadius();

  
  void UpdateLayoutOfItem();

  
  MultiProfileIndex multiprofile_index_;

  tray::UserView* user_;

  
  views::View* layout_view_;
  tray::RoundedImageView* avatar_;
  views::Label* label_;

  DISALLOW_COPY_AND_ASSIGN(TrayUser);
};

}  
}  

#endif  
