// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LANGUAGE_PREFERENCES_H_
#define CHROME_BROWSER_CHROMEOS_LANGUAGE_PREFERENCES_H_
#pragma once

#include <stddef.h>  

namespace chromeos {
namespace language_prefs {

template <typename DataType>
struct LanguageMultipleChoicePreference {
  const char* pref_name;  
  DataType default_pref_value;
  const char* ibus_config_name;
  
  static const size_t kMaxItems = 11;
  struct {
    DataType ibus_config_value;
    int item_message_id;  
  } values_and_ids[kMaxItems];
  int label_message_id;  
};

struct LanguageBooleanPrefs {
  const char* pref_name;  
  bool default_pref_value;
  const char* ibus_config_name;
  int message_id;
};

struct LanguageIntegerRangePreference {
  const char* pref_name;  
  int default_pref_value;
  int min_pref_value;
  int max_pref_value;
  const char* ibus_config_name;
  int message_id;
};

extern const char kGeneralSectionName[];
extern const char kHotKeySectionName[];
extern const char kPreloadEnginesConfigName[];
extern const char kNextEngineInMenuConfigName[];
extern const char kPreviousEngineConfigName[];
extern const char kHotkeyNextEngineInMenu[];
extern const char kHotkeyPreviousEngine[];

extern const char kChewingSectionName[];

extern const LanguageBooleanPrefs kChewingBooleanPrefs[];
const size_t kNumChewingBooleanPrefs = 8 - 2; 

extern const LanguageIntegerRangePreference kChewingIntegerPrefs[];
const size_t kNumChewingIntegerPrefs = 2;
const int kChewingMaxChiSymbolLenIndex = 0;
const int kChewingCandPerPageIndex = 1;

extern const LanguageMultipleChoicePreference<const char*>
    kChewingMultipleChoicePrefs[];
const size_t kNumChewingMultipleChoicePrefs = 2;

extern const LanguageMultipleChoicePreference<int> kChewingHsuSelKeyType;

extern const char kHangulSectionName[];
extern const char kHangulKeyboardConfigName[];
extern const char kHangulHanjaKeysConfigName[];
extern const char kHangulHanjaKeys[];

struct HangulKeyboardNameIDPair {
  int message_id;
  const char* keyboard_id;
};

extern const HangulKeyboardNameIDPair kHangulKeyboardNameIDPairs[];
const size_t kNumHangulKeyboardNameIDPairs = 4;

extern const char kPinyinSectionName[];

extern const LanguageBooleanPrefs kPinyinBooleanPrefs[];
const size_t kNumPinyinBooleanPrefs = 11;

extern const LanguageMultipleChoicePreference<int> kPinyinDoublePinyinSchema;

struct PinyinIntegerPref {
  const char* pref_name;  
  int default_pref_value;
  const char* ibus_config_name;
  
};

extern const PinyinIntegerPref kPinyinIntegerPrefs[];
const size_t kNumPinyinIntegerPrefs = 1;

extern const char kMozcSectionName[];

extern const LanguageBooleanPrefs kMozcBooleanPrefs[];
const size_t kNumMozcBooleanPrefs = 8;

extern const LanguageMultipleChoicePreference<const char*>
    kMozcMultipleChoicePrefs[];
const size_t kNumMozcMultipleChoicePrefs = 8;

extern const LanguageIntegerRangePreference kMozcIntegerPrefs[];
const size_t kNumMozcIntegerPrefs = 1;

enum RemapType {
  kNoRemap = 0,
  kSwapCtrlAndAlt = 1,
  kSwapSearchAndCtrl = 2,
};
extern const LanguageMultipleChoicePreference<int>
    kXkbModifierMultipleChoicePrefs;

extern const int kXkbAutoRepeatDelayInMs;
extern const int kXkbAutoRepeatIntervalInMs;

extern const char kPreferredKeyboardLayout[];

}  
}  

#endif  
