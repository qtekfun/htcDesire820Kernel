// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LANGUAGE_LIST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LANGUAGE_LIST_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace chromeos {

class LanguageList {
 public:
  LanguageList();
  ~LanguageList();

  
  int languages_count() const { return static_cast<int>(locale_names_.size()); }

  
  base::string16 GetLanguageNameAt(int index) const;

  
  std::string GetLocaleFromIndex(int index) const;

  
  int GetIndexFromLocale(const std::string& locale) const;

  
  
  void CopySpecifiedLanguagesUp(const std::string& locale_codes);

 private:
  struct LocaleData {
    LocaleData() {}
    LocaleData(const base::string16& name, const std::string& code)
        : native_name(name), locale_code(code) {}

    base::string16 native_name;
    std::string locale_code;  
  };

  typedef std::map<base::string16, LocaleData> LocaleDataMap;

  void InitNativeNames(const std::vector<std::string>& locale_codes);

  
  std::vector<base::string16> locale_names_;

  
  LocaleDataMap native_names_;

  DISALLOW_COPY_AND_ASSIGN(LanguageList);
};

}  

#endif  
