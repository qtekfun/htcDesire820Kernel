// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_MAIN_RUNNER_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_MAIN_RUNNER_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

struct MainFunctionParams;

class CONTENT_EXPORT BrowserMainRunner {
 public:
  virtual ~BrowserMainRunner() {}

  
  static BrowserMainRunner* Create();

  
  
  virtual int Initialize(const content::MainFunctionParams& parameters) = 0;

  
  virtual int Run() = 0;

  
  virtual void Shutdown() = 0;
};

}  

#endif  
