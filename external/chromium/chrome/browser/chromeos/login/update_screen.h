// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_UPDATE_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_UPDATE_SCREEN_H_
#pragma once

#include "base/timer.h"
#include "chrome/browser/chromeos/cros/update_library.h"
#include "chrome/browser/chromeos/login/update_view.h"
#include "chrome/browser/chromeos/login/view_screen.h"

namespace chromeos {

class UpdateController {
 public:
  
  virtual void StartUpdate() = 0;
  
  virtual void CancelUpdate() = 0;
};

class UpdateScreen: public DefaultViewScreen<chromeos::UpdateView>,
                    public UpdateLibrary::Observer,
                    public UpdateController {
 public:
  explicit UpdateScreen(WizardScreenDelegate* delegate);
  virtual ~UpdateScreen();

  
  virtual void UpdateStatusChanged(UpdateLibrary* library);

  
  virtual void StartUpdate();
  virtual void CancelUpdate();

  
  virtual void Show();

  enum ExitReason {
     REASON_UPDATE_CANCELED,
     REASON_UPDATE_INIT_FAILED,
     REASON_UPDATE_NON_CRITICAL,
     REASON_UPDATE_ENDED
  };

  
  virtual void ExitUpdate(ExitReason reason);

  
  int reboot_check_delay() const { return reboot_check_delay_; }
  void SetRebootCheckDelay(int seconds);

  
  
  bool HasCriticalUpdate();

  
  
  void SetAllUpdatesCritical(bool is_critical);

  
  static bool HasInstance(UpdateScreen* inst);

 private:
  
  void OnWaitForRebootTimeElapsed();

  
  void MakeSureScreenIsShown();

  
  
  base::OneShotTimer<UpdateScreen> reboot_timer_;

  
  typedef std::set<UpdateScreen*> InstanceSet;
  static InstanceSet& GetInstanceSet();

  
  bool checking_for_update_;

  
  
  
  int reboot_check_delay_;

  
  bool is_downloading_update_;

  
  bool is_all_updates_critical_;

  DISALLOW_COPY_AND_ASSIGN(UpdateScreen);
};

}  

#endif  
