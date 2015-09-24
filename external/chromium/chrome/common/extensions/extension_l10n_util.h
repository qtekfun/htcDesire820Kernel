// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_L10N_UTIL_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_L10N_UTIL_H_
#pragma once

#include <set>
#include <string>
#include <vector>

class DictionaryValue;
class Extension;
class ExtensionMessageBundle;
class FilePath;
class GURL;
class ResourceDispatcherHostRequestInfo;
struct ExtensionInfo;

namespace extension_l10n_util {

void SetProcessLocale(const std::string& locale);

std::string GetDefaultLocaleFromManifest(const DictionaryValue& manifest,
                                         std::string* error);

// doesn't match the locale written into info.extension_manifest.
bool ShouldRelocalizeManifest(const ExtensionInfo& info);

bool LocalizeManifest(const ExtensionMessageBundle& messages,
                      DictionaryValue* manifest,
                      std::string* error);

bool LocalizeExtension(const FilePath& extension_path,
                       DictionaryValue* manifest,
                       std::string* error);

bool AddLocale(const std::set<std::string>& chrome_locales,
               const FilePath& locale_folder,
               const std::string& locale_name,
               std::set<std::string>* valid_locales,
               std::string* error);

std::string CurrentLocaleOrDefault();

void GetAllLocales(std::set<std::string>* all_locales);

bool GetValidLocales(const FilePath& locale_path,
                     std::set<std::string>* locales,
                     std::string* error);

ExtensionMessageBundle* LoadMessageCatalogs(
    const FilePath& locale_path,
    const std::string& default_locale,
    const std::string& app_locale,
    const std::set<std::string>& valid_locales,
    std::string* error);

bool ShouldSkipValidation(const FilePath& locales_path,
                          const FilePath& locale_path,
                          const std::set<std::string>& all_locales);

}  

#endif  
