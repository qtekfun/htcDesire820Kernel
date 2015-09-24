// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_APP_SHIM_APP_SHIM_LAUNCH_H_
#define APPS_APP_SHIM_APP_SHIM_LAUNCH_H_

namespace apps {

enum AppShimLaunchType {
  
  
  APP_SHIM_LAUNCH_REGISTER_ONLY = 0,
  
  APP_SHIM_LAUNCH_NORMAL,
  
  APP_SHIM_LAUNCH_NUM_TYPES
};

enum AppShimLaunchResult {
  
  APP_SHIM_LAUNCH_SUCCESS = 0,
  
  APP_SHIM_LAUNCH_DUPLICATE_HOST,
  
  APP_SHIM_LAUNCH_PROFILE_NOT_FOUND,
  
  APP_SHIM_LAUNCH_APP_NOT_FOUND,
  
  APP_SHIM_LAUNCH_NUM_RESULTS
};

enum AppShimFocusType {
  
  APP_SHIM_FOCUS_NORMAL = 0,
  
  APP_SHIM_FOCUS_REOPEN,
  
  APP_SHIM_FOCUS_OPEN_FILES,
  
  APP_SHIM_FOCUS_NUM_TYPES
};

}  

#endif  
