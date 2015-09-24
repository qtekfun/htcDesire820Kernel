// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_METRICS_VARIATIONS_UNIFORMITY_FIELD_TRIALS_H_
#define CHROME_COMMON_METRICS_VARIATIONS_UNIFORMITY_FIELD_TRIALS_H_

namespace base {
class Time;
}

namespace chrome_variations {

void SetupUniformityFieldTrials(const base::Time install_date);

}  

#endif  
