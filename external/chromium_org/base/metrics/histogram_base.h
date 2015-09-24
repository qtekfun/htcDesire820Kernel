// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_HISTOGRAM_BASE_H_
#define BASE_METRICS_HISTOGRAM_BASE_H_

#include <string>
#include <vector>

#include "base/atomicops.h"
#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"

class Pickle;
class PickleIterator;

namespace base {

class DictionaryValue;
class HistogramBase;
class HistogramSamples;
class ListValue;


enum BASE_EXPORT HistogramType {
  HISTOGRAM,
  LINEAR_HISTOGRAM,
  BOOLEAN_HISTOGRAM,
  CUSTOM_HISTOGRAM,
  SPARSE_HISTOGRAM,
};

std::string HistogramTypeToString(HistogramType type);

BASE_EXPORT_PRIVATE HistogramBase* DeserializeHistogramInfo(
    PickleIterator* iter);


class BASE_EXPORT HistogramBase {
 public:
  typedef int Sample;                   
  typedef subtle::Atomic32 Count;     

  static const Sample kSampleType_MAX;  

  enum Flags {
    kNoFlags = 0,
    kUmaTargetedHistogramFlag = 0x1,  

    
    
    
    
    
    kIPCSerializationSourceFlag = 0x10,

    
    kHexRangePrintingFlag = 0x8000,
  };

  
  enum Inconsistency {
    NO_INCONSISTENCIES = 0x0,
    RANGE_CHECKSUM_ERROR = 0x1,
    BUCKET_ORDER_ERROR = 0x2,
    COUNT_HIGH_ERROR = 0x4,
    COUNT_LOW_ERROR = 0x8,

    NEVER_EXCEEDED_VALUE = 0x10
  };

  explicit HistogramBase(const std::string& name);
  virtual ~HistogramBase();

  std::string histogram_name() const { return histogram_name_; }

  
  int32 flags() const { return flags_; }
  void SetFlags(int32 flags);
  void ClearFlags(int32 flags);

  virtual HistogramType GetHistogramType() const = 0;

  
  
  
  virtual bool HasConstructionArguments(Sample expected_minimum,
                                        Sample expected_maximum,
                                        size_t expected_bucket_count) const = 0;

  virtual void Add(Sample value) = 0;

  
  void AddTime(const TimeDelta& time);
  void AddBoolean(bool value);

  virtual void AddSamples(const HistogramSamples& samples) = 0;
  virtual bool AddSamplesFromPickle(PickleIterator* iter) = 0;

  
  
  
  bool SerializeInfo(Pickle* pickle) const;

  
  
  virtual int FindCorruption(const HistogramSamples& samples) const;

  
  
  virtual scoped_ptr<HistogramSamples> SnapshotSamples() const = 0;

  
  virtual void WriteHTMLGraph(std::string* output) const = 0;
  virtual void WriteAscii(std::string* output) const = 0;

  
  
  
  void WriteJSON(std::string* output) const;

protected:
  
  virtual bool SerializeInfoImpl(Pickle* pickle) const = 0;

  
  virtual void GetParameters(DictionaryValue* params) const = 0;

  
  
  
  virtual void GetCountAndBucketData(Count* count,
                                     int64* sum,
                                     ListValue* buckets) const = 0;

  
  void WriteAsciiBucketGraph(double current_size,
                             double max_size,
                             std::string* output) const;

  
  const std::string GetSimpleAsciiBucketRange(Sample sample) const;

  
  
  void WriteAsciiBucketValue(Count current,
                             double scaled_sum,
                             std::string* output) const;

 private:
  const std::string histogram_name_;
  int32 flags_;

  DISALLOW_COPY_AND_ASSIGN(HistogramBase);
};

}  

#endif  
