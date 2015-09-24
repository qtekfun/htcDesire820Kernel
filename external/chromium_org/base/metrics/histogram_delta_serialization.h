// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_DELTA_SERIALIZATION_H_
#define BASE_METRICS_HISTOGRAM_DELTA_SERIALIZATION_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/histogram_flattener.h"
#include "base/metrics/histogram_snapshot_manager.h"

namespace base {

class HistogramBase;

class BASE_EXPORT HistogramDeltaSerialization : public HistogramFlattener {
 public:
  
  explicit HistogramDeltaSerialization(const std::string& caller_name);
  virtual ~HistogramDeltaSerialization();

  
  
  
  
  void PrepareAndSerializeDeltas(std::vector<std::string>* serialized_deltas);

  
  
  static void DeserializeAndAddSamples(
      const std::vector<std::string>& serialized_deltas);

 private:
  
  virtual void RecordDelta(const HistogramBase& histogram,
                           const HistogramSamples& snapshot) OVERRIDE;
  virtual void InconsistencyDetected(
      HistogramBase::Inconsistency problem) OVERRIDE;
  virtual void UniqueInconsistencyDetected(
      HistogramBase::Inconsistency problem) OVERRIDE;
  virtual void InconsistencyDetectedInLoggedCount(int amount) OVERRIDE;

  
  HistogramSnapshotManager histogram_snapshot_manager_;

  
  std::vector<std::string>* serialized_deltas_;

  
  HistogramBase* inconsistencies_histogram_;
  HistogramBase* inconsistencies_unique_histogram_;
  HistogramBase* inconsistent_snapshot_histogram_;

  DISALLOW_COPY_AND_ASSIGN(HistogramDeltaSerialization);
};

}  

#endif  
