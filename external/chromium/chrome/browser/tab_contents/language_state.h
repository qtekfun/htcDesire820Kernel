// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_LANGUAGE_STATE_H_
#define CHROME_BROWSER_TAB_CONTENTS_LANGUAGE_STATE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "content/browser/tab_contents/navigation_controller.h"


class LanguageState {
 public:
  explicit LanguageState(NavigationController* nav_controller);
  ~LanguageState();

  
  
  void DidNavigate(const NavigationController::LoadCommittedDetails& details);

  
  
  
  void LanguageDetermined(const std::string& page_language,
                          bool page_translatable);

  
  
  
  
  std::string AutoTranslateTo() const;

  
  bool IsPageTranslated() const { return original_lang_ != current_lang_; }

  const std::string& original_language() const { return original_lang_; }

  void set_current_language(const std::string& language) {
    current_lang_ = language;
  }
  const std::string& current_language() const { return current_lang_; }

  bool page_translatable() const { return page_translatable_; }

  
  bool translation_pending() const { return translation_pending_; }
  void set_translation_pending(bool value) { translation_pending_ = value; }

  
  bool translation_declined() const { return translation_declined_; }
  void set_translation_declined(bool value) { translation_declined_ = value; }

  
  
  bool in_page_navigation() const { return in_page_navigation_; }

 private:
  
  
  
  
  std::string original_lang_;
  std::string current_lang_;

  
  std::string prev_original_lang_;
  std::string prev_current_lang_;

  
  NavigationController* navigation_controller_;

  
  
  
  bool page_translatable_;

  
  
  
  
  
  
  
  
  bool translation_pending_;

  
  
  
  bool translation_declined_;

  
  bool in_page_navigation_;

  DISALLOW_COPY_AND_ASSIGN(LanguageState);
};

#endif  
