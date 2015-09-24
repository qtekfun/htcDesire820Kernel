// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_METRICS_VARIATIONS_EXPERIMENT_LABELS_H_
#define CHROME_COMMON_METRICS_VARIATIONS_EXPERIMENT_LABELS_H_

#include "base/metrics/field_trial.h"
#include "base/strings/string16.h"

namespace chrome_variations {

base::string16 BuildGoogleUpdateExperimentLabel(
    const base::FieldTrial::ActiveGroups& active_groups);

base::string16 CombineExperimentLabels(const base::string16& variation_labels,
                                       const base::string16& other_labels);

base::string16 ExtractNonVariationLabels(const base::string16& labels);

}  

#endif  
