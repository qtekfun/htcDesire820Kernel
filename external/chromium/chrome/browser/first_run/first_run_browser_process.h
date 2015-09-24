// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_FIRST_RUN_BROWSER_PROCESS_H_
#define CHROME_BROWSER_FIRST_RUN_FIRST_RUN_BROWSER_PROCESS_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/browser_process_impl.h"

class FirstRunBrowserProcess : public BrowserProcessImpl {
 public:
  explicit FirstRunBrowserProcess(const CommandLine& command_line);
  virtual ~FirstRunBrowserProcess();

  
  virtual GoogleURLTracker* google_url_tracker() OVERRIDE;
  virtual IntranetRedirectDetector* intranet_redirect_detector() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(FirstRunBrowserProcess);
};

#endif  
