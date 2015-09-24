// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_NOTIFIER_H_
#define CHROME_BROWSER_PREFS_PREF_NOTIFIER_H_
#pragma once

#include <string>

class PrefNotifier {
 public:
  virtual ~PrefNotifier() {}

  
  
  virtual void OnPreferenceChanged(const std::string& pref_name) = 0;

  
  virtual void OnInitializationCompleted() = 0;
};

#endif  
