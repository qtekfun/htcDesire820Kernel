// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VARIATIONS_PROCESSED_STUDY_H_
#define COMPONENTS_VARIATIONS_PROCESSED_STUDY_H_

#include <vector>

#include "base/metrics/field_trial.h"

namespace chrome_variations {

class Study;

class ProcessedStudy {
 public:
  ProcessedStudy();
  ~ProcessedStudy();

  bool Init(const Study* study, bool is_expired);

  const Study* study() const { return study_; }

  base::FieldTrial::Probability total_probability() const {
    return total_probability_;
  }

  bool is_expired() const { return is_expired_; }

  static bool ValidateAndAppendStudy(
      const Study* study,
      bool is_expired,
      std::vector<ProcessedStudy>* processed_studies);

 private:
  
  const Study* study_;

  
  base::FieldTrial::Probability total_probability_;

  
  bool is_expired_;
};

}  

#endif  
