// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_UPDATE_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_UPDATE_SCREEN_H_

#include <set>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/login/screens/update_screen_actor.h"
#include "chrome/browser/chromeos/login/screens/wizard_screen.h"
#include "chrome/browser/chromeos/net/network_portal_detector.h"
#include "chromeos/dbus/update_engine_client.h"

namespace chromeos {

class ErrorScreen;
class NetworkState;
class ScreenObserver;

class UpdateScreen: public UpdateEngineClient::Observer,
                    public UpdateScreenActor::Delegate,
                    public WizardScreen,
                    public NetworkPortalDetector::Observer {
 public:
  UpdateScreen(ScreenObserver* screen_observer, UpdateScreenActor* actor);
  virtual ~UpdateScreen();

  
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual std::string GetName() const OVERRIDE;

  
  virtual void CancelUpdate() OVERRIDE;
  virtual void OnActorDestroyed(UpdateScreenActor* actor) OVERRIDE;
  virtual void OnConnectToNetworkRequested(
      const std::string& service_path) OVERRIDE;

  
  virtual void StartNetworkCheck();

  
  int reboot_check_delay() const { return reboot_check_delay_; }
  void SetRebootCheckDelay(int seconds);

  
  static bool HasInstance(UpdateScreen* inst);

  void SetIgnoreIdleStatus(bool ignore_idle_status);

  enum ExitReason {
     REASON_UPDATE_CANCELED = 0,
     REASON_UPDATE_INIT_FAILED,
     REASON_UPDATE_NON_CRITICAL,
     REASON_UPDATE_ENDED
  };
  
  virtual void ExitUpdate(ExitReason reason);

  
  virtual void UpdateStatusChanged(
      const UpdateEngineClient::Status& status) OVERRIDE;

  
  virtual void OnPortalDetectionCompleted(
      const NetworkState* network,
      const NetworkPortalDetector::CaptivePortalState& state) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(UpdateScreenTest, TestBasic);
  FRIEND_TEST_ALL_PREFIXES(UpdateScreenTest, TestUpdateAvailable);
  FRIEND_TEST_ALL_PREFIXES(UpdateScreenTest, TestAPReselection);

  enum State {
    STATE_IDLE = 0,
    STATE_FIRST_PORTAL_CHECK,
    STATE_UPDATE,
    STATE_ERROR
  };

  
  
  void UpdateDownloadingStats(const UpdateEngineClient::Status& status);

  
  
  bool HasCriticalUpdate();

  
  void OnWaitForRebootTimeElapsed();

  
  void MakeSureScreenIsShown();

  
  ErrorScreen* GetErrorScreen();

  void StartUpdateCheck();
  void ShowErrorMessage();
  void HideErrorMessage();
  void UpdateErrorMessage(
      const NetworkState* network,
      const NetworkPortalDetector::CaptivePortalStatus status);
  
  
  base::OneShotTimer<UpdateScreen> reboot_timer_;

  
  typedef std::set<UpdateScreen*> InstanceSet;
  static InstanceSet& GetInstanceSet();

  
  State state_;

  
  
  
  int reboot_check_delay_;

  
  bool is_checking_for_update_;
  
  bool is_downloading_update_;
  
  
  bool is_ignore_update_deadlines_;
  
  bool is_shown_;
  
  bool ignore_idle_status_;

  
  UpdateScreenActor* actor_;

  
  base::Time download_start_time_;
  double download_start_progress_;

  
  base::Time download_last_time_;
  double download_last_progress_;

  bool is_download_average_speed_computed_;
  double download_average_speed_;

  
  
  bool is_first_detection_notification_;

  
  
  bool is_first_portal_notification_;

  base::WeakPtrFactory<UpdateScreen> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(UpdateScreen);
};

}  

#endif  
