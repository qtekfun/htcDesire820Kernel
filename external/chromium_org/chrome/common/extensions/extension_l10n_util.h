// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_L10N_UTIL_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_L10N_UTIL_H_

#include <set>
#include <string>
#include <vector>

namespace base {
class DictionaryValue;
class FilePath;
}

namespace extensions {
struct ExtensionInfo;
class MessageBundle;
}

namespace extension_l10n_util {

void SetProcessLocale(const std::string& locale);

std::string GetDefaultLocaleFromManifest(const base::DictionaryValue& manifest,
                                         std::string* error);

// doesn't match the locale written into info.extension_manifest.
bool ShouldRelocalizeManifest(const base::DictionaryValue* manifest);

bool LocalizeManifest(const extensions::MessageBundle& messages,
                      base::DictionaryValue* manifest,
                      std::string* error);

bool LocalizeExtension(const base::FilePath& extension_path,
                       base::DictionaryValue* manifest,
                       std::string* error);

bool AddLocale(const std::set<std::string>& chrome_locales,
               const base::FilePath& locale_folder,
               const std::string& locale_name,
               std::set<std::string>* valid_locales,
               std::string* error);

std::string CurrentLocaleOrDefault();

void GetAllLocales(std::set<std::string>* all_locales);

void GetAllFallbackLocales(const std::string& application_locale,
                           const std::string& default_locale,
                           std::vector<std::string>* all_fallback_locales);

bool GetValidLocales(const base::FilePath& locale_path,
                     std::set<std::string>* locales,
                     std::string* error);

extensions::MessageBundle* LoadMessageCatalogs(
    const base::FilePath& locale_path,
    const std::string& default_locale,
    const std::string& app_locale,
    const std::set<std::string>& valid_locales,
    std::string* error);

bool ValidateExtensionLocales(const base::FilePath& extension_path,
                              const base::DictionaryValue* manifest,
                              std::string* error);

bool ShouldSkipValidation(const base::FilePath& locales_path,
                          const base::FilePath& locale_path,
                          const std::set<std::string>& all_locales);

class ScopedLocaleForTest {
 public:
  
  ScopedLocaleForTest();

  
  explicit ScopedLocaleForTest(const std::string& locale);

  ~ScopedLocaleForTest();

 private:
  std::string locale_;  
};


}  

#endif  
