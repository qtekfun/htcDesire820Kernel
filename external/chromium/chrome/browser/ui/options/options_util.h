// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OPTIONS_OPTIONS_UTIL_H_
#define CHROME_BROWSER_UI_OPTIONS_OPTIONS_UTIL_H_
#pragma once

#include "base/basictypes.h"

class Profile;

class OptionsUtil {
 public:
  
  
  
  static void ResetToDefaults(Profile* profile);

  
  
  static bool ResolveMetricsReportingEnabled(bool enabled);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(OptionsUtil);
};

#endif  
