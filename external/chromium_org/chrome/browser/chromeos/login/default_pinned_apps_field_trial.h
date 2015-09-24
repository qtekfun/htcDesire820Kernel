// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_PINNED_APPS_FIELD_TRIAL_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_DEFAULT_PINNED_APPS_FIELD_TRIAL_H_

#include <string>

namespace base {
class ListValue;
}

class PrefRegistrySimple;
class PrefService;

namespace chromeos {
namespace default_pinned_apps_field_trial {

void RegisterPrefs(PrefRegistrySimple* registry);

void MigratePrefs(PrefService* local_state);

}  
}  

#endif  
