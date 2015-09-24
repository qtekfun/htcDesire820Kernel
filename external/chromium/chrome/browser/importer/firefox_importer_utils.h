// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_FIREFOX_IMPORTER_UTILS_H_
#define CHROME_BROWSER_IMPORTER_FIREFOX_IMPORTER_UTILS_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "build/build_config.h"

class DictionaryValue;
class FilePath;
class GURL;
class TemplateURL;

#if defined(OS_WIN)
int GetCurrentFirefoxMajorVersionFromRegistry();

FilePath GetFirefoxInstallPathFromRegistry();
#endif  

#if defined(OS_MACOSX)
FilePath GetFirefoxDylibPath();
#endif  

FilePath GetFirefoxProfilePath();

bool GetFirefoxVersionAndPathFromProfile(const FilePath& profile_path,
                                         int* version,
                                         FilePath* app_path);

FilePath GetProfilesINI();

void ParseProfileINI(const FilePath& file, DictionaryValue* root);

bool CanImportURL(const GURL& url);

void ParseSearchEnginesFromXMLFiles(const std::vector<FilePath>& xml_files,
                                    std::vector<TemplateURL*>* search_engines);

int GetFirefoxDefaultSearchEngineIndex(
    const std::vector<TemplateURL*>& search_engines,
    const FilePath& profile_path);

GURL GetHomepage(const FilePath& profile_path);

bool IsDefaultHomepage(const GURL& homepage, const FilePath& app_path);

bool ParsePrefFile(const FilePath& pref_file, DictionaryValue* prefs);

std::string GetPrefsJsValue(const std::string& prefs,
                            const std::string& pref_key);

#endif  
