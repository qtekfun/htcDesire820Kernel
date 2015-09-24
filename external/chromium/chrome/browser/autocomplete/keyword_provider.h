// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_KEYWORD_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_KEYWORD_PROVIDER_H_
#pragma once

#include <string>

#include "chrome/browser/autocomplete/autocomplete.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;
class TemplateURL;
class TemplateURLModel;

class KeywordProvider : public AutocompleteProvider,
                        public NotificationObserver {
 public:
  KeywordProvider(ACProviderListener* listener, Profile* profile);
  
  KeywordProvider(ACProviderListener* listener, TemplateURLModel* model);

  
  
  
  
  
  static string16 SplitReplacementStringFromInput(
      const string16& input,
      bool trim_leading_whitespace);

  
  
  static const TemplateURL* GetSubstitutingTemplateURLForInput(
      Profile* profile,
      const AutocompleteInput& input,
      string16* remaining_input);

  
  virtual void Start(const AutocompleteInput& input, bool minimal_changes);
  virtual void Stop();

 private:
  class ScopedEndExtensionKeywordMode;
  friend class ScopedEndExtensionKeywordMode;

  virtual ~KeywordProvider();

  
  
  
  
  
  
  
  static bool ExtractKeywordFromInput(const AutocompleteInput& input,
                                      string16* keyword,
                                      string16* remaining_input);

  
  
  
  
  
  static string16 SplitKeywordFromInput(const string16& input,
                                        bool trim_leading_whitespace,
                                        string16* remaining_input);

  
  
  static void FillInURLAndContents(
      const string16& remaining_input,
      const TemplateURL* element,
      AutocompleteMatch* match);

  
  
  
  
  
  static int CalculateRelevance(AutocompleteInput::Type type,
                                bool complete,
                                bool support_replacement,
                                bool prefer_keyword,
                                bool allow_exact_keyword_match);

  
  
  AutocompleteMatch CreateAutocompleteMatch(
      TemplateURLModel* model,
      const string16& keyword,
      const AutocompleteInput& input,
      size_t prefix_length,
      const string16& remaining_input,
      int relevance);

  void EnterExtensionKeywordMode(const std::string& extension_id);
  void MaybeEndExtensionKeywordMode();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  TemplateURLModel* model_;

  
  
  
  int current_input_id_;

  
  
  AutocompleteInput extension_suggest_last_input_;

  
  
  std::vector<AutocompleteMatch> extension_suggest_matches_;

  
  
  std::string current_keyword_extension_id_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(KeywordProvider);
};

#endif  
