// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_SESSION_STARTUP_PREF_H__
#define CHROME_BROWSER_PREFS_SESSION_STARTUP_PREF_H__

#include <vector>

#include "url/gurl.h"

class PrefService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

struct SessionStartupPref {
  enum Type {
    
    DEFAULT,

    
    HOMEPAGE,

    
    LAST,

    
    
    URLS,

    
    TYPE_COUNT
  };

  
  
  
  
  static const int kPrefValueHomePage = 0;  
  static const int kPrefValueLast = 1;
  static const int kPrefValueURLs = 4;
  static const int kPrefValueNewTab = 5;
  static const int kPrefValueMax = 6;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static Type GetDefaultStartupType();

  
  static void SetStartupPref(Profile* profile, const SessionStartupPref& pref);
  static void SetStartupPref(PrefService* prefs,
                             const SessionStartupPref& pref);
  static SessionStartupPref GetStartupPref(Profile* profile);
  static SessionStartupPref GetStartupPref(PrefService* prefs);

  
  
  
  static void MigrateIfNecessary(PrefService* prefs);

  
  
  static void MigrateMacDefaultPrefIfNecessary(PrefService* prefs);

  
  static bool TypeIsManaged(PrefService* prefs);
  static bool URLsAreManaged(PrefService* prefs);

  
  static bool TypeIsDefault(PrefService* prefs);

  
  static SessionStartupPref::Type PrefValueToType(int pref_value);

  explicit SessionStartupPref(Type type);

  ~SessionStartupPref();

  
  Type type;

  
  std::vector<GURL> urls;
};

#endif  
