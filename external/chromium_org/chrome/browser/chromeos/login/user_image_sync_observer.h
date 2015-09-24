// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_SYNC_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_IMAGE_SYNC_OBSERVER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/prefs/pref_service_syncable_observer.h"
#include "content/public/browser/notification_observer.h"

class PrefChangeRegistrar;
class PrefServiceSyncable;
class Profile;
namespace chromeos {
class User;
}
namespace content {
class NotificationRegistrar;
}
namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chromeos {

class UserImageSyncObserver: public PrefServiceSyncableObserver,
                             public content::NotificationObserver {
 public:
  class Observer {
   public:
    
    
    
    virtual void OnInitialSync(bool local_image_updated) = 0;
    virtual ~Observer();
  };

 public:
  explicit UserImageSyncObserver(const User* user);
  virtual ~UserImageSyncObserver();

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  bool is_synced() const { return is_synced_; }

  
  void AddObserver(Observer* observer);
  
  void RemoveObserver(Observer* observer);

 private:
  
  virtual void OnIsSyncingChanged() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnProfileGained(Profile* profile);

  
  
  void OnInitialSync();

  
  void OnPreferenceChanged(const std::string& pref_name);

  
  void UpdateSyncedImageFromLocal();

  
  void UpdateLocalImageFromSynced();

  
  bool GetSyncedImageIndex(int* result);

  
  bool CanUpdateLocalImageNow();

  const User* user_;
  scoped_ptr<PrefChangeRegistrar> pref_change_registrar_;
  scoped_ptr<content::NotificationRegistrar> notification_registrar_;
  PrefServiceSyncable* prefs_;
  bool is_synced_;
  
  bool local_image_changed_;
  ObserverList<Observer> observer_list_;
};

}  

#endif  

