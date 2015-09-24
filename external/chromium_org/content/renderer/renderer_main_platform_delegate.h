// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_RENDERER_MAIN_PLATFORM_DELEGATE_H_
#define CHROME_RENDERER_RENDERER_MAIN_PLATFORM_DELEGATE_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/common/main_function_params.h"

#ifdef __OBJC__
@class NSBundle;
#else
class NSBundle;
#endif  

namespace content {

class CONTENT_EXPORT RendererMainPlatformDelegate {
 public:
  explicit RendererMainPlatformDelegate(
      const MainFunctionParams& parameters);
  ~RendererMainPlatformDelegate();

  
  
  void PlatformInitialize();
  void PlatformUninitialize();

  
  
  bool InitSandboxTests(bool no_sandbox);

  
  bool EnableSandbox();

  
  void RunSandboxTests(bool no_sandbox);

 private:
  const MainFunctionParams& parameters_;
#if defined(OS_WIN)
  HMODULE sandbox_test_module_;
#elif defined(OS_MACOSX)
  NSBundle* sandbox_tests_bundle_;
#endif

  DISALLOW_COPY_AND_ASSIGN(RendererMainPlatformDelegate);
};

}  

#endif  
