// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_AUTOMATION_EXTENSION_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_AUTOMATION_EXTENSION_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

class Status;
class WebView;

class AutomationExtension {
 public:
  explicit AutomationExtension(scoped_ptr<WebView> web_view);
  ~AutomationExtension();

  
  
  Status CaptureScreenshot(std::string* screenshot);

  
  Status GetWindowPosition(int* x, int* y);

  
  Status SetWindowPosition(int x, int y);

  
  Status GetWindowSize(int* width, int* height);

  
  Status SetWindowSize(int width, int height);

  
  Status MaximizeWindow();

 private:
  Status GetWindowInfo(int* x, int* y, int* width, int* height);
  Status UpdateWindow(const base::DictionaryValue& update_info);

  scoped_ptr<WebView> web_view_;

  DISALLOW_COPY_AND_ASSIGN(AutomationExtension);
};

#endif  
