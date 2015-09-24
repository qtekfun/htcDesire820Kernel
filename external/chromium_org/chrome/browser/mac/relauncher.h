// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MAC_RELAUNCHER_H_
#define CHROME_BROWSER_MAC_RELAUNCHER_H_


#include <string>
#include <vector>

namespace content {
struct MainFunctionParams;
}

namespace mac_relauncher {

extern const char* const kRelauncherDMGDeviceArg;

bool RelaunchApp(const std::vector<std::string>& args);

bool RelaunchAppWithHelper(const std::string& helper,
                           const std::vector<std::string>& relauncher_args,
                           const std::vector<std::string>& args);

namespace internal {

int RelauncherMain(const content::MainFunctionParams& main_parameters);

}  

}  

#endif  
