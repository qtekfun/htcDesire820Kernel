// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_APP_CONTENT_MAIN_RUNNER_H_
#define CONTENT_PUBLIC_APP_CONTENT_MAIN_RUNNER_H_

#include <string>

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace sandbox {
struct SandboxInterfaceInfo;
}

namespace content {

class ContentMainDelegate;

class ContentMainRunner {
 public:
  virtual ~ContentMainRunner() {}

  
  static ContentMainRunner* Create();

  
#if defined(OS_WIN)
  
  
  
  virtual int Initialize(HINSTANCE instance,
                         sandbox::SandboxInterfaceInfo* sandbox_info,
                         ContentMainDelegate* delegate) = 0;
#else
  
  virtual int Initialize(int argc,
                         const char** argv,
                         ContentMainDelegate* delegate) = 0;
#endif

  
  virtual int Run() = 0;

  
  virtual void Shutdown() = 0;
};

}  

#endif  
