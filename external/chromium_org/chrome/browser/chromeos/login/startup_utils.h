// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_STARTUP_UTILS_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_STARTUP_UTILS_H_

#include <string>

class PrefRegistrySimple;

namespace chromeos {

class StartupUtils {
 public:
  
  static bool IsEulaAccepted();

  
  static bool IsOobeCompleted();

  
  static void MarkEulaAccepted();

  
  static void MarkOobeCompleted();

  
  static bool IsDeviceRegistered();

  
  static void MarkDeviceRegistered();

  
  static std::string GetInitialLocale();

  
  static void SetInitialLocale(const std::string& locale);

  
  static void RegisterPrefs(PrefRegistrySimple* registry);
};

}  

#endif  
