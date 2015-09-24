// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_VERIFICATION_COMMON_H_
#define CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_VERIFICATION_COMMON_H_

#include <set>
#include <string>
#include <vector>
#include "base/strings/string16.h"
#include "chrome/browser/install_verification/win/module_ids.h"

struct ModuleInfo;

std::string CalculateModuleNameDigest(const base::string16& module_name);

bool GetLoadedModules(std::set<ModuleInfo>* loaded_modules);

typedef void (ModuleVerificationDelegate)(size_t module_id);

void ReportModuleMatches(const std::vector<std::string>& module_name_digests,
                         const ModuleIDs& module_ids,
                         ModuleVerificationDelegate* delegate);

#endif  
