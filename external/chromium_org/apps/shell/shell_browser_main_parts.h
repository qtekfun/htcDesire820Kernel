// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_SHELL_SHELL_BROWSER_MAIN_PARTS_H_
#define APPS_SHELL_SHELL_BROWSER_MAIN_PARTS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_main_parts.h"

namespace base {
class FilePath;
}

namespace content {
class ShellBrowserContext;
struct MainFunctionParams;
}

namespace views {
class ViewsDelegate;
}

namespace wm {
class WMTestHelper;
}

namespace apps {

class ShellBrowserContext;
class ShellExtensionsBrowserClient;
class ShellExtensionsClient;

class ShellBrowserMainParts : public content::BrowserMainParts {
 public:
  explicit ShellBrowserMainParts(
      const content::MainFunctionParams& parameters);
  virtual ~ShellBrowserMainParts();

  ShellBrowserContext* browser_context() {
    return browser_context_.get();
  }

  
  void CreateRootWindow();

  
  void LoadAndLaunchApp(const base::FilePath& app_dir);

  
  virtual void PreEarlyInitialization() OVERRIDE;
  virtual void PreMainMessageLoopStart() OVERRIDE;
  virtual void PostMainMessageLoopStart() OVERRIDE;
  virtual int PreCreateThreads() OVERRIDE;
  virtual void PreMainMessageLoopRun() OVERRIDE;
  virtual bool MainMessageLoopRun(int* result_code) OVERRIDE;
  virtual void PostMainMessageLoopRun() OVERRIDE;

 private:
  scoped_ptr<ShellBrowserContext> browser_context_;
  scoped_ptr<ShellExtensionsClient> extensions_client_;
  scoped_ptr<ShellExtensionsBrowserClient> extensions_browser_client_;

  
  scoped_ptr<wm::WMTestHelper> wm_test_helper_;

  DISALLOW_COPY_AND_ASSIGN(ShellBrowserMainParts);
};

}  

#endif  
