// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MAC_APP_MODE_COMMON_H_
#define CHROME_COMMON_MAC_APP_MODE_COMMON_H_

#import <Foundation/Foundation.h>

#include "base/files/file_path.h"
#include "base/strings/string16.h"


namespace app_mode {

const AEEventClass kAEChromeAppClass = 'cApp';
const AEEventID kAEChromeAppPing = 'ping';

extern const char kAppShimSocketName[];

extern const char kAppListModeId[];

extern const char kLaunchedByChromeProcessId[];

extern NSString* const kCFBundleDisplayNameKey;

extern NSString* const kLSHasLocalizedDisplayNameKey;

extern NSString* const kBrowserBundleIDKey;

extern NSString* const kCrAppModeShortcutIDKey;

extern NSString* const kCrAppModeShortcutNameKey;

extern NSString* const kCrAppModeShortcutURLKey;

extern NSString* const kCrAppModeUserDataDirKey;

extern NSString* const kCrAppModeProfileDirKey;

extern NSString* const kCrAppModeProfileNameKey;

extern NSString* const kLastRunAppBundlePathPrefsKey;

extern NSString* const kShortcutIdPlaceholder; 
extern NSString* const kShortcutNamePlaceholder; 
extern NSString* const kShortcutURLPlaceholder;
extern NSString* const kShortcutBrowserBundleIDPlaceholder;

const unsigned kCurrentChromeAppModeInfoMajorVersion = 1;
const unsigned kCurrentChromeAppModeInfoMinorVersion = 0;

// written below as v<major>.<minor>. Version-number checking is done by the
struct ChromeAppModeInfo {
 public:
  ChromeAppModeInfo();
  ~ChromeAppModeInfo();

  
  unsigned major_version;  
  unsigned minor_version;  

  
  int argc;  
  char** argv;  

  
  base::FilePath chrome_versioned_path;  

  
  base::FilePath chrome_outer_bundle_path;  

  

  
  base::FilePath app_mode_bundle_path;  

  
  
  std::string app_mode_id;  

  
  base::string16 app_mode_name;  

  
  std::string app_mode_url;  

  
  base::FilePath user_data_dir;

  
  base::FilePath profile_dir;
};

}  

#endif  
