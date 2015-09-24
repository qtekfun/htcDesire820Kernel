// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_APP_CONTENT_MAIN_DELEGATE_H_
#define CONTENT_PUBLIC_APP_CONTENT_MAIN_DELEGATE_H_

#include <string>

#include "build/build_config.h"
#include "content/common/content_export.h"

namespace content {

class ContentBrowserClient;
class ContentPluginClient;
class ContentRendererClient;
class ContentUtilityClient;
class ZygoteForkDelegate;
struct MainFunctionParams;

class CONTENT_EXPORT ContentMainDelegate {
 public:
  virtual ~ContentMainDelegate() {}

  
  
  
  
  
  virtual bool BasicStartupComplete(int* exit_code);

  
  
  virtual void PreSandboxStartup() {}

  
  
  virtual void SandboxInitialized(const std::string& process_type) {}

  
  virtual int RunProcess(
      const std::string& process_type,
      const MainFunctionParams& main_function_params);

  
  virtual void ProcessExiting(const std::string& process_type) {}

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  
  virtual bool ProcessRegistersWithSystemProcess(
      const std::string& process_type);

  
  
  
  virtual bool ShouldSendMachPort(const std::string& process_type);

  
  
  
  virtual bool DelaySandboxInitialization(const std::string& process_type);

#elif defined(OS_POSIX) && !defined(OS_ANDROID) && !defined(OS_IOS)
  
  
  virtual ZygoteForkDelegate* ZygoteStarting();

  
  virtual void ZygoteForked() {}
#endif  

 protected:
  friend class ContentClientInitializer;

  
  
  
  virtual ContentBrowserClient* CreateContentBrowserClient();
  virtual ContentPluginClient* CreateContentPluginClient();
  virtual ContentRendererClient* CreateContentRendererClient();
  virtual ContentUtilityClient* CreateContentUtilityClient();
};

}  

#endif  
