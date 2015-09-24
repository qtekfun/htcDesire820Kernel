// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_FIRST_RUN_H_
#define CHROME_BROWSER_FIRST_RUN_FIRST_RUN_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"

class CommandLine;
class FilePath;
class GURL;
class Profile;
class ProcessSingleton;
class ImporterHost;
class ImporterList;

class FirstRun {
 public:
  
  enum BubbleType {
    LARGE_BUBBLE,      
    OEM_BUBBLE,        
    MINIMAL_BUBBLE     
  };

  
  struct MasterPrefs {
    MasterPrefs();
    ~MasterPrefs();

    int ping_delay;
    bool homepage_defined;
    int do_import_items;
    int dont_import_items;
    bool run_search_engine_experiment;
    bool randomize_search_engine_experiment;
    bool make_chrome_default;
    std::vector<GURL> new_tabs;
    std::vector<GURL> bookmarks;
  };

  
  
  
  
  static int ImportNow(Profile* profile, const CommandLine& cmdline);

  
  
  static void AutoImport(
      Profile* profile,
      bool homepage_defined,
      int import_items,
      int dont_import_items,
      bool search_engine_experiment,
      bool randomize_search_engine_experiment,
      bool make_chrome_default,
      ProcessSingleton* process_singleton);

  
  static void PlatformSetup();

  
  static bool IsOrganicFirstRun();

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool ProcessMasterPreferences(const FilePath& user_data_dir,
                                       MasterPrefs* out_prefs);

  
  static bool IsChromeFirstRun();

  
  static bool CreateSentinel();

  
  
  static bool RemoveSentinel();

  
  
  
  static bool ImportSettings(Profile* profile,
                             scoped_refptr<ImporterHost> importer_host,
                             scoped_refptr<ImporterList> importer_list,
                             int items_to_import);

  
  
  
  
  
  static bool SetShowFirstRunBubblePref(bool show_bubble);

  
  
  
  static bool SetOEMFirstRunBubblePref();

  
  
  
  static bool SetMinimalFirstRunBubblePref();

  
  
  
  static bool SetShowWelcomePagePref();

  
  
  
  static bool SetPersonalDataManagerFirstRunPref();

  
  
  static bool SearchEngineSelectorDisallowed();

 private:
  friend class FirstRunTest;
  FRIEND_TEST_ALL_PREFIXES(Toolbar5ImporterTest, BookmarkParse);

#if defined(OS_WIN)
  
  
  static bool WriteEULAtoTempFile(FilePath* eula_path);

  
  
  
  static bool LaunchSetupWithParam(const std::string& param,
                                   const std::wstring& value,
                                   int* ret_code);

  
  
  static void DoDelayedInstallExtensions();

  
  
  
  static bool ImportSettings(Profile* profile,
                             int importer_type,
                             int items_to_import,
                             const FilePath& import_path,
                             bool skip_first_run_ui,
                             gfx::NativeView parent_window);

  
  
  static int ImportFromBrowser(Profile* profile, const CommandLine& cmdline);

#else
  static bool ImportBookmarks(const FilePath& import_bookmarks_path);
#endif

  
  
  static int ImportFromFile(Profile* profile, const CommandLine& cmdline);

  
  static bool GetFirstRunSentinelFilePath(FilePath* path);

  enum FirstRunState {
    FIRST_RUN_UNKNOWN,  
    FIRST_RUN_TRUE,
    FIRST_RUN_FALSE
  };

  
  static FirstRunState first_run_;

  
  DISALLOW_IMPLICIT_CONSTRUCTORS(FirstRun);
};

#endif  
