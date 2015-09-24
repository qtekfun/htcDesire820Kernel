// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_INSTALLER_SETUP_SETUP_MAIN_H_
#define CHROME_INSTALLER_SETUP_SETUP_MAIN_H_

#include "chrome/installer/util/util_constants.h"

class CommandLine;

namespace base {
class FilePath;
}  

namespace installer {
class InstallationState;
class InstallerState;
class MasterPreferences;

InstallStatus InstallProductsHelper(const InstallationState& original_state,
                                    const CommandLine& cmd_line,
                                    const MasterPreferences& prefs,
                                    const InstallerState& installer_state,
                                    base::FilePath* installer_directory,
                                    ArchiveType* archive_type,
                                    bool* delegated_to_existing);

}  

#endif  
