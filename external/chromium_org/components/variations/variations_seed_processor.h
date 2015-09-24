// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VARIATIONS_VARIATIONS_SEED_PROCESSOR_H_
#define COMPONENTS_VARIATIONS_VARIATIONS_SEED_PROCESSOR_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/metrics/field_trial.h"
#include "base/time/time.h"
#include "base/version.h"
#include "components/variations/proto/study.pb.h"
#include "components/variations/proto/variations_seed.pb.h"

namespace chrome_variations {

class ProcessedStudy;

class VariationsSeedProcessor {
 public:
  VariationsSeedProcessor();
  virtual ~VariationsSeedProcessor();

  
  
  void CreateTrialsFromSeed(const VariationsSeed& seed,
                            const std::string& locale,
                            const base::Time& reference_date,
                            const base::Version& version,
                            Study_Channel channel,
                            Study_FormFactor form_factor);

 private:
  friend class VariationsSeedProcessorTest;
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           AllowForceGroupAndVariationId);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           AllowVariationIdWithForcingFlag);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, CheckStudyChannel);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, CheckStudyFormFactor);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, CheckStudyLocale);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, CheckStudyPlatform);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, CheckStudyStartDate);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, CheckStudyVersion);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           FilterAndValidateStudies);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           ForbidForceGroupWithVariationId);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, ForceGroupWithFlag1);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, ForceGroupWithFlag2);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           ForceGroup_ChooseFirstGroupWithFlag);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           ForceGroup_DontChooseGroupWithFlag);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, IsStudyExpired);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, ValidateStudy);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest, VariationParams);
  FRIEND_TEST_ALL_PREFIXES(VariationsSeedProcessorTest,
                           VariationParamsWithForcingFlag);

  
  
  
  bool AllowVariationIdWithForcingFlag(const Study& study);

  
  
  
  void FilterAndValidateStudies(const VariationsSeed& seed,
                                const std::string& locale,
                                const base::Time& reference_date,
                                const base::Version& version,
                                Study_Channel channel,
                                Study_FormFactor form_factor,
                                std::vector<ProcessedStudy>* filtered_studies);

  
  
  void ValidateAndAddStudy(const Study& study,
                           bool is_expired,
                           std::vector<ProcessedStudy>* filtered_studies);

  
  bool CheckStudyChannel(const Study_Filter& filter, Study_Channel channel);

  
  
  bool CheckStudyFormFactor(const Study_Filter& filter,
                            Study_FormFactor form_factor);

  
  bool CheckStudyLocale(const Study_Filter& filter, const std::string& locale);

  
  bool CheckStudyPlatform(const Study_Filter& filter, Study_Platform platform);

  
  bool CheckStudyStartDate(const Study_Filter& filter,
                           const base::Time& date_time);

  
  bool CheckStudyVersion(const Study_Filter& filter,
                         const base::Version& version);

  
  
  void CreateTrialFromStudy(const ProcessedStudy& processed_study);

  
  bool IsStudyExpired(const Study& study, const base::Time& date_time);

  
  
  
  
  bool ShouldAddStudy(const Study& study,
                      const std::string& locale,
                      const base::Time& reference_date,
                      const base::Version& version,
                      Study_Channel channel,
                      Study_FormFactor form_factor);

  
  bool ValidateStudyAndComputeTotalProbability(
      const Study& study,
      base::FieldTrial::Probability* total_probability);

  DISALLOW_COPY_AND_ASSIGN(VariationsSeedProcessor);
};

}  

#endif  
