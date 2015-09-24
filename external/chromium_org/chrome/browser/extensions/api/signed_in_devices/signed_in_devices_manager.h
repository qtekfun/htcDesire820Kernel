// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SIGNED_IN_DEVICES_SIGNED_IN_DEVICES_MANAGER_H__
#define CHROME_BROWSER_EXTENSIONS_API_SIGNED_IN_DEVICES_SIGNED_IN_DEVICES_MANAGER_H__

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/sync/glue/synced_device_tracker.h"
#include "extensions/browser/event_router.h"

class Profile;
namespace content {
  class NotificationDetails;
  class NotificationObserver;
  class NotificationRegistrar;
}  

namespace extensions {
class ProfileKeyedAPI;

struct EventListenerInfo;

class SignedInDevicesChangeObserver
    : public browser_sync::SyncedDeviceTracker::Observer {
 public:
  SignedInDevicesChangeObserver(const std::string& extension_id,
                                Profile* profile);
  virtual ~SignedInDevicesChangeObserver();

  virtual void OnDeviceInfoChange() OVERRIDE;

  const std::string& extension_id() {
    return extension_id_;
  }

 private:
  std::string extension_id_;
  Profile* const profile_;
  content::NotificationRegistrar registrar_;
};

class SignedInDevicesManager
    : public ProfileKeyedAPI,
      public content::NotificationObserver,
      public EventRouter::Observer {
 public:
  
  SignedInDevicesManager();
  explicit SignedInDevicesManager(Profile* profile);
  virtual ~SignedInDevicesManager();

  
  static ProfileKeyedAPIFactory<SignedInDevicesManager>* GetFactoryInstance();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;
  virtual void OnListenerRemoved(const EventListenerInfo& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<SignedInDevicesManager>;

  
  static const char* service_name() {
    return "SignedInDevicesManager";
  }
  static const bool kServiceHasOwnInstanceInIncognito = true;

  void RemoveChangeObserverForExtension(const std::string& extension_id);

  Profile* const profile_;
  content::NotificationRegistrar registrar_;
  ScopedVector<SignedInDevicesChangeObserver> change_observers_;

  FRIEND_TEST_ALL_PREFIXES(SignedInDevicesManager, UpdateListener);

  DISALLOW_COPY_AND_ASSIGN(SignedInDevicesManager);
};

}  

#endif  

