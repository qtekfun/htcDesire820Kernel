// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_VISITSEGMENT_DATABASE_H_
#define CHROME_BROWSER_HISTORY_VISITSEGMENT_DATABASE_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/history/history_types.h"

class PageUsageData;

namespace sql {
class Connection;
}

namespace history {

class VisitSegmentDatabase {
 public:
  
  VisitSegmentDatabase();
  virtual ~VisitSegmentDatabase();

  
  
  static std::string ComputeSegmentName(const GURL& url);

  
  
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

  
  
  void DeleteSegmentData(base::Time older_than);

  
  void SetSegmentPresentationIndex(SegmentID segment_id, int index);

  
  
  bool DeleteSegmentForURL(URLID url_id);

 protected:
  
  virtual sql::Connection& GetDB() = 0;

  
  
  bool InitSegmentTables();

  
  bool DropSegmentTables();

 private:
  DISALLOW_COPY_AND_ASSIGN(VisitSegmentDatabase);
};

}  

#endif  
