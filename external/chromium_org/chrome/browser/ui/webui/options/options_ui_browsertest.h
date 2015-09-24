// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_UI_BROWSERTEST_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_UI_BROWSERTEST_H_

#include "base/basictypes.h"
#include "chrome/test/base/in_process_browser_test.h"

namespace options {

class OptionsUIBrowserTest : public InProcessBrowserTest {
 public:
  OptionsUIBrowserTest();

  
  void NavigateToSettings();

  
  void VerifyNavbar();

  
  
  
  void VerifyTitle();
};

}  

#endif  
