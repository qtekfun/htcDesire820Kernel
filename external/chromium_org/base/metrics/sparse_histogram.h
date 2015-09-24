// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_SPARSE_HISTOGRAM_H_
#define BASE_METRICS_SPARSE_HISTOGRAM_H_

#include <map>
#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/histogram_base.h"
#include "base/metrics/sample_map.h"
#include "base/synchronization/lock.h"

namespace base {

#define HISTOGRAM_SPARSE_COMMON(name, sample, flag) \
    do { \
      base::HistogramBase* histogram( \
          base::SparseHistogram::FactoryGet(name, flag)); \
      DCHECK_EQ(histogram->histogram_name(), name); \
      histogram->Add(sample); \
    } while (0)

#define HISTOGRAM_SPARSE_SLOWLY(name, sample) \
    HISTOGRAM_SPARSE_COMMON(name, sample, base::HistogramBase::kNoFlags)

#define UMA_HISTOGRAM_SPARSE_SLOWLY(name, sample) \
    HISTOGRAM_SPARSE_COMMON(name, sample, \
                            base::HistogramBase::kUmaTargetedHistogramFlag)

#ifndef NDEBUG

#define DHISTOGRAM_SPARSE_SLOWLY(name, sample) \
    HISTOGRAM_SPARSE_SLOWLY(name, sample)

#else  

#define DHISTOGRAM_SPARSE_SLOWLY(name, sample) \
    while (0) { \
      static_cast<void>(name); \
      static_cast<void>(sample); \
    }

#endif  

class HistogramSamples;

class BASE_EXPORT_PRIVATE SparseHistogram : public HistogramBase {
 public:
  
  
  static HistogramBase* FactoryGet(const std::string& name, int32 flags);

  virtual ~SparseHistogram();

  
  virtual HistogramType GetHistogramType() const OVERRIDE;
  virtual bool HasConstructionArguments(
      Sample expected_minimum,
      Sample expected_maximum,
      size_t expected_bucket_count) const OVERRIDE;
  virtual void Add(Sample value) OVERRIDE;
  virtual void AddSamples(const HistogramSamples& samples) OVERRIDE;
  virtual bool AddSamplesFromPickle(PickleIterator* iter) OVERRIDE;
  virtual scoped_ptr<HistogramSamples> SnapshotSamples() const OVERRIDE;
  virtual void WriteHTMLGraph(std::string* output) const OVERRIDE;
  virtual void WriteAscii(std::string* output) const OVERRIDE;

 protected:
  
  virtual bool SerializeInfoImpl(Pickle* pickle) const OVERRIDE;

 private:
  
  explicit SparseHistogram(const std::string& name);

  friend BASE_EXPORT_PRIVATE HistogramBase* DeserializeHistogramInfo(
      PickleIterator* iter);
  static HistogramBase* DeserializeInfoImpl(PickleIterator* iter);

  virtual void GetParameters(DictionaryValue* params) const OVERRIDE;
  virtual void GetCountAndBucketData(Count* count,
                                     int64* sum,
                                     ListValue* buckets) const OVERRIDE;

  
  void WriteAsciiImpl(bool graph_it,
                      const std::string& newline,
                      std::string* output) const;

  
  void WriteAsciiHeader(const Count total_count,
                        std::string* output) const;

  
  friend class SparseHistogramTest;

  
  mutable base::Lock lock_;

  SampleMap samples_;

  DISALLOW_COPY_AND_ASSIGN(SparseHistogram);
};

}  

#endif  
