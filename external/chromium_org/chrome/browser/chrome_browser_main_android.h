// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_MAIN_ANDROID_H_
#define CHROME_BROWSER_CHROME_BROWSER_MAIN_ANDROID_H_

#include "chrome/browser/chrome_browser_main.h"

namespace breakpad {
class CrashDumpManager;
}

class ChromeBrowserMainPartsAndroid : public ChromeBrowserMainParts {
 public:
  explicit ChromeBrowserMainPartsAndroid(
      const content::MainFunctionParams& parameters);
  virtual ~ChromeBrowserMainPartsAndroid();

  
  virtual void PreProfileInit() OVERRIDE;
  virtual void PreEarlyInitialization() OVERRIDE;

  
  virtual void ShowMissingLocaleMessageBox() OVERRIDE;

 private:
  scoped_ptr<base::MessageLoop> main_message_loop_;
  scoped_ptr<breakpad::CrashDumpManager> crash_dump_manager_;

  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserMainPartsAndroid);
};

#endif  