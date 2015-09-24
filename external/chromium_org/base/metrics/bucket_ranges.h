// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_BUCKET_RANGES_H_
#define BASE_METRICS_BUCKET_RANGES_H_

#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/metrics/histogram_base.h"

namespace base {

class BASE_EXPORT BucketRanges {
 public:
  typedef std::vector<HistogramBase::Sample> Ranges;

  explicit BucketRanges(size_t num_ranges);
  ~BucketRanges();

  size_t size() const { return ranges_.size(); }
  HistogramBase::Sample range(size_t i) const { return ranges_[i]; }
  void set_range(size_t i, HistogramBase::Sample value);
  uint32 checksum() const { return checksum_; }
  void set_checksum(uint32 checksum) { checksum_ = checksum; }

  
  
  
  
  size_t bucket_count() const { return ranges_.size() - 1; }

  
  
  uint32 CalculateChecksum() const;
  bool HasValidChecksum() const;
  void ResetChecksum();

  
  bool Equals(const BucketRanges* other) const;

 private:
  
  
  
  Ranges ranges_;

  
  
  
  
  
  uint32 checksum_;

  DISALLOW_COPY_AND_ASSIGN(BucketRanges);
};

BASE_EXPORT_PRIVATE extern const uint32 kCrcTable[256];

}  

#endif  
