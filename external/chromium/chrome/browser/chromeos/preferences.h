// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PREFERENCES_H_
#define CHROME_BROWSER_CHROMEOS_PREFERENCES_H_
#pragma once

#include <string>
#include <vector>

#include "chrome/browser/chromeos/language_preferences.h"
#include "chrome/browser/prefs/pref_member.h"
#include "content/common/notification_observer.h"

class PrefService;

namespace chromeos {

class Preferences : public NotificationObserver {
 public:
  Preferences();
  virtual ~Preferences();

  
  static void RegisterUserPrefs(PrefService* prefs);

  
  void Init(PrefService* prefs);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  
  
  void NotifyPrefChanged(const std::string* pref_name);

  
  
  
  void SetLanguageConfigBoolean(const char* section,
                                const char* name,
                                bool value);

  
  
  void SetLanguageConfigInteger(const char* section,
                                const char* name,
                                int value);

  
  
  void SetLanguageConfigString(const char* section,
                               const char* name,
                               const std::string& value);

  
  
  void SetLanguageConfigStringList(const char* section,
                                   const char* name,
                                   const std::vector<std::string>& values);

  
  
  void SetLanguageConfigStringListAsCSV(const char* section,
                                        const char* name,
                                        const std::string& value);

  
  void UpdateModifierKeyMapping();

  
  
  
  void UpdateAutoRepeatRate();

  BooleanPrefMember tap_to_click_enabled_;
  BooleanPrefMember vert_edge_scroll_enabled_;
  BooleanPrefMember accessibility_enabled_;
  IntegerPrefMember speed_factor_;
  IntegerPrefMember sensitivity_;
  BooleanPrefMember use_24hour_clock_;

  
  StringPrefMember language_hotkey_next_engine_in_menu_;
  StringPrefMember language_hotkey_previous_engine_;
  StringPrefMember language_preferred_languages_;
  StringPrefMember language_preload_engines_;
  BooleanPrefMember language_chewing_boolean_prefs_[
      language_prefs::kNumChewingBooleanPrefs];
  StringPrefMember language_chewing_multiple_choice_prefs_[
      language_prefs::kNumChewingMultipleChoicePrefs];
  IntegerPrefMember language_chewing_hsu_sel_key_type_;
  IntegerPrefMember language_chewing_integer_prefs_[
      language_prefs::kNumChewingIntegerPrefs];
  StringPrefMember language_hangul_keyboard_;
  StringPrefMember language_hangul_hanja_keys_;
  BooleanPrefMember language_pinyin_boolean_prefs_[
      language_prefs::kNumPinyinBooleanPrefs];
  IntegerPrefMember language_pinyin_int_prefs_[
      language_prefs::kNumPinyinIntegerPrefs];
  IntegerPrefMember language_pinyin_double_pinyin_schema_;
  BooleanPrefMember language_mozc_boolean_prefs_[
      language_prefs::kNumMozcBooleanPrefs];
  StringPrefMember language_mozc_multiple_choice_prefs_[
      language_prefs::kNumMozcMultipleChoicePrefs];
  IntegerPrefMember language_mozc_integer_prefs_[
      language_prefs::kNumMozcIntegerPrefs];
  IntegerPrefMember language_xkb_remap_search_key_to_;
  IntegerPrefMember language_xkb_remap_control_key_to_;
  IntegerPrefMember language_xkb_remap_alt_key_to_;
  BooleanPrefMember language_xkb_auto_repeat_enabled_;
  IntegerPrefMember language_xkb_auto_repeat_delay_pref_;
  IntegerPrefMember language_xkb_auto_repeat_interval_pref_;

  BooleanPrefMember enable_screen_lock_;

  DISALLOW_COPY_AND_ASSIGN(Preferences);
};

}  

#endif  
