// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_LANGUAGE_SELECTOR_H_
#define CHROME_INSTALLER_UTIL_LANGUAGE_SELECTOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"

namespace installer {

class LanguageSelector {
 public:
  
  
  LanguageSelector();

  
  explicit LanguageSelector(const std::vector<std::wstring>& candidates);

  ~LanguageSelector();

  
  int offset() const { return offset_; }

  
  const std::wstring& matched_candidate() const { return matched_candidate_; }

  
  std::wstring selected_translation() const { return GetLanguageName(offset_); }

  
  static std::wstring GetLanguageName(int offset);

 private:
  typedef bool (*SelectPred_Fn)(const std::wstring&, int*);

  static bool SelectIf(const std::vector<std::wstring>& candidates,
                       SelectPred_Fn select_predicate,
                       std::wstring* matched_name, int* matched_offset);
  void DoSelect(const std::vector<std::wstring>& candidates);

  std::wstring matched_candidate_;
  int offset_;

  DISALLOW_COPY_AND_ASSIGN(LanguageSelector);
};

}  

#endif  
