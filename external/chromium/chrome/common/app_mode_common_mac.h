// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_APP_MODE_COMMON_MAC_H_
#define CHROME_COMMON_APP_MODE_COMMON_MAC_H_
#pragma once

#include <CoreFoundation/CoreFoundation.h>


namespace app_mode {

extern const CFStringRef kAppPrefsID;

extern const CFStringRef kLastRunAppBundlePathPrefsKey;

const unsigned kCurrentChromeAppModeInfoMajorVersion = 1;
const unsigned kCurrentChromeAppModeInfoMinorVersion = 0;

// written below as v<major>.<minor>. Version-number checking is done by the
struct ChromeAppModeInfo {
  
  unsigned major_version;  
  unsigned minor_version;  

  
  int argc;  
  char** argv;  

  
  char* chrome_versioned_path;  

  

  
  char* app_mode_bundle_path;  

  
  
  char* app_mode_id;  

  
  char* app_mode_short_name;  

  
  char* app_mode_name;  

  
  char* app_mode_url;  
};

}  

#endif  
