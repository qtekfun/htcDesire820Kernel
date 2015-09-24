// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_MAIN_MAC_H_
#define CHROME_BROWSER_CHROME_BROWSER_MAIN_MAC_H_

#include "chrome/browser/chrome_browser_main_posix.h"

class ChromeBrowserMainPartsMac : public ChromeBrowserMainPartsPosix {
 public:
  explicit ChromeBrowserMainPartsMac(
      const content::MainFunctionParams& parameters);
  virtual ~ChromeBrowserMainPartsMac();

  
  virtual void PreEarlyInitialization() OVERRIDE;
  virtual void PreMainMessageLoopStart() OVERRIDE;
  virtual void PostProfileInit() OVERRIDE;

  
  
  static void DidEndMainMessageLoop();

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserMainPartsMac);
};

#endif  
