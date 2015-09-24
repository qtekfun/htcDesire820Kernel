// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.








#ifndef BASE_METRICS_HISTOGRAM_H_
#define BASE_METRICS_HISTOGRAM_H_

#include <map>
#include <string>
#include <vector>

#include "base/atomicops.h"
#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/bucket_ranges.h"
#include "base/metrics/histogram_base.h"
#include "base/metrics/histogram_samples.h"
#include "base/time/time.h"

class Pickle;
class PickleIterator;

namespace base {

class Lock;



#define STATIC_HISTOGRAM_POINTER_BLOCK(constant_histogram_name, \
                                       histogram_add_method_invocation, \
                                       histogram_factory_get_invocation) \
  do { \
    static base::subtle::AtomicWord atomic_histogram_pointer = 0; \
    base::HistogramBase* histogram_pointer( \
        reinterpret_cast<base::HistogramBase*>( \
            base::subtle::Acquire_Load(&atomic_histogram_pointer))); \
    if (!histogram_pointer) { \
      histogram_pointer = histogram_factory_get_invocation; \
      base::subtle::Release_Store(&atomic_histogram_pointer, \
          reinterpret_cast<base::subtle::AtomicWord>(histogram_pointer)); \
    } \
    DCHECK_EQ(histogram_pointer->histogram_name(), \
              std::string(constant_histogram_name)); \
    histogram_pointer->histogram_add_method_invocation; \
  } while (0)



#define HISTOGRAM_TIMES(name, sample) HISTOGRAM_CUSTOM_TIMES( \
    name, sample, base::TimeDelta::FromMilliseconds(1), \
    base::TimeDelta::FromSeconds(10), 50)

#define HISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, AddTime(sample), \
        base::Histogram::FactoryTimeGet(name, min, max, bucket_count, \
                                        base::HistogramBase::kNoFlags))

#define HISTOGRAM_COUNTS(name, sample) HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 1000000, 50)

#define HISTOGRAM_COUNTS_100(name, sample) HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 100, 50)

#define HISTOGRAM_COUNTS_10000(name, sample) HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 10000, 50)

#define HISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, Add(sample), \
        base::Histogram::FactoryGet(name, min, max, bucket_count, \
                                    base::HistogramBase::kNoFlags))

#define HISTOGRAM_PERCENTAGE(name, under_one_hundred) \
    HISTOGRAM_ENUMERATION(name, under_one_hundred, 101)

#define HISTOGRAM_BOOLEAN(name, sample) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, AddBoolean(sample), \
        base::BooleanHistogram::FactoryGet(name, base::Histogram::kNoFlags))

#define HISTOGRAM_ENUMERATION(name, sample, boundary_value) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, Add(sample), \
        base::LinearHistogram::FactoryGet(name, 1, boundary_value, \
            boundary_value + 1, base::HistogramBase::kNoFlags))

#define HISTOGRAM_CUSTOM_ENUMERATION(name, sample, custom_ranges) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, Add(sample), \
        base::CustomHistogram::FactoryGet(name, custom_ranges, \
                                          base::HistogramBase::kNoFlags))

#define HISTOGRAM_MEMORY_KB(name, sample) HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1000, 500000, 50)

#ifndef NDEBUG

#define DHISTOGRAM_TIMES(name, sample) HISTOGRAM_TIMES(name, sample)
#define DHISTOGRAM_COUNTS(name, sample) HISTOGRAM_COUNTS(name, sample)
#define DHISTOGRAM_PERCENTAGE(name, under_one_hundred) HISTOGRAM_PERCENTAGE(\
    name, under_one_hundred)
#define DHISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count) \
    HISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count)
#define DHISTOGRAM_CLIPPED_TIMES(name, sample, min, max, bucket_count) \
    HISTOGRAM_CLIPPED_TIMES(name, sample, min, max, bucket_count)
#define DHISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count) \
    HISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count)
#define DHISTOGRAM_ENUMERATION(name, sample, boundary_value) \
    HISTOGRAM_ENUMERATION(name, sample, boundary_value)
#define DHISTOGRAM_CUSTOM_ENUMERATION(name, sample, custom_ranges) \
    HISTOGRAM_CUSTOM_ENUMERATION(name, sample, custom_ranges)

#else  
#define DISCARD_2_ARGUMENTS(a, b) \
  while (0) { \
    static_cast<void>(a); \
    static_cast<void>(b); \
 }
#define DISCARD_3_ARGUMENTS(a, b, c) \
  while (0) { \
    static_cast<void>(a); \
    static_cast<void>(b); \
    static_cast<void>(c); \
 }
#define DISCARD_5_ARGUMENTS(a, b, c, d ,e) \
  while (0) { \
    static_cast<void>(a); \
    static_cast<void>(b); \
    static_cast<void>(c); \
    static_cast<void>(d); \
    static_cast<void>(e); \
 }
#define DHISTOGRAM_TIMES(name, sample) \
    DISCARD_2_ARGUMENTS(name, sample)

#define DHISTOGRAM_COUNTS(name, sample) \
    DISCARD_2_ARGUMENTS(name, sample)

#define DHISTOGRAM_PERCENTAGE(name, under_one_hundred) \
    DISCARD_2_ARGUMENTS(name, under_one_hundred)

#define DHISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count) \
    DISCARD_5_ARGUMENTS(name, sample, min, max, bucket_count)

#define DHISTOGRAM_CLIPPED_TIMES(name, sample, min, max, bucket_count) \
    DISCARD_5_ARGUMENTS(name, sample, min, max, bucket_count)

#define DHISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count) \
    DISCARD_5_ARGUMENTS(name, sample, min, max, bucket_count)

#define DHISTOGRAM_ENUMERATION(name, sample, boundary_value) \
    DISCARD_3_ARGUMENTS(name, sample, boundary_value)

#define DHISTOGRAM_CUSTOM_ENUMERATION(name, sample, custom_ranges) \
    DISCARD_3_ARGUMENTS(name, sample, custom_ranges)

#endif  


#define UMA_HISTOGRAM_TIMES(name, sample) UMA_HISTOGRAM_CUSTOM_TIMES( \
    name, sample, base::TimeDelta::FromMilliseconds(1), \
    base::TimeDelta::FromSeconds(10), 50)

#define UMA_HISTOGRAM_MEDIUM_TIMES(name, sample) UMA_HISTOGRAM_CUSTOM_TIMES( \
    name, sample, base::TimeDelta::FromMilliseconds(10), \
    base::TimeDelta::FromMinutes(3), 50)

#define UMA_HISTOGRAM_LONG_TIMES(name, sample) UMA_HISTOGRAM_CUSTOM_TIMES( \
    name, sample, base::TimeDelta::FromMilliseconds(1), \
    base::TimeDelta::FromHours(1), 50)

#define UMA_HISTOGRAM_LONG_TIMES_100(name, sample) UMA_HISTOGRAM_CUSTOM_TIMES( \
    name, sample, base::TimeDelta::FromMilliseconds(1), \
    base::TimeDelta::FromHours(1), 100)

#define UMA_HISTOGRAM_CUSTOM_TIMES(name, sample, min, max, bucket_count) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, AddTime(sample), \
        base::Histogram::FactoryTimeGet(name, min, max, bucket_count, \
            base::HistogramBase::kUmaTargetedHistogramFlag))

#define UMA_HISTOGRAM_COUNTS(name, sample) UMA_HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 1000000, 50)

#define UMA_HISTOGRAM_COUNTS_100(name, sample) UMA_HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 100, 50)

#define UMA_HISTOGRAM_COUNTS_10000(name, sample) UMA_HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 10000, 50)

#define UMA_HISTOGRAM_CUSTOM_COUNTS(name, sample, min, max, bucket_count) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, Add(sample), \
        base::Histogram::FactoryGet(name, min, max, bucket_count, \
            base::HistogramBase::kUmaTargetedHistogramFlag))

#define UMA_HISTOGRAM_MEMORY_KB(name, sample) UMA_HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1000, 500000, 50)

#define UMA_HISTOGRAM_MEMORY_MB(name, sample) UMA_HISTOGRAM_CUSTOM_COUNTS( \
    name, sample, 1, 1000, 50)

#define UMA_HISTOGRAM_PERCENTAGE(name, under_one_hundred) \
    UMA_HISTOGRAM_ENUMERATION(name, under_one_hundred, 101)

#define UMA_HISTOGRAM_BOOLEAN(name, sample) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, AddBoolean(sample), \
        base::BooleanHistogram::FactoryGet(name, \
            base::HistogramBase::kUmaTargetedHistogramFlag))

#define UMA_HISTOGRAM_ENUMERATION(name, sample, boundary_value) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, Add(sample), \
        base::LinearHistogram::FactoryGet(name, 1, boundary_value, \
            boundary_value + 1, base::HistogramBase::kUmaTargetedHistogramFlag))

#define UMA_HISTOGRAM_CUSTOM_ENUMERATION(name, sample, custom_ranges) \
    STATIC_HISTOGRAM_POINTER_BLOCK(name, Add(sample), \
        base::CustomHistogram::FactoryGet(name, custom_ranges, \
            base::HistogramBase::kUmaTargetedHistogramFlag))


class BucketRanges;
class SampleVector;

class BooleanHistogram;
class CustomHistogram;
class Histogram;
class LinearHistogram;

class BASE_EXPORT Histogram : public HistogramBase {
 public:
  
  static const size_t kBucketCount_MAX;

  typedef std::vector<Count> Counts;

  
  
  
  
  
  
  
  
  
  
  static HistogramBase* FactoryGet(const std::string& name,
                                   Sample minimum,
                                   Sample maximum,
                                   size_t bucket_count,
                                   int32 flags);
  static HistogramBase* FactoryTimeGet(const std::string& name,
                                       base::TimeDelta minimum,
                                       base::TimeDelta maximum,
                                       size_t bucket_count,
                                       int32 flags);

  
  
  static TimeTicks DebugNow();

  static void InitializeBucketRanges(Sample minimum,
                                     Sample maximum,
                                     BucketRanges* ranges);

  
  
  
  
  
  
  
  static const int kCommonRaceBasedCountMismatch;

  
  
  
  
  
  virtual int FindCorruption(const HistogramSamples& samples) const OVERRIDE;

  
  
  
  Sample declared_min() const { return declared_min_; }
  Sample declared_max() const { return declared_max_; }
  virtual Sample ranges(size_t i) const;
  virtual size_t bucket_count() const;
  const BucketRanges* bucket_ranges() const { return bucket_ranges_; }

  
  
  
  
  
  
  static bool InspectConstructionArguments(const std::string& name,
                                           Sample* minimum,
                                           Sample* maximum,
                                           size_t* bucket_count);

  
  virtual HistogramType GetHistogramType() const OVERRIDE;
  virtual bool HasConstructionArguments(
      Sample expected_minimum,
      Sample expected_maximum,
      size_t expected_bucket_count) const OVERRIDE;
  virtual void Add(Sample value) OVERRIDE;
  virtual scoped_ptr<HistogramSamples> SnapshotSamples() const OVERRIDE;
  virtual void AddSamples(const HistogramSamples& samples) OVERRIDE;
  virtual bool AddSamplesFromPickle(PickleIterator* iter) OVERRIDE;
  virtual void WriteHTMLGraph(std::string* output) const OVERRIDE;
  virtual void WriteAscii(std::string* output) const OVERRIDE;

 protected:
  
  
  Histogram(const std::string& name,
            Sample minimum,
            Sample maximum,
            const BucketRanges* ranges);

  virtual ~Histogram();

  
  virtual bool SerializeInfoImpl(Pickle* pickle) const OVERRIDE;

  
  virtual bool PrintEmptyBucket(size_t index) const;

  
  virtual double GetBucketSize(Count current, size_t i) const;

  
  
  
  virtual const std::string GetAsciiBucketRange(size_t it) const;

 private:
  
  FRIEND_TEST_ALL_PREFIXES(HistogramTest, BoundsTest);
  FRIEND_TEST_ALL_PREFIXES(HistogramTest, BucketPlacementTest);
  FRIEND_TEST_ALL_PREFIXES(HistogramTest, CorruptBucketBounds);
  FRIEND_TEST_ALL_PREFIXES(HistogramTest, CorruptSampleCounts);
  FRIEND_TEST_ALL_PREFIXES(HistogramTest, NameMatchTest);

  friend class StatisticsRecorder;  
  friend class StatisticsRecorderTest;

  friend BASE_EXPORT_PRIVATE HistogramBase* DeserializeHistogramInfo(
      PickleIterator* iter);
  static HistogramBase* DeserializeInfoImpl(PickleIterator* iter);

  
  scoped_ptr<SampleVector> SnapshotSampleVector() const;

  
  

  void WriteAsciiImpl(bool graph_it,
                      const std::string& newline,
                      std::string* output) const;

  
  double GetPeakBucketSize(const SampleVector& samples) const;

  
  void WriteAsciiHeader(const SampleVector& samples,
                        Count sample_count,
                        std::string* output) const;

  
  
  void WriteAsciiBucketContext(const int64 past, const Count current,
                               const int64 remaining, const size_t i,
                               std::string* output) const;

  
  virtual void GetParameters(DictionaryValue* params) const OVERRIDE;

  virtual void GetCountAndBucketData(Count* count,
                                     int64* sum,
                                     ListValue* buckets) const OVERRIDE;

  
  const BucketRanges* bucket_ranges_;

  Sample declared_min_;  
  Sample declared_max_;  

  
  
  scoped_ptr<SampleVector> samples_;

  DISALLOW_COPY_AND_ASSIGN(Histogram);
};


class BASE_EXPORT LinearHistogram : public Histogram {
 public:
  virtual ~LinearHistogram();

  static HistogramBase* FactoryGet(const std::string& name,
                                   Sample minimum,
                                   Sample maximum,
                                   size_t bucket_count,
                                   int32 flags);
  static HistogramBase* FactoryTimeGet(const std::string& name,
                                       TimeDelta minimum,
                                       TimeDelta maximum,
                                       size_t bucket_count,
                                       int32 flags);

  struct DescriptionPair {
    Sample sample;
    const char* description;  
  };

  
  
  
  
  
  static HistogramBase* FactoryGetWithRangeDescription(
      const std::string& name,
      Sample minimum,
      Sample maximum,
      size_t bucket_count,
      int32 flags,
      const DescriptionPair descriptions[]);

  static void InitializeBucketRanges(Sample minimum,
                                     Sample maximum,
                                     BucketRanges* ranges);

  
  virtual HistogramType GetHistogramType() const OVERRIDE;

 protected:
  LinearHistogram(const std::string& name,
                  Sample minimum,
                  Sample maximum,
                  const BucketRanges* ranges);

  virtual double GetBucketSize(Count current, size_t i) const OVERRIDE;

  
  
  virtual const std::string GetAsciiBucketRange(size_t i) const OVERRIDE;

  
  
  virtual bool PrintEmptyBucket(size_t index) const OVERRIDE;

 private:
  friend BASE_EXPORT_PRIVATE HistogramBase* DeserializeHistogramInfo(
      PickleIterator* iter);
  static HistogramBase* DeserializeInfoImpl(PickleIterator* iter);

  
  
  
  typedef std::map<Sample, std::string> BucketDescriptionMap;
  BucketDescriptionMap bucket_description_;

  DISALLOW_COPY_AND_ASSIGN(LinearHistogram);
};


class BASE_EXPORT BooleanHistogram : public LinearHistogram {
 public:
  static HistogramBase* FactoryGet(const std::string& name, int32 flags);

  virtual HistogramType GetHistogramType() const OVERRIDE;

 private:
  BooleanHistogram(const std::string& name, const BucketRanges* ranges);

  friend BASE_EXPORT_PRIVATE HistogramBase* DeserializeHistogramInfo(
      PickleIterator* iter);
  static HistogramBase* DeserializeInfoImpl(PickleIterator* iter);

  DISALLOW_COPY_AND_ASSIGN(BooleanHistogram);
};


class BASE_EXPORT CustomHistogram : public Histogram {
 public:
  
  
  
  
  static HistogramBase* FactoryGet(const std::string& name,
                                   const std::vector<Sample>& custom_ranges,
                                   int32 flags);

  
  virtual HistogramType GetHistogramType() const OVERRIDE;

  
  
  
  
  
  
  static std::vector<Sample> ArrayToCustomRanges(const Sample* values,
                                                 size_t num_values);
 protected:
  CustomHistogram(const std::string& name,
                  const BucketRanges* ranges);

  
  virtual bool SerializeInfoImpl(Pickle* pickle) const OVERRIDE;

  virtual double GetBucketSize(Count current, size_t i) const OVERRIDE;

 private:
  friend BASE_EXPORT_PRIVATE HistogramBase* DeserializeHistogramInfo(
      PickleIterator* iter);
  static HistogramBase* DeserializeInfoImpl(PickleIterator* iter);

  static bool ValidateCustomRanges(const std::vector<Sample>& custom_ranges);
  static BucketRanges* CreateBucketRangesFromCustomRanges(
      const std::vector<Sample>& custom_ranges);

  DISALLOW_COPY_AND_ASSIGN(CustomHistogram);
};

}  

#endif  
