// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_H_
#define CHROME_BROWSER_UI_STARTUP_STARTUP_BROWSER_CREATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/prefs/session_startup_pref.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/startup/startup_tab.h"
#include "chrome/browser/ui/startup/startup_types.h"
#include "url/gurl.h"

class Browser;
class CommandLine;
class GURL;
class PrefService;

class StartupBrowserCreator {
 public:
  typedef std::vector<Profile*> Profiles;

  StartupBrowserCreator();
  ~StartupBrowserCreator();

  
  
  void AddFirstRunTab(const GURL& url);

  
  
  bool Start(const CommandLine& cmd_line,
             const base::FilePath& cur_dir,
             Profile* last_used_profile,
             const Profiles& last_opened_profiles,
             int* return_code) {
    return ProcessCmdLineImpl(cmd_line, cur_dir, true, last_used_profile,
                              last_opened_profiles, return_code, this);
  }

  
  
  
  
  
  
  
  static void ProcessCommandLineAlreadyRunning(
      const CommandLine& command_line,
      const base::FilePath& cur_dir,
      const base::FilePath& startup_profile_dir);

  template <class AutomationProviderClass>
  static bool CreateAutomationProvider(const std::string& channel_id,
                                       Profile* profile,
                                       size_t expected_tabs);

  
  
  static bool InSynchronousProfileLaunch();

  
  
  
  
  
  bool LaunchBrowser(const CommandLine& command_line,
                     Profile* profile,
                     const base::FilePath& cur_dir,
                     chrome::startup::IsProcessStartup is_process_startup,
                     chrome::startup::IsFirstRun is_first_run,
                     int* return_code);

  
  
  
  static bool WasRestarted();

  static SessionStartupPref GetSessionStartupPref(
      const CommandLine& command_line,
      Profile* profile);

  void set_is_default_browser_dialog_suppressed(bool new_value) {
    is_default_browser_dialog_suppressed_ = new_value;
  }

  bool is_default_browser_dialog_suppressed() const {
    return is_default_browser_dialog_suppressed_;
  }

  void set_show_main_browser_window(bool show_main_browser_window) {
    show_main_browser_window_ = show_main_browser_window;
  }

  bool show_main_browser_window() const {
    return show_main_browser_window_;
  }

  
  static void ClearLaunchedProfilesForTesting();

 private:
  friend class CloudPrintProxyPolicyTest;
  friend class CloudPrintProxyPolicyStartupTest;
  friend class StartupBrowserCreatorImpl;
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest,
                           ReadingWasRestartedAfterNormalStart);
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest,
                           ReadingWasRestartedAfterRestart);
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest, UpdateWithTwoProfiles);
  FRIEND_TEST_ALL_PREFIXES(StartupBrowserCreatorTest, LastUsedProfileActivated);

  
  
  static std::vector<GURL> GetURLsFromCommandLine(
      const CommandLine& command_line,
      const base::FilePath& cur_dir,
      Profile* profile);

  static bool ProcessCmdLineImpl(const CommandLine& command_line,
                                 const base::FilePath& cur_dir,
                                 bool process_startup,
                                 Profile* last_used_profile,
                                 const Profiles& last_opened_profiles,
                                 int* return_code,
                                 StartupBrowserCreator* browser_creator);

  
  static void ProcessCommandLineOnProfileCreated(
      const CommandLine& command_line,
      const base::FilePath& cur_dir,
      Profile* profile,
      Profile::CreateStatus status);

  
  
  static bool ActivatedProfile();

  
  std::vector<GURL> first_run_tabs_;

  
  
  
  bool is_default_browser_dialog_suppressed_;

  
  
  bool show_main_browser_window_;

  
  
  
  static bool was_restarted_read_;

  static bool in_synchronous_profile_launch_;

  DISALLOW_COPY_AND_ASSIGN(StartupBrowserCreator);
};

bool HasPendingUncleanExit(Profile* profile);

#endif  
