// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_METRICS_METRICS_SERVICE_BASE_H_
#define CHROME_COMMON_METRICS_METRICS_SERVICE_BASE_H_

#include "base/basictypes.h"
#include "base/metrics/histogram_base.h"
#include "base/metrics/histogram_flattener.h"
#include "base/metrics/histogram_snapshot_manager.h"
#include "chrome/common/metrics/metrics_log_manager.h"

namespace base {
class HistogramSamples;
}  

class MetricsServiceBase : public base::HistogramFlattener {
 public:
  
  virtual void RecordDelta(const base::HistogramBase& histogram,
                           const base::HistogramSamples& snapshot) OVERRIDE;
  virtual void InconsistencyDetected(
      base::HistogramBase::Inconsistency problem) OVERRIDE;
  virtual void UniqueInconsistencyDetected(
      base::HistogramBase::Inconsistency problem) OVERRIDE;
  virtual void InconsistencyDetectedInLoggedCount(int amount) OVERRIDE;

 protected:
  MetricsServiceBase();
  virtual ~MetricsServiceBase();

  
  static const char kServerUrl[];

  
  static const char kMimeType[];

  
  
  void RecordCurrentHistograms();

  
  MetricsLogManager log_manager_;

 private:
  
  base::HistogramSnapshotManager histogram_snapshot_manager_;

  DISALLOW_COPY_AND_ASSIGN(MetricsServiceBase);
};

#endif  
