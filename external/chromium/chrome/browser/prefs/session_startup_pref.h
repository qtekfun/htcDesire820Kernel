// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_SESSION_STARTUP_PREF_H__
#define CHROME_BROWSER_PREFS_SESSION_STARTUP_PREF_H__
#pragma once

#include <vector>

#include "googleurl/src/gurl.h"

class PrefService;
class Profile;

struct SessionStartupPref {
  enum Type {
    
    DEFAULT,

    
    LAST,

    
    
    URLS
  };

  static void RegisterUserPrefs(PrefService* prefs);

  
  static void SetStartupPref(Profile* profile, const SessionStartupPref& pref);
  static void SetStartupPref(PrefService* prefs,
                             const SessionStartupPref& pref);
  static SessionStartupPref GetStartupPref(Profile* profile);
  static SessionStartupPref GetStartupPref(PrefService* prefs);

  
  static bool TypeIsManaged(PrefService* prefs);
  static bool URLsAreManaged(PrefService* prefs);

  SessionStartupPref();

  explicit SessionStartupPref(Type type);

  ~SessionStartupPref();

  
  Type type;

  
  std::vector<GURL> urls;
};

#endif  
