// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APPS_APP_BROWSERTEST_UTIL_H_
#define CHROME_BROWSER_APPS_APP_BROWSERTEST_UTIL_H_


#include "apps/shell_window.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "content/public/common/page_transition_types.h"

namespace content {
class WebContents;
}

class Browser;
class CommandLine;

namespace extensions {
class Extension;

class PlatformAppBrowserTest : public ExtensionApiTest {
 public:
  PlatformAppBrowserTest();

  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;

  
  static apps::ShellWindow* GetFirstShellWindowForBrowser(Browser* browser);

 protected:
  
  
  const Extension* LoadAndLaunchPlatformApp(const char* name);

  
  const Extension* InstallPlatformApp(const char* name);

  
  
  const Extension* InstallAndLaunchPlatformApp(const char* name);

  
  
  
  content::WebContents* GetFirstShellWindowWebContents();

  
  
  apps::ShellWindow* GetFirstShellWindow();

  
  
  size_t RunGetWindowsFunctionForExtension(const Extension* extension);

  
  
  bool RunGetWindowFunctionForExtension(int window_id,
                                        const Extension* extension);

  
  size_t GetShellWindowCount();

  
  
  
  void ClearCommandLineArgs();

  
  void SetCommandLineArg(const std::string& test_file);

  
  apps::ShellWindow* CreateShellWindow(const Extension* extension);

  apps::ShellWindow* CreateShellWindowFromParams(
      const Extension* extension,
      const apps::ShellWindow::CreateParams& params);

  
  void CloseShellWindow(apps::ShellWindow* window);

  
  void CallAdjustBoundsToBeVisibleOnScreenForShellWindow(
      apps::ShellWindow* window,
      const gfx::Rect& cached_bounds,
      const gfx::Rect& cached_screen_bounds,
      const gfx::Rect& current_screen_bounds,
      const gfx::Size& minimum_size,
      gfx::Rect* bounds);
};

class ExperimentalPlatformAppBrowserTest : public PlatformAppBrowserTest {
 public:
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
};

}  

#endif  
