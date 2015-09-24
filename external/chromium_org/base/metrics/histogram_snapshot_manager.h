// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_SNAPSHOT_MANAGER_H_
#define BASE_METRICS_HISTOGRAM_SNAPSHOT_MANAGER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/metrics/histogram_base.h"

namespace base {

class HistogramSamples;
class HistogramFlattener;

class BASE_EXPORT HistogramSnapshotManager {
 public:
  explicit HistogramSnapshotManager(HistogramFlattener* histogram_flattener);
  virtual ~HistogramSnapshotManager();

  
  
  
  void PrepareDeltas(HistogramBase::Flags flags_to_set, bool record_only_uma);

 private:
  
  void PrepareDelta(const HistogramBase& histogram);

  
  void InspectLoggedSamplesInconsistency(
      const HistogramSamples& new_snapshot,
      HistogramSamples* logged_samples);

  
  
  std::map<std::string, HistogramSamples*> logged_samples_;

  
  std::map<std::string, int> inconsistencies_;

  
  
  HistogramFlattener* histogram_flattener_;  

  DISALLOW_COPY_AND_ASSIGN(HistogramSnapshotManager);
};

}  

#endif  
