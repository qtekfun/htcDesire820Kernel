// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_CONSTANTS_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_CONSTANTS_H_

#include "base/basictypes.h"
#include "base/time/time.h"

namespace performance_monitor {


extern const char kMetricNotFoundError[];
extern const char kProcessChromeAggregate[];

extern const char kStateChromeVersion[];
extern const char kStateProfilePrefix[];

const int kSampleIntervalInSeconds = 10;
const int kDefaultGatherIntervalInSeconds = 120;


const int64 kBytesPerKilobyte = 1 << 10;
const int64 kBytesPerMegabyte = kBytesPerKilobyte * (1 << 10);
const int64 kBytesPerGigabyte = kBytesPerMegabyte * (1 << 10);
const int64 kBytesPerTerabyte = kBytesPerGigabyte * (1 << 10);

const int64 kMicrosecondsPerMonth = base::Time::kMicrosecondsPerDay * 30;
const int64 kMicrosecondsPerYear = base::Time::kMicrosecondsPerDay * 365;


const float kHighCPUUtilizationThreshold = 90.0f;
}  

#endif  
