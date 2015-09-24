// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SIGNED_SETTINGS_TEMP_STORAGE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SIGNED_SETTINGS_TEMP_STORAGE_H_
#pragma once

#include <string>

#include "base/basictypes.h"

class PrefService;

namespace chromeos {

class SignedSettingsTempStorage {
 public:
  
  static void RegisterPrefs(PrefService* local_state);

  static bool Store(const std::string& name,
                    const std::string& value,
                    PrefService* local_state);
  static bool Retrieve(const std::string& name,
                       std::string* value,
                       PrefService* local_state);

  
  
  static void Finalize(PrefService* local_state);

 private:
  SignedSettingsTempStorage() {}
  DISALLOW_COPY_AND_ASSIGN(SignedSettingsTempStorage);
};

}  

#endif  
