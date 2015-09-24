// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_COMMON_INSTALL_UTILS_H_
#define CLOUD_PRINT_COMMON_INSTALL_UTILS_H_

#include <wtypes.h>
#include <string>

#include "base/files/file_path.h"
#include "base/strings/string16.h"

namespace cloud_print {

void SetGoogleUpdateKeys(const base::string16& product_id,
                         const base::string16& product_name);

void SetGoogleUpdateError(const base::string16& product_id,
                          const base::string16& message);

void DeleteGoogleUpdateKeys(const base::string16& product_id);

void CreateUninstallKey(const base::string16& uninstall_id,
                        const base::string16& product_name,
                        const std::string& uninstall_switch);

void DeleteUninstallKey(const base::string16& uninstall_id);

base::FilePath GetInstallLocation(const base::string16& uninstall_id);

void DeleteProgramDir(const std::string& delete_switch);

bool IsProgramsFilesParent(const base::FilePath& path);

}  

#endif  

