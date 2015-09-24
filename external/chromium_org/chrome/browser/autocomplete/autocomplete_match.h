// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_MATCH_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_MATCH_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/search_engines/template_url.h"
#include "chrome/common/autocomplete_match_type.h"
#include "content/public/common/page_transition_types.h"
#include "url/gurl.h"

class AutocompleteProvider;
class Profile;
class TemplateURL;

namespace base {
class Time;
}  


struct AutocompleteMatch {
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct ACMatchClassification {
    
    
    
    enum Style {
      NONE  = 0,
      URL   = 1 << 0,  
      MATCH = 1 << 1,  
      DIM   = 1 << 2,  
    };

    ACMatchClassification(size_t offset, int style)
        : offset(offset),
          style(style) {
    }

    
    size_t offset;

    int style;
  };

  typedef std::vector<ACMatchClassification> ACMatchClassifications;

  
  
  typedef std::map<std::string, std::string> AdditionalInfo;

  
  typedef AutocompleteMatchType::Type Type;

  
  
  static const char16 kInvalidChars[];

  AutocompleteMatch();
  AutocompleteMatch(AutocompleteProvider* provider,
                    int relevance,
                    bool deletable,
                    Type type);
  AutocompleteMatch(const AutocompleteMatch& match);
  ~AutocompleteMatch();

  
  AutocompleteMatch& operator=(const AutocompleteMatch& match);

  
  
  static int TypeToIcon(Type type);

  
  
  static int TypeToLocationBarIcon(Type type);

  
  static bool MoreRelevant(const AutocompleteMatch& elem1,
                           const AutocompleteMatch& elem2);

  
  
  
  
  static bool DestinationSortFunc(const AutocompleteMatch& elem1,
                                  const AutocompleteMatch& elem2);
  static bool DestinationsEqual(const AutocompleteMatch& elem1,
                                const AutocompleteMatch& elem2);

  
  
  
  
  static void ClassifyMatchInString(const base::string16& find_text,
                                    const base::string16& text,
                                    int style,
                                    ACMatchClassifications* classifications);

  
  
  
  
  static void ClassifyLocationInString(size_t match_location,
                                       size_t match_length,
                                       size_t overall_length,
                                       int style,
                                       ACMatchClassifications* classifications);

  
  
  static ACMatchClassifications MergeClassifications(
      const ACMatchClassifications& classifications1,
      const ACMatchClassifications& classifications2);

  
  
  static std::string ClassificationsToString(
      const ACMatchClassifications& classifications);
  static ACMatchClassifications ClassificationsFromString(
      const std::string& serialized_classifications);

  
  
  
  static void AddLastClassificationIfNecessary(
      ACMatchClassifications* classifications,
      size_t offset,
      int style);

  
  
  
  static base::string16 SanitizeString(const base::string16& text);

  
  
  static bool IsSearchType(Type type);

  
  
  
  
  
  
  
  
  
  
  void ComputeStrippedDestinationURL(Profile* profile);

  
  
  
  
  
  
  
  
  
  
  
  
  void GetKeywordUIState(Profile* profile,
                         base::string16* keyword,
                         bool* is_keyword_hint) const;

  
  
  
  
  
  
  base::string16 GetSubstitutingExplicitlyInvokedKeyword(
      Profile* profile) const;

  
  
  
  
  
  
  TemplateURL* GetTemplateURL(Profile* profile,
                              bool allow_fallback_to_destination_host) const;

  
  void RecordAdditionalInfo(const std::string& property,
                            const std::string& value);
  void RecordAdditionalInfo(const std::string& property, int value);
  void RecordAdditionalInfo(const std::string& property,
                            const base::Time& value);

  
  
  std::string GetAdditionalInfo(const std::string& property) const;

  
  
  
  
  
  
  
  
  bool IsVerbatimType() const;

  
  
  
  AutocompleteProvider* provider;

  
  
  
  
  
  
  
  int relevance;

  
  
  
  
  int typed_count;

  
  bool deletable;

  
  
  
  base::string16 fill_into_edit;

  
  
  base::string16 inline_autocompletion;

  
  
  
  
  
  
  
  
  
  bool allowed_to_be_default_match;

  
  
  
  GURL destination_url;

  
  GURL stripped_destination_url;

  
  base::string16 contents;
  ACMatchClassifications contents_class;

  
  base::string16 description;
  ACMatchClassifications description_class;

  
  
  
  content::PageTransition transition;

  
  
  bool is_history_what_you_typed_match;

  
  Type type;

  
  
  
  scoped_ptr<AutocompleteMatch> associated_keyword;

  
  
  
  
  
  
  
  
  
  
  
  base::string16 keyword;

  
  bool starred;

  
  bool from_previous;

  
  
  
  
  
  
  
  scoped_ptr<TemplateURLRef::SearchTermsArgs> search_terms_args;

  
  
  AdditionalInfo additional_info;

#ifndef NDEBUG
  
  void Validate() const;

  
  void ValidateClassifications(
      const base::string16& text,
      const ACMatchClassifications& classifications) const;
#endif
};

typedef AutocompleteMatch::ACMatchClassification ACMatchClassification;
typedef std::vector<ACMatchClassification> ACMatchClassifications;
typedef std::vector<AutocompleteMatch> ACMatches;

#endif  
