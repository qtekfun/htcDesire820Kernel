// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_OBSERVER_H_
#define BASE_PREFS_PREF_OBSERVER_H_

#include <string>

class PrefService;

class PrefObserver {
 public:
  virtual void OnPreferenceChanged(PrefService* service,
                                   const std::string& pref_name) = 0;
};

#endif  
