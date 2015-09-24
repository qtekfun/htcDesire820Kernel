// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_METRICS_VARIATIONS_VARIATIONS_UTIL_H_
#define CHROME_COMMON_METRICS_VARIATIONS_VARIATIONS_UTIL_H_

#include <string>
#include <vector>

#include "base/metrics/field_trial.h"
#include "base/strings/string16.h"
#include "components/variations/variations_associated_data.h"

namespace chrome_variations {

void GetFieldTrialActiveGroupIds(std::vector<ActiveGroupId>* name_group_ids);

void GetFieldTrialActiveGroupIdsAsStrings(std::vector<std::string>* output);

void SetChildProcessLoggingVariationList();

namespace testing {

void ClearAllVariationIDs();

void TestGetFieldTrialActiveGroupIds(
    const base::FieldTrial::ActiveGroups& active_groups,
    std::vector<ActiveGroupId>* name_group_ids);

}  

}  

#endif  
