// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_INIT_H_
#define CHROME_BROWSER_UI_BROWSER_INIT_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "googleurl/src/gurl.h"

class Browser;
class CommandLine;
class GURL;
class Profile;
class TabContents;

class BrowserInit {
 public:
  BrowserInit();
  ~BrowserInit();

  
  
  void AddFirstRunTab(const GURL& url);

  
  
  bool Start(const CommandLine& cmd_line, const FilePath& cur_dir,
             Profile* profile, int* return_code) {
    return ProcessCmdLineImpl(cmd_line, cur_dir, true, profile, return_code,
                              this);
  }

  
  
  
  
  
  
  static bool ProcessCommandLine(const CommandLine& cmd_line,
                                 const FilePath& cur_dir, bool process_startup,
                                 Profile* profile, int* return_code) {
    return ProcessCmdLineImpl(cmd_line, cur_dir, process_startup, profile,
                              return_code, NULL);
  }

  template <class AutomationProviderClass>
  static bool CreateAutomationProvider(const std::string& channel_id,
                                       Profile* profile,
                                       size_t expected_tabs);

  
  static bool InProcessStartup();

  
  
  
  
  bool LaunchBrowser(const CommandLine& command_line, Profile* profile,
                     const FilePath& cur_dir, bool process_startup,
                     int* return_code);

  
  
  
  

  class LaunchWithProfile {
   public:
    
    struct Tab {
      Tab();
      ~Tab();

      
      GURL url;

      
      
      bool is_app;

      
      bool is_pinned;

      
      std::string app_id;
    };

    
    
    
    
    LaunchWithProfile(const FilePath& cur_dir, const CommandLine& command_line);
    LaunchWithProfile(const FilePath& cur_dir, const CommandLine& command_line,
                      BrowserInit* browser_init);
    ~LaunchWithProfile();

    
    
    
    
    bool Launch(Profile* profile,
                const std::vector<GURL>& urls_to_open,
                bool process_startup);

    
    
    Browser* OpenURLsInBrowser(Browser* browser,
                               bool process_startup,
                               const std::vector<GURL>& urls);

    
    
    
    
    
    Browser* OpenTabsInBrowser(Browser* browser,
                               bool process_startup,
                               const std::vector<Tab>& tabs);

   private:
    FRIEND_TEST_ALL_PREFIXES(BrowserTest, RestorePinnedTabs);
    FRIEND_TEST_ALL_PREFIXES(BrowserTest, AppIdSwitch);

    
    
    
    bool IsAppLaunch(std::string* app_url, std::string* app_id);

    
    
    
    bool OpenApplicationWindow(Profile* profile);

    
    
    bool OpenApplicationTab(Profile* profile);

    
    
    
    
    
    void ProcessLaunchURLs(bool process_startup,
                           const std::vector<GURL>& urls_to_open);

    
    
    
    
    
    
    
    
    
    
    
    bool ProcessStartupURLs(const std::vector<GURL>& urls_to_open);

    
    
    void AddUniqueURLs(const std::vector<GURL>& urls,
                       std::vector<Tab>* tabs);

    
    void AddInfoBarsIfNecessary(Browser* browser);

    
    
    void AddCrashedInfoBarIfNecessary(TabContents* tab);

    
    
    void AddBadFlagsInfoBarIfNecessary(TabContents* tab);

    
    
    void AddDNSCertProvenanceCheckingWarningInfoBarIfNecessary(
        TabContents* tab);

    
    
    
    void AddObsoleteSystemInfoBarIfNecessary(TabContents* tab);

    
    void AddStartupURLs(std::vector<GURL>* startup_urls) const;

    
    
    void CheckDefaultBrowser(Profile* profile);

    const FilePath cur_dir_;
    const CommandLine& command_line_;
    Profile* profile_;
    BrowserInit* browser_init_;
    DISALLOW_COPY_AND_ASSIGN(LaunchWithProfile);
  };

 private:
  
  
  static std::vector<GURL> GetURLsFromCommandLine(
      const CommandLine& command_line,
      const FilePath& cur_dir,
      Profile* profile);

  static bool ProcessCmdLineImpl(const CommandLine& command_line,
                                 const FilePath& cur_dir, bool process_startup,
                                 Profile* profile, int* return_code,
                                 BrowserInit* browser_init);

  
  std::vector<GURL> first_run_tabs_;

  DISALLOW_COPY_AND_ASSIGN(BrowserInit);
};

#endif  
