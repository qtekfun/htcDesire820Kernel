// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_SAMPLES_H_
#define BASE_METRICS_HISTOGRAM_SAMPLES_H_

#include "base/basictypes.h"
#include "base/metrics/histogram_base.h"
#include "base/memory/scoped_ptr.h"

class Pickle;
class PickleIterator;

namespace base {

class SampleCountIterator;

class BASE_EXPORT HistogramSamples {
 public:
  HistogramSamples();
  virtual ~HistogramSamples();

  virtual void Accumulate(HistogramBase::Sample value,
                          HistogramBase::Count count) = 0;
  virtual HistogramBase::Count GetCount(HistogramBase::Sample value) const = 0;
  virtual HistogramBase::Count TotalCount() const = 0;

  virtual void Add(const HistogramSamples& other);

  
  virtual bool AddFromPickle(PickleIterator* iter);

  virtual void Subtract(const HistogramSamples& other);

  virtual scoped_ptr<SampleCountIterator> Iterator() const = 0;
  virtual bool Serialize(Pickle* pickle) const;

  
  int64 sum() const { return sum_; }
  HistogramBase::Count redundant_count() const { return redundant_count_; }

 protected:
  
  enum Operator { ADD, SUBTRACT };
  virtual bool AddSubtractImpl(SampleCountIterator* iter, Operator op) = 0;

  void IncreaseSum(int64 diff);
  void IncreaseRedundantCount(HistogramBase::Count diff);

 private:
  int64 sum_;

  
  
  
  
  
  
  
  HistogramBase::Count redundant_count_;
};

class BASE_EXPORT SampleCountIterator {
 public:
  virtual ~SampleCountIterator();

  virtual bool Done() const = 0;
  virtual void Next() = 0;

  
  
  
  virtual void Get(HistogramBase::Sample* min,
                   HistogramBase::Sample* max,
                   HistogramBase::Count* count) const = 0;

  
  
  
  virtual bool GetBucketIndex(size_t* index) const;
};

}  

#endif  
