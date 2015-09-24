// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_FIREFOX_IMPORTER_UTILS_H_
#define CHROME_COMMON_IMPORTER_FIREFOX_IMPORTER_UTILS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "build/build_config.h"

class GURL;
class TemplateURL;

namespace base {
class DictionaryValue;
class FilePath;
}

#if defined(OS_WIN)
int GetCurrentFirefoxMajorVersionFromRegistry();

base::FilePath GetFirefoxInstallPathFromRegistry();
#endif  

#if defined(OS_MACOSX)
base::FilePath GetFirefoxDylibPath();
#endif  

base::FilePath GetFirefoxProfilePath();

base::FilePath GetFirefoxProfilePathFromDictionary(
    const base::DictionaryValue& root);

bool GetFirefoxVersionAndPathFromProfile(const base::FilePath& profile_path,
                                         int* version,
                                         base::FilePath* app_path);

base::FilePath GetProfilesINI();

void ParseProfileINI(const base::FilePath& file, base::DictionaryValue* root);

GURL GetHomepage(const base::FilePath& profile_path);

bool IsDefaultHomepage(const GURL& homepage, const base::FilePath& app_path);

std::string GetPrefsJsValue(const std::string& prefs,
                            const std::string& pref_key);

base::string16 GetFirefoxImporterName(const base::FilePath& app_path);

#endif  
