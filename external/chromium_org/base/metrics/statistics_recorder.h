// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_METRICS_STATISTICS_RECORDER_H_
#define BASE_METRICS_STATISTICS_RECORDER_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/lazy_instance.h"

namespace base {

class BucketRanges;
class HistogramBase;
class Lock;

class BASE_EXPORT StatisticsRecorder {
 public:
  typedef std::vector<HistogramBase*> Histograms;

  
  static void Initialize();

  
  static bool IsActive();

  
  
  
  
  static HistogramBase* RegisterOrDeleteDuplicate(HistogramBase* histogram);

  
  
  
  
  static const BucketRanges* RegisterOrDeleteDuplicateRanges(
      const BucketRanges* ranges);

  
  // have |query| as a substring are written to |output| (an empty string will
  
  static void WriteHTMLGraph(const std::string& query, std::string* output);
  static void WriteGraph(const std::string& query, std::string* output);

  
  
  static std::string ToJSON(const std::string& query);

  
  static void GetHistograms(Histograms* output);

  
  static void GetBucketRanges(std::vector<const BucketRanges*>* output);

  
  
  static HistogramBase* FindHistogram(const std::string& name);

  
  
  
  
  static void GetSnapshot(const std::string& query, Histograms* snapshot);

 private:
  
  typedef std::map<std::string, HistogramBase*> HistogramMap;

  
  
  
  typedef std::map<uint32, std::list<const BucketRanges*>*> RangesMap;

  friend struct DefaultLazyInstanceTraits<StatisticsRecorder>;
  friend class HistogramBaseTest;
  friend class HistogramTest;
  friend class SparseHistogramTest;
  friend class StatisticsRecorderTest;
  FRIEND_TEST_ALL_PREFIXES(HistogramDeltaSerializationTest,
                           DeserializeHistogramAndAddSamples);

  
  
  
  StatisticsRecorder();
  ~StatisticsRecorder();

  static void DumpHistogramsToVlog(void* instance);

  static HistogramMap* histograms_;
  static RangesMap* ranges_;

  
  static base::Lock* lock_;

  DISALLOW_COPY_AND_ASSIGN(StatisticsRecorder);
};

}  

#endif  
