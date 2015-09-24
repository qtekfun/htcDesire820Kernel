// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_PREFS_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_PREFS_H_
#pragma once

#include <string>

#include "googleurl/src/gurl.h"

class DictionaryValue;
class ListValue;
class PrefService;

class TranslatePrefs {
 public:
  static const char kPrefTranslateLanguageBlacklist[];
  static const char kPrefTranslateSiteBlacklist[];
  static const char kPrefTranslateWhitelists[];
  static const char kPrefTranslateDeniedCount[];
  static const char kPrefTranslateAcceptedCount[];

  explicit TranslatePrefs(PrefService* user_prefs);

  bool IsLanguageBlacklisted(const std::string& original_language);
  void BlacklistLanguage(const std::string& original_language);
  void RemoveLanguageFromBlacklist(const std::string& original_language);

  bool IsSiteBlacklisted(const std::string& site);
  void BlacklistSite(const std::string& site);
  void RemoveSiteFromBlacklist(const std::string& site);

  bool IsLanguagePairWhitelisted(const std::string& original_language,
      const std::string& target_language);
  void WhitelistLanguagePair(const std::string& original_language,
      const std::string& target_language);
  void RemoveLanguagePairFromWhitelist(const std::string& original_language,
      const std::string& target_language);

  
  
  
  int GetTranslationDeniedCount(const std::string& language);
  void IncrementTranslationDeniedCount(const std::string& language);
  void ResetTranslationDeniedCount(const std::string& language);

  
  
  
  int GetTranslationAcceptedCount(const std::string& language);
  void IncrementTranslationAcceptedCount(const std::string& language);
  void ResetTranslationAcceptedCount(const std::string& language);

  static bool CanTranslate(PrefService* user_prefs,
      const std::string& original_language, const GURL& url);
  static bool ShouldAutoTranslate(PrefService* user_prefs,
      const std::string& original_language, std::string* target_language);
  static void RegisterUserPrefs(PrefService* user_prefs);

 private:
  static void MigrateTranslateWhitelists(PrefService* user_prefs);
  bool IsValueBlacklisted(const char* pref_id, const std::string& value);
  void BlacklistValue(const char* pref_id, const std::string& value);
  void RemoveValueFromBlacklist(const char* pref_id, const std::string& value);
  bool IsValueInList(const ListValue* list, const std::string& value);
  bool IsLanguageWhitelisted(const std::string& original_language,
      std::string* target_language);

  
  
  DictionaryValue* GetTranslationDeniedCountDictionary();

  
  
  DictionaryValue* GetTranslationAcceptedCountDictionary();

  PrefService* prefs_;  
};

#endif  
