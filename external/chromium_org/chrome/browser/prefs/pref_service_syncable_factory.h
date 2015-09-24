// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_SERVICE_SYNCABLE_FACTORY_H_
#define CHROME_BROWSER_PREFS_PREF_SERVICE_SYNCABLE_FACTORY_H_

#include "base/prefs/pref_service_factory.h"

class CommandLine;
class PrefServiceSyncable;

namespace policy {
class PolicyService;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PrefServiceSyncableFactory : public base::PrefServiceFactory {
 public:
  PrefServiceSyncableFactory();
  virtual ~PrefServiceSyncableFactory();

#if defined(ENABLE_CONFIGURATION_POLICY)
  
  void SetManagedPolicies(policy::PolicyService* service);
  void SetRecommendedPolicies(policy::PolicyService* service);
#endif

  
  void SetCommandLine(CommandLine* command_line);

  scoped_ptr<PrefServiceSyncable> CreateSyncable(
      user_prefs::PrefRegistrySyncable* registry);

 private:
  DISALLOW_COPY_AND_ASSIGN(PrefServiceSyncableFactory);
};

#endif  
