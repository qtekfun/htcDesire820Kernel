// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_UMA_HISTOGRAM_HELPER_H_
#define CHROME_TEST_BASE_UMA_HISTOGRAM_HELPER_H_

#include "base/metrics/histogram.h"
#include "base/metrics/histogram_base.h"
#include "base/metrics/histogram_samples.h"

class UMAHistogramHelper {
 public:
  UMAHistogramHelper();

  
  
  void Fetch();

  
  
  void ExpectUniqueSample(const std::string& name,
                          base::HistogramBase::Sample sample,
                          base::HistogramBase::Count expected_count);

  
  void ExpectTotalCount(const std::string& name,
                        base::HistogramBase::Count count);

 private:
  void FetchCallback();

  void CheckBucketCount(const std::string& name,
                        base::HistogramBase::Sample sample,
                        base::Histogram::Count expected_count,
                        base::HistogramSamples& samples);

  void CheckTotalCount(const std::string& name,
                       base::Histogram::Count expected_count,
                       base::HistogramSamples& samples);

  DISALLOW_COPY_AND_ASSIGN(UMAHistogramHelper);
};

#endif  
