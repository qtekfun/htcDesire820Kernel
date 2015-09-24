// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_LANGUAGE_COMBOBOX_MODEL_H_
#define CHROME_BROWSER_LANGUAGE_COMBOBOX_MODEL_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "ui/base/models/combobox_model.h"

class Profile;

class LanguageList {
 public:
  struct LocaleData {
    LocaleData() { }
    LocaleData(const string16& name, const std::string& code)
        : native_name(name), locale_code(code) { }

    string16 native_name;
    std::string locale_code;  
  };
  typedef std::map<string16, LocaleData> LocaleDataMap;

  LanguageList();

  explicit LanguageList(const std::vector<std::string>& locale_codes);

  virtual ~LanguageList();

  
  
  void CopySpecifiedLanguagesUp(const std::string& locale_codes);

  int get_languages_count() const;

  string16 GetLanguageNameAt(int index) const;

  
  std::string GetLocaleFromIndex(int index) const;

  
  
  int GetIndexFromLocale(const std::string& locale) const;

 private:
  
  std::vector<string16> locale_names_;

  
  LocaleDataMap native_names_;

  void InitNativeNames(const std::vector<std::string>& locale_codes);

  DISALLOW_COPY_AND_ASSIGN(LanguageList);
};

class LanguageComboboxModel : public LanguageList, public ui::ComboboxModel {
 public:
  LanguageComboboxModel();

  
  LanguageComboboxModel(Profile* profile,
                        const std::vector<std::string>& locale_codes);

  virtual ~LanguageComboboxModel();

  virtual int GetItemCount();
  virtual string16 GetItemAt(int index);

  
  
  
  
  
  
  
  int GetSelectedLanguageIndex(const std::string& prefs);

 private:
  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(LanguageComboboxModel);
};

#endif  
