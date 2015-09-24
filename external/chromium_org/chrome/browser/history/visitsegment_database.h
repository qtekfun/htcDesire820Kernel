// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_VISITSEGMENT_DATABASE_H_
#define CHROME_BROWSER_HISTORY_VISITSEGMENT_DATABASE_H_

#include "base/basictypes.h"
#include "chrome/browser/history/history_types.h"

class PageUsageData;

namespace sql {
class Connection;
class Statement;
}

namespace history {

class VisitSegmentDatabase {
 public:
  
  VisitSegmentDatabase();
  virtual ~VisitSegmentDatabase();

  
  
  static std::string ComputeSegmentName(const GURL& url);

  
  
  static base::Time SegmentTime(base::Time time);

  
  
  SegmentID GetSegmentNamed(const std::string& segment_name);

  
  
  
  bool UpdateSegmentRepresentationURL(SegmentID segment_id,
                                      URLID url_id);

  
  
  URLID GetSegmentRepresentationURL(SegmentID segment_id);

  
  
  SegmentID CreateSegment(URLID url_id, const std::string& segment_name);

  
  
  bool IncreaseSegmentVisitCount(SegmentID segment_id, base::Time ts,
                                 int amount);

  
  
  
  void QuerySegmentUsage(base::Time from_time,
                         int max_result_count,
                         std::vector<PageUsageData*>* result);

  
  
  bool DeleteSegmentData(base::Time older_than);

  
  bool SetSegmentPresentationIndex(SegmentID segment_id, int index);

  
  
  bool DeleteSegmentForURL(URLID url_id);

  
  
  SegmentDurationID CreateSegmentDuration(SegmentID segment_id,
                                          base::Time time,
                                          base::TimeDelta delta);

  
  bool SetSegmentDuration(SegmentDurationID duration_id,
                          base::TimeDelta time_delta);

  
  
  bool GetSegmentDuration(SegmentID segment_id,
                          base::Time time,
                          SegmentDurationID* duration_id,
                          base::TimeDelta* time_delta);

  
  void QuerySegmentDuration(base::Time from_time,
                            int max_result_count,
                            std::vector<PageUsageData*>* result);

 protected:
  
  virtual sql::Connection& GetDB() = 0;

  
  
  bool InitSegmentTables();

  
  bool DropSegmentTables();

  
  
  bool MigratePresentationIndex();

 private:
  enum QueryType {
    QUERY_VISIT_COUNT,
    QUERY_DURATION,
  };

  
  void QuerySegmentsCommon(sql::Statement* statement,
                           base::Time from_time,
                           int max_result_count,
                           QueryType query_type,
                           std::vector<PageUsageData*>* result);

  
  const bool has_duration_table_;

  DISALLOW_COPY_AND_ASSIGN(VisitSegmentDatabase);
};

}  

#endif  
