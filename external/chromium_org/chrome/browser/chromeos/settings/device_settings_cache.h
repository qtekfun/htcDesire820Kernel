// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_CACHE_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_DEVICE_SETTINGS_CACHE_H_

namespace enterprise_management {
class PolicyData;
}

class PrefService;
class PrefRegistrySimple;

namespace chromeos {

namespace device_settings_cache {
void RegisterPrefs(PrefRegistrySimple* registry);

bool Store(const enterprise_management::PolicyData &policy,
           PrefService* local_state);

bool Retrieve(enterprise_management::PolicyData *policy,
              PrefService* local_state);

void Finalize(PrefService* local_state);
}  

}  

#endif  
