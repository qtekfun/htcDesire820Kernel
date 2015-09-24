// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_AUTOMATIC_REBOOT_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_AUTOMATIC_REBOOT_MANAGER_H_

#include "ash/wm/user_activity_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chromeos/dbus/power_manager_client.h"
#include "chromeos/dbus/update_engine_client.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefRegistrySimple;

namespace base {
class TickClock;
}

namespace chromeos {
namespace system {

class AutomaticRebootManagerObserver;

class AutomaticRebootManager : public PowerManagerClient::Observer,
                               public UpdateEngineClient::Observer,
                               public ash::UserActivityObserver,
                               public content::NotificationObserver {
 public:
  
  
  
  struct SystemEventTimes {
    SystemEventTimes();
    SystemEventTimes(const base::TimeDelta& uptime,
                     const base::TimeDelta& update_reboot_needed_uptime);

    bool has_boot_time;
    base::TimeTicks boot_time;

    bool has_update_reboot_needed_time;
    base::TimeTicks update_reboot_needed_time;
  };

  explicit AutomaticRebootManager(scoped_ptr<base::TickClock> clock);
  virtual ~AutomaticRebootManager();

  void AddObserver(AutomaticRebootManagerObserver* observer);
  void RemoveObserver(AutomaticRebootManagerObserver* observer);

  
  virtual void SystemResumed(const base::TimeDelta& sleep_duration) OVERRIDE;

  
  virtual void UpdateStatusChanged(
      const UpdateEngineClient::Status& status) OVERRIDE;

  
  virtual void OnUserActivity(const ui::Event* event) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  friend class AutomaticRebootManagerBasicTest;

  
  
  void Init(const SystemEventTimes& system_event_times);

  
  
  void Reschedule();

  
  void RequestReboot();

  
  
  
  
  void MaybeReboot(bool ignore_session);

  
  void Reboot();

  
  scoped_ptr<base::TickClock> clock_;

  PrefChangeRegistrar local_state_registrar_;

  content::NotificationRegistrar notification_registrar_;

  
  
  scoped_ptr<base::OneShotTimer<AutomaticRebootManager> >
      login_screen_idle_timer_;

  
  bool have_boot_time_;
  base::TimeTicks boot_time_;

  
  
  bool have_update_reboot_needed_time_;
  base::TimeTicks update_reboot_needed_time_;

  
  bool reboot_requested_;

  
  scoped_ptr<base::OneShotTimer<AutomaticRebootManager> > grace_start_timer_;
  scoped_ptr<base::OneShotTimer<AutomaticRebootManager> > grace_end_timer_;

  base::WeakPtrFactory<AutomaticRebootManager> weak_ptr_factory_;

  ObserverList<AutomaticRebootManagerObserver, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(AutomaticRebootManager);
};

}  
}  

#endif  
