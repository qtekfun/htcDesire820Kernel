// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_FLATTENER_H_
#define BASE_METRICS_HISTOGRAM_FLATTENER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/metrics/histogram.h"

namespace base {

class HistogramSamples;

class BASE_EXPORT HistogramFlattener {
 public:
  virtual void RecordDelta(const HistogramBase& histogram,
                           const HistogramSamples& snapshot) = 0;

  
  
  virtual void InconsistencyDetected(HistogramBase::Inconsistency problem) = 0;

  
  
  virtual void UniqueInconsistencyDetected(
      HistogramBase::Inconsistency problem) = 0;

  
  
  
  virtual void InconsistencyDetectedInLoggedCount(int amount) = 0;

 protected:
  HistogramFlattener() {}
  virtual ~HistogramFlattener() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(HistogramFlattener);
};

}  

#endif  
