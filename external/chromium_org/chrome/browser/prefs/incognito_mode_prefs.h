// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_INCOGNITO_MODE_PREFS_H_
#define CHROME_BROWSER_PREFS_INCOGNITO_MODE_PREFS_H_

#include "base/basictypes.h"

class CommandLine;
class PrefService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

class IncognitoModePrefs {
 public:
  
  
  enum Availability {
    
    
    ENABLED = 0,
    
    
    DISABLED,
    
    
    FORCED,

    AVAILABILITY_NUM_TYPES
  };

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  static Availability GetAvailability(const PrefService* prefs);

  
  
  static void SetAvailability(PrefService* prefs,
                              const Availability availability);

  
  
  
  static bool IntToAvailability(int in_value, Availability* out_value);

  
  static bool ShouldLaunchIncognito(const CommandLine& command_line,
                                    const PrefService* prefs);

  
  
  
  static bool CanOpenBrowser(Profile* profile);

 private:
  
  
  static bool ArePlatformParentalControlsEnabled();

  DISALLOW_IMPLICIT_CONSTRUCTORS(IncognitoModePrefs);
};

#endif  
