// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APPS_APP_LAUNCH_FOR_METRO_RESTART_WIN_H_
#define CHROME_BROWSER_APPS_APP_LAUNCH_FOR_METRO_RESTART_WIN_H_

#include <string>

#include "base/basictypes.h"

class PrefRegistrySimple;
class Profile;

namespace app_metro_launch {

void HandleAppLaunchForMetroRestart(Profile* profile);

void SetAppLaunchForMetroRestart(Profile* profile,
                                 const std::string& extension_id);

void RegisterPrefs(PrefRegistrySimple* registry);

}  

#endif  
