// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_NOTIFIER_H_
#define BASE_PREFS_PREF_NOTIFIER_H_

#include <string>

class PrefNotifier {
 public:
  virtual ~PrefNotifier() {}

  
  
  virtual void OnPreferenceChanged(const std::string& pref_name) = 0;

  
  virtual void OnInitializationCompleted(bool succeeded) = 0;
};

#endif  
