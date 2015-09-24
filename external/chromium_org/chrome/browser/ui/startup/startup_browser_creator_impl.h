// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_IMPL_H_
#define CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_IMPL_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/startup/startup_tab.h"
#include "chrome/browser/ui/startup/startup_types.h"
#include "url/gurl.h"

class Browser;
class CommandLine;
class Profile;
class StartupBrowserCreator;

namespace base {
class FilePath;
}

namespace content {
class WebContents;
}

namespace gfx {
class Rect;
}

namespace internals {
GURL GetWelcomePageURL();
}

class StartupBrowserCreatorImpl {
 public:
  
  
  
  
  StartupBrowserCreatorImpl(const base::FilePath& cur_dir,
                            const CommandLine& command_line,
                            chrome::startup::IsFirstRun is_first_run);
  StartupBrowserCreatorImpl(const base::FilePath& cur_dir,
                            const CommandLine& command_line,
                            StartupBrowserCreator* browser_creator,
                            chrome::startup::IsFirstRun is_first_run);
  ~StartupBrowserCreatorImpl();

  
  
  
  
  bool Launch(Profile* profile,
              const std::vector<GURL>& urls_to_open,
              bool process_startup,
              chrome::HostDesktopType desktop_type);

  
  
  Browser* OpenURLsInBrowser(Browser* browser,
                             bool process_startup,
                             const std::vector<GURL>& urls,
                             chrome::HostDesktopType desktop_type);

  
  
  
  
  
  Browser* OpenTabsInBrowser(Browser* browser,
                             bool process_startup,
                             const StartupTabs& tabs,
                             chrome::HostDesktopType desktop_type);

 private:
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, RestorePinnedTabs);
  FRIEND_TEST_ALL_PREFIXES(BrowserTest, AppIdSwitch);

  
  void ExtractOptionalAppWindowSize(gfx::Rect* bounds);

  
  
  
  bool IsAppLaunch(std::string* app_url, std::string* app_id);

  
  
  
  
  bool OpenApplicationWindow(Profile* profile,
                             content::WebContents** out_app_contents);

  
  
  bool OpenApplicationTab(Profile* profile);

  
  
  
  
  
  
  void ProcessLaunchURLs(bool process_startup,
                         const std::vector<GURL>& urls_to_open,
                         chrome::HostDesktopType desktop_type);

  
  
  
  
  
  
  
  bool ProcessStartupURLs(const std::vector<GURL>& urls_to_open,
                          chrome::HostDesktopType desktop_type);

  
  
  
  
  
  
  
  
  
  
  
  Browser* ProcessSpecifiedURLs(const std::vector<GURL>& urls_to_open,
                                chrome::HostDesktopType desktop_type);

  
  
  void AddUniqueURLs(const std::vector<GURL>& urls, StartupTabs* tabs);

  
  void AddInfoBarsIfNecessary(
      Browser* browser,
      chrome::startup::IsProcessStartup is_process_startup);

  
  void AddStartupURLs(std::vector<GURL>* startup_urls) const;

  
  
  void CheckPreferencesBackup(Profile* profile);

  
  
  static bool OpenStartupURLsInExistingBrowser(
      Profile* profile,
      const std::vector<GURL>& startup_urls);

  const base::FilePath cur_dir_;
  const CommandLine& command_line_;
  Profile* profile_;
  StartupBrowserCreator* browser_creator_;
  bool is_first_run_;
  DISALLOW_COPY_AND_ASSIGN(StartupBrowserCreatorImpl);
};

#endif  
