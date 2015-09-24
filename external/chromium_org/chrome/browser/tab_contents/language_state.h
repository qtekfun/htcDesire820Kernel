// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_LANGUAGE_STATE_H_
#define CHROME_BROWSER_TAB_CONTENTS_LANGUAGE_STATE_H_

#include <string>

#include "base/basictypes.h"

class LanguageStateObserver;

namespace content {
struct LoadCommittedDetails;
class NavigationController;
class WebContents;
}


class LanguageState {
 public:
  explicit LanguageState(content::NavigationController* nav_controller);
  ~LanguageState();

  
  
  void DidNavigate(const content::LoadCommittedDetails& details);

  
  
  
  void LanguageDetermined(const std::string& page_language,
                          bool page_needs_translation);

  
  
  
  
  std::string AutoTranslateTo() const;

  
  bool InTranslateNavigation() const;

  
  bool IsPageTranslated() const { return original_lang_ != current_lang_; }

  const std::string& original_language() const { return original_lang_; }

  void SetCurrentLanguage(const std::string& language);
  const std::string& current_language() const { return current_lang_; }

  bool page_needs_translation() const { return page_needs_translation_; }

  
  bool translation_pending() const { return translation_pending_; }
  void set_translation_pending(bool value) { translation_pending_ = value; }

  
  bool translation_declined() const { return translation_declined_; }
  void set_translation_declined(bool value) { translation_declined_ = value; }

  
  
  bool in_page_navigation() const { return in_page_navigation_; }

  
  
  bool translate_enabled() const { return translate_enabled_; }
  void SetTranslateEnabled(bool value);

  
  
  bool HasLanguageChanged() const;

  void set_observer(LanguageStateObserver* observer) { observer_ = observer; }

 private:
  void SetIsPageTranslated(bool value);

  
  bool is_page_translated_;

  
  
  
  
  std::string original_lang_;
  std::string current_lang_;

  
  std::string prev_original_lang_;
  std::string prev_current_lang_;

  
  content::NavigationController* navigation_controller_;

  
  
  
  bool page_needs_translation_;

  
  
  
  
  
  
  
  
  bool translation_pending_;

  
  
  
  bool translation_declined_;

  
  bool in_page_navigation_;

  
  bool translate_enabled_;

  LanguageStateObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(LanguageState);
};

#endif  
