// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_MULTI_PROFILE_UMA_H_
#define ASH_MULTI_PROFILE_UMA_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"

namespace ash {

class ASH_EXPORT MultiProfileUMA {
 public:
  
  enum SessionMode {
    SESSION_SINGLE_USER_MODE = 0,
    SESSION_SIDE_BY_SIDE_MODE,
    SESSION_SEPARATE_DESKTOP_MODE,
    NUM_SESSION_MODES
  };

  enum SigninUserAction {
    SIGNIN_USER_BY_TRAY = 0,
    SIGNIN_USER_BY_BROWSER_FRAME,
    NUM_SIGNIN_USER_ACTIONS
  };

  enum SwitchActiveUserAction {
    SWITCH_ACTIVE_USER_BY_TRAY = 0,
    SWITCH_ACTIVE_USER_BY_ACCELERATOR,
    NUM_SWITCH_ACTIVE_USER_ACTIONS
  };

  enum TeleportWindowType {
    TELEPORT_WINDOW_BROWSER = 0,
    TELEPORT_WINDOW_INCOGNITO_BROWSER,
    TELEPORT_WINDOW_V1_APP,
    TELEPORT_WINDOW_V2_APP,
    TELEPORT_WINDOW_PANEL,
    TELEPORT_WINDOW_POPUP,
    TELEPORT_WINDOW_UNKNOWN,
    NUM_TELEPORT_WINDOW_TYPES
  };

  enum TeleportWindowAction {
    TELEPORT_WINDOW_DRAG_AND_DROP = 0,
    TELEPORT_WINDOW_CAPTION_MENU,
    TELEPORT_WINDOW_RETURN_BY_MINIMIZE,
    TELEPORT_WINDOW_RETURN_BY_LAUNCHER,
    NUM_TELEPORT_WINDOW_ACTIONS
  };

  
  static void RecordSessionMode(SessionMode mode);

  
  static void RecordSigninUser(SigninUserAction action);

  
  static void RecordSwitchActiveUser(SwitchActiveUserAction action);

  
  static void RecordTeleportWindowType(TeleportWindowType window_type);

  
  static void RecordTeleportAction(TeleportWindowAction action);

  
  static void RecordUserCount(int number_of_users);

  
  static void RecordDiscardedTab(int number_of_users);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(MultiProfileUMA);
};

}  

#endif  
