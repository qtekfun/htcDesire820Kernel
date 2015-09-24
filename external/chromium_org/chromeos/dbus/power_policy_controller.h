// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_POWER_POLICY_CONTROLLER_H_
#define CHROMEOS_DBUS_POWER_POLICY_CONTROLLER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_thread_manager_observer.h"
#include "chromeos/dbus/power_manager/policy.pb.h"
#include "chromeos/dbus/power_manager_client.h"

namespace chromeos {

class DBusThreadManager;

class CHROMEOS_EXPORT PowerPolicyController
    : public DBusThreadManagerObserver,
      public PowerManagerClient::Observer {
 public:
  
  enum Action {
    ACTION_SUSPEND      = 0,
    ACTION_STOP_SESSION = 1,
    ACTION_SHUT_DOWN    = 2,
    ACTION_DO_NOTHING   = 3,
  };

  
  struct PrefValues {
    PrefValues();

    int ac_screen_dim_delay_ms;
    int ac_screen_off_delay_ms;
    int ac_screen_lock_delay_ms;
    int ac_idle_warning_delay_ms;
    int ac_idle_delay_ms;
    int battery_screen_dim_delay_ms;
    int battery_screen_off_delay_ms;
    int battery_screen_lock_delay_ms;
    int battery_idle_warning_delay_ms;
    int battery_idle_delay_ms;
    Action ac_idle_action;
    Action battery_idle_action;
    Action lid_closed_action;
    bool use_audio_activity;
    bool use_video_activity;
    double ac_brightness_percent;
    double battery_brightness_percent;
    bool allow_screen_wake_locks;
    bool enable_screen_lock;
    double presentation_screen_dim_delay_factor;
    double user_activity_screen_dim_delay_factor;
    bool wait_for_initial_user_activity;
  };

  
  static std::string GetPolicyDebugString(
      const power_manager::PowerManagementPolicy& policy);

  
  
  
  
  static const int kScreenLockAfterOffDelayMs;

  PowerPolicyController();
  virtual ~PowerPolicyController();

  void Init(DBusThreadManager* manager);

  
  void ApplyPrefs(const PrefValues& values);

  
  void ClearPrefs();

  
  
  
  
  
  int AddScreenWakeLock(const std::string& reason);
  int AddSystemWakeLock(const std::string& reason);

  
  
  void RemoveWakeLock(int id);

  
  virtual void OnDBusThreadManagerDestroying(DBusThreadManager* manager)
      OVERRIDE;

  
  virtual void PowerManagerRestarted() OVERRIDE;

 private:
  friend class PowerPrefsTest;

  typedef std::map<int, std::string> WakeLockMap;

  
  void SendCurrentPolicy();

  
  void SendEmptyPolicy();

  DBusThreadManager* manager_;  
  PowerManagerClient* client_;  

  
  power_manager::PowerManagementPolicy prefs_policy_;

  
  bool prefs_were_set_;

  
  
  
  WakeLockMap screen_wake_locks_;
  WakeLockMap system_wake_locks_;

  
  
  bool honor_screen_wake_locks_;

  
  int next_wake_lock_id_;

  DISALLOW_COPY_AND_ASSIGN(PowerPolicyController);
};

}  

#endif  
