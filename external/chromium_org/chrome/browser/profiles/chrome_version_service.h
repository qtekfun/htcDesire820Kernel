// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_CHROME_VERSION_SERVICE_H_
#define CHROME_BROWSER_PROFILES_CHROME_VERSION_SERVICE_H_

#include <string>

#include "base/basictypes.h"

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

class ChromeVersionService {
 public:
  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static void SetVersion(PrefService* prefs, const std::string& version);

  
  static std::string GetVersion(PrefService* prefs);

  
  static void OnProfileLoaded(PrefService* prefs, bool is_new_profile);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ChromeVersionService);
};

#endif  
