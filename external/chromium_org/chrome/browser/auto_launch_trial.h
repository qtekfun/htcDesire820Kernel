// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTO_LAUNCH_TRIAL_H_
#define CHROME_BROWSER_AUTO_LAUNCH_TRIAL_H_

#include <string>


extern const char kAutoLaunchTrialName[];
extern const char kAutoLaunchTrialAutoLaunchGroup[];
extern const char kAutoLaunchTrialControlGroup[];

namespace auto_launch_trial {

enum InfobarMetricResponse {
  INFOBAR_CUT_IT_OUT = 0,
  INFOBAR_OK,
  INFOBAR_IGNORE,
};

bool IsInAutoLaunchGroup();

bool IsInExperimentGroup(const std::string& brand_code);

bool IsInControlGroup(const std::string& brand_code);

}  

#endif  
