// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_FIRST_RUN_INTERNAL_H_
#define CHROME_BROWSER_FIRST_RUN_FIRST_RUN_INTERNAL_H_

class MasterPrefs;
class Profile;

namespace base {
class FilePath;
}

namespace installer {
class MasterPreferences;
}

namespace first_run {

namespace internal {

enum FirstRunState {
  FIRST_RUN_UNKNOWN,  
  FIRST_RUN_TRUE,
  FIRST_RUN_FALSE,
  FIRST_RUN_CANCEL,  
                     
};

extern FirstRunState first_run_;

bool GeneratePrefFile(const base::FilePath& user_data_dir,
                      const installer::MasterPreferences& master_prefs);

void SetupMasterPrefsFromInstallPrefs(
    const installer::MasterPreferences& install_prefs,
    MasterPrefs* out_prefs);

bool CreateSentinel();


void DoPostImportPlatformSpecificTasks(Profile* profile);

bool GetFirstRunSentinelFilePath(base::FilePath* path);

bool IsOrganicFirstRun();

bool ShowPostInstallEULAIfNeeded(installer::MasterPreferences* install_prefs);

base::FilePath MasterPrefsPath();

}  
}  

#endif  
