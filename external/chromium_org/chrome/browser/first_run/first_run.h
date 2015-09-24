// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_FIRST_RUN_H_
#define CHROME_BROWSER_FIRST_RUN_FIRST_RUN_H_

#include <string>
#include <vector>

#include "base/basictypes.h"

class CommandLine;
class GURL;
class Profile;

namespace base {
class FilePath;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace first_run {

enum AutoImportState {
  AUTO_IMPORT_NONE = 0,
  AUTO_IMPORT_CALLED = 1 << 0,
  AUTO_IMPORT_PROFILE_IMPORTED = 1 << 1,
  AUTO_IMPORT_BOOKMARKS_FILE_IMPORTED = 1 << 2,
};

enum FirstRunBubbleMetric {
  FIRST_RUN_BUBBLE_SHOWN = 0,       
  FIRST_RUN_BUBBLE_CHANGE_INVOKED,  
  NUM_FIRST_RUN_BUBBLE_METRICS
};

enum FirstRunBubbleOptions {
  FIRST_RUN_BUBBLE_DONT_SHOW,
  FIRST_RUN_BUBBLE_SUPPRESS,
  FIRST_RUN_BUBBLE_SHOW,
};

enum ProcessMasterPreferencesResult {
  FIRST_RUN_PROCEED = 0,  
  EULA_EXIT_NOW,          
};

struct MasterPrefs {
  MasterPrefs();
  ~MasterPrefs();

  
  
  

  int ping_delay;
  bool homepage_defined;
  int do_import_items;
  int dont_import_items;
  bool make_chrome_default_for_user;
  bool suppress_first_run_default_browser_prompt;
  std::vector<GURL> new_tabs;
  std::vector<GURL> bookmarks;
  std::string import_bookmarks_path;
  std::string variations_seed;
  std::string suppress_default_browser_prompt_for_version;
};

bool IsChromeFirstRun();

bool IsFirstRunSuppressed(const CommandLine& command_line);

void CreateSentinelIfNeeded();

std::string GetPingDelayPrefName();

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

bool RemoveSentinel();

bool SetShowFirstRunBubblePref(FirstRunBubbleOptions show_bubble_option);

void SetShouldShowWelcomePage();

bool ShouldShowWelcomePage();

void SetShouldDoPersonalDataManagerFirstRun();

bool ShouldDoPersonalDataManagerFirstRun();

void LogFirstRunMetric(FirstRunBubbleMetric metric);

void AutoImport(Profile* profile,
                bool homepage_defined,
                int import_items,
                int dont_import_items,
                const std::string& import_bookmarks_path);

void DoPostImportTasks(Profile* profile, bool make_chrome_default_for_user);

uint16 auto_import_state();

void SetMasterPrefsPathForTesting(const base::FilePath& master_prefs);

ProcessMasterPreferencesResult ProcessMasterPreferences(
    const base::FilePath& user_data_dir,
    MasterPrefs* out_prefs);

}  

#endif  
