// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LANGUAGE_PREFERENCES_H_
#define CHROME_BROWSER_CHROMEOS_LANGUAGE_PREFERENCES_H_

class PrefRegistrySimple;


namespace chromeos {
namespace language_prefs {

extern const char kGeneralSectionName[];
extern const char kPreloadEnginesConfigName[];

extern const int kXkbAutoRepeatDelayInMs;
extern const int kXkbAutoRepeatIntervalInMs;

extern const char kPreferredKeyboardLayout[];

void RegisterPrefs(PrefRegistrySimple* registry);

}  
}  

#endif  
