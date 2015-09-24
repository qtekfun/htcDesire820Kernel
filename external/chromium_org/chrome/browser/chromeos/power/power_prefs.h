// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POWER_POWER_PREFS_H_
#define CHROME_BROWSER_CHROMEOS_POWER_POWER_PREFS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefChangeRegistrar;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chromeos {

class PowerPolicyController;

class PowerPrefs : public content::NotificationObserver {
 public:
  explicit PowerPrefs(PowerPolicyController* power_policy_controller);
  virtual ~PowerPrefs();

  
  static void RegisterUserProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry);

  
  static void RegisterLoginProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void UpdatePowerPolicyFromPrefs();

 private:
  friend class PowerPrefsTest;

  
  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  void SetProfile(Profile* profile);

  PowerPolicyController* power_policy_controller_;  

  content::NotificationRegistrar notification_registrar_;

  Profile* profile_;  
  scoped_ptr<PrefChangeRegistrar> pref_change_registrar_;

  DISALLOW_COPY_AND_ASSIGN(PowerPrefs);
};

}  

#endif  
