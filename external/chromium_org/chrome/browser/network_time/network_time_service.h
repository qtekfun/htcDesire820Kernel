// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NETWORK_TIME_NETWORK_TIME_SERVICE_H_
#define CHROME_BROWSER_NETWORK_TIME_NETWORK_TIME_SERVICE_H_

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class NetworkTimeTracker;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

class NetworkTimeService : public BrowserContextKeyedService {
 public:
  explicit NetworkTimeService(Profile* profile);
  virtual ~NetworkTimeService();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  base::Time GetNetworkTime(const base::Time& local_time);

  
  base::Time GetNetworkNow();

  virtual void Shutdown() OVERRIDE;

 private:
  Profile* profile_;
  scoped_ptr<NetworkTimeTracker> network_time_tracker_;
};

#endif  
