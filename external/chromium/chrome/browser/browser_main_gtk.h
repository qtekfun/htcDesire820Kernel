// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BROWSER_MAIN_GTK_H_
#define CHROME_BROWSER_BROWSER_MAIN_GTK_H_
#pragma once

#include "base/compiler_specific.h"
#include "chrome/browser/browser_main_posix.h"

class BrowserMainPartsGtk : public BrowserMainPartsPosix {
 public:
  explicit BrowserMainPartsGtk(const MainFunctionParams& parameters)
      : BrowserMainPartsPosix(parameters) {}

  virtual void PreEarlyInitialization() OVERRIDE;

 private:
  void DetectRunningAsRoot();
  void SetupSandbox();
};

void SetBrowserX11ErrorHandlers();

#endif  
