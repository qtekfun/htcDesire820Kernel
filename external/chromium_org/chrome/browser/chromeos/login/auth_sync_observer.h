// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_SYNC_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTH_SYNC_OBSERVER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class Profile;

namespace chromeos {

class AuthSyncObserver : public BrowserContextKeyedService,
                         public ProfileSyncServiceObserver {
 public:
  explicit AuthSyncObserver(Profile* user_profile);
  virtual ~AuthSyncObserver();

  void StartObserving();

 private:
  friend class AuthSyncObserverFactory;

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnStateChanged() OVERRIDE;

  
  void OnSupervisedTokenLoaded(const std::string& token);

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(AuthSyncObserver);
};

}  

#endif  
