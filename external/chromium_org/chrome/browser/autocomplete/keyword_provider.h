// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_KEYWORD_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_KEYWORD_PROVIDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;
class TemplateURL;
class TemplateURLService;

class KeywordProvider : public AutocompleteProvider,
                        public content::NotificationObserver {
 public:
  KeywordProvider(AutocompleteProviderListener* listener, Profile* profile);
  
  KeywordProvider(AutocompleteProviderListener* listener,
                  TemplateURLService* model);

  
  
  
  
  
  static base::string16 SplitKeywordFromInput(const base::string16& input,
                                              bool trim_leading_whitespace,
                                              base::string16* remaining_input);

  
  
  
  
  
  static base::string16 SplitReplacementStringFromInput(
      const base::string16& input,
      bool trim_leading_whitespace);

  
  
  
  static const TemplateURL* GetSubstitutingTemplateURLForInput(
      TemplateURLService* model,
      AutocompleteInput* input);

  
  
  
  base::string16 GetKeywordForText(const base::string16& text) const;

  
  AutocompleteMatch CreateVerbatimMatch(const base::string16& text,
                                        const base::string16& keyword,
                                        const AutocompleteInput& input);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;
  virtual void Stop(bool clear_cached_results) OVERRIDE;

 private:
  class ScopedEndExtensionKeywordMode;
  friend class ScopedEndExtensionKeywordMode;

  virtual ~KeywordProvider();

  
  
  
  
  
  
  
  static bool ExtractKeywordFromInput(const AutocompleteInput& input,
                                      base::string16* keyword,
                                      base::string16* remaining_input);

  
  
  
  
  
  static int CalculateRelevance(AutocompleteInput::Type type,
                                bool complete,
                                bool support_replacement,
                                bool prefer_keyword,
                                bool allow_exact_keyword_match);

  
  
  AutocompleteMatch CreateAutocompleteMatch(
      const TemplateURL* template_url,
      const AutocompleteInput& input,
      size_t prefix_length,
      const base::string16& remaining_input,
      bool allowed_to_be_default_match,
      int relevance);

  
  
  void FillInURLAndContents(const base::string16& remaining_input,
                            const TemplateURL* element,
                            AutocompleteMatch* match) const;

  void EnterExtensionKeywordMode(const std::string& extension_id);
  void MaybeEndExtensionKeywordMode();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  TemplateURLService* GetTemplateURLService() const;

  
  
  TemplateURLService* model_;

  
  
  
  int current_input_id_;

  
  
  AutocompleteInput extension_suggest_last_input_;

  
  
  std::vector<AutocompleteMatch> extension_suggest_matches_;

  
  
  std::string current_keyword_extension_id_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(KeywordProvider);
};

#endif  
