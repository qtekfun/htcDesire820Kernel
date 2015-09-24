// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_HELPER_H_
#define CHROME_INSTALLER_UTIL_HELPER_H_

#include <string>
#include <vector>

class BrowserDistribution;

namespace base {
class FilePath;
}

namespace installer {

base::FilePath GetChromeInstallPath(bool system_install, BrowserDistribution* dist);

void GetChromeUserDataPaths(BrowserDistribution* dist,
                            std::vector<base::FilePath>* paths);

BrowserDistribution* GetBinariesDistribution(bool system_install);

std::wstring GetAppGuidForUpdates(bool system_install);

}  

#endif  
