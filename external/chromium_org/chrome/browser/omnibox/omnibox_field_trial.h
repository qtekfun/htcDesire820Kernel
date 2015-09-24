// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_OMNIBOX_OMNIBOX_FIELD_TRIAL_H_
#define CHROME_BROWSER_OMNIBOX_OMNIBOX_FIELD_TRIAL_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/common/autocomplete_match_type.h"

class OmniboxFieldTrial {
 public:
  
  
  
  typedef std::map<AutocompleteMatchType::Type, float> DemotionMultipliers;

  
  typedef std::set<AutocompleteMatchType::Type> UndemotableTopMatchTypes;

  
  
  static void ActivateStaticTrials();

  
  
  
  
  
  
  static void ActivateDynamicTrials();

  
  
  
  
  
  
  static int GetDisabledProviderTypes();

  
  
  static bool HasDynamicFieldTrialGroupPrefix(const char *group_prefix);

  
  

  
  
  static void GetActiveSuggestFieldTrialHashes(
      std::vector<uint32>* field_trial_hash);

  
  

  
  
  static bool InHUPCullRedirectsFieldTrial();

  
  
  static bool InHUPCullRedirectsFieldTrialExperimentGroup();

  
  
  

  
  
  static bool InHUPCreateShorterMatchFieldTrial();

  
  
  static bool InHUPCreateShorterMatchFieldTrialExperimentGroup();

  
  

  
  
  
  
  
  
  
  static bool InStopTimerFieldTrialExperimentGroup();

  
  

  
  
  
  static bool InZeroSuggestFieldTrial();

  
  
  
  static bool InZeroSuggestMostVisitedFieldTrial();

  
  
  static bool InZeroSuggestAfterTypingFieldTrial();

  
  
  

  
  
  
  
  
  
  
  
  
  
  static bool ShortcutsScoringMaxRelevance(
      AutocompleteInput::PageClassification current_page_classification,
      int* max_relevance);

  
  
  

  
  
  
  static bool SearchHistoryPreventInlining(
      AutocompleteInput::PageClassification current_page_classification);

  
  
  
  static bool SearchHistoryDisable(
      AutocompleteInput::PageClassification current_page_classification);

  
  
  

  
  
  
  
  static void GetDemotionsByType(
      AutocompleteInput::PageClassification current_page_classification,
      DemotionMultipliers* demotions_by_type);

  
  
  static UndemotableTopMatchTypes GetUndemotableTopTypes(
      AutocompleteInput::PageClassification current_page_classification);

  
  
  

  
  
  
  
  
  
  static bool ReorderForLegalDefaultMatch(
      AutocompleteInput::PageClassification current_page_classification);

  
  
  

  
  
  
  
  static int HQPBookmarkValue();

  
  
  

  
  
  
  
  static bool HQPDiscountFrecencyWhenFewVisits();

  
  
  

  
  
  
  static bool HQPAllowMatchInTLDValue();

  
  
  

  
  
  
  static bool HQPAllowMatchInSchemeValue();

  
  
  static const char kBundledExperimentFieldTrialName[];
  
  static const char kShortcutsScoringMaxRelevanceRule[];
  static const char kSearchHistoryRule[];
  static const char kDemoteByTypeRule[];
  static const char kUndemotableTopTypeRule[];
  static const char kReorderForLegalDefaultMatchRule[];
  static const char kHQPBookmarkValueRule[];
  static const char kHQPDiscountFrecencyWhenFewVisitsRule[];
  static const char kHQPAllowMatchInTLDRule[];
  static const char kHQPAllowMatchInSchemeRule[];
  
  static const char kReorderForLegalDefaultMatchRuleDisabled[];

 private:
  friend class OmniboxFieldTrialTest;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static std::string GetValueForRuleInContext(
      const std::string& rule,
      AutocompleteInput::PageClassification page_classification);

  DISALLOW_IMPLICIT_CONSTRUCTORS(OmniboxFieldTrial);
};

#endif  
