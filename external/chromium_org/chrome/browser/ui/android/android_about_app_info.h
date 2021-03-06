// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ANDROID_ANDROID_ABOUT_APP_INFO_H_
#define CHROME_BROWSER_UI_ANDROID_ANDROID_ABOUT_APP_INFO_H_

#include <string>

class AndroidAboutAppInfo {
 public:
  
  static std::string GetOsInfo();

  
  static std::string GetJavaScriptVersion();
};

#endif  
