// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_VISIT_DATABASE_H_
#define CHROME_BROWSER_HISTORY_VISIT_DATABASE_H_

#include <vector>

#include "chrome/browser/history/history_types.h"

namespace sql {
class Connection;
class Statement;
}

namespace history {

class VisitFilter;

class VisitDatabase {
 public:
  
  
  VisitDatabase();
  virtual ~VisitDatabase();

  
  
  
  bool DropVisitTable();

  
  
  
  
  VisitID AddVisit(VisitRow* visit, VisitSource source);

  
  
  void DeleteVisit(const VisitRow& visit);

  
  
  bool GetRowForVisit(VisitID visit_id, VisitRow* out_visit);

  
  
  bool UpdateVisitRow(const VisitRow& visit);

  
  
  
  bool GetVisitsForURL(URLID url_id, VisitVector* visits);

  
  
  
  
  
  
  bool GetVisibleVisitsForURL(URLID url_id,
                              const QueryOptions& options,
                              VisitVector* visits);

  
  
  
  
  
  bool GetVisitsForTimes(const std::vector<base::Time>& times,
                         VisitVector* visits);

  
  
  
  
  
  
  
  
  
  bool GetAllVisitsInRange(base::Time begin_time, base::Time end_time,
                           int max_results, VisitVector* visits);

  
  
  
  
  
  
  
  
  
  bool GetVisitsInRangeForTransition(base::Time begin_time,
                                     base::Time end_time,
                                     int max_results,
                                     content::PageTransition transition,
                                     VisitVector* visits);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetVisibleVisitsInRange(const QueryOptions& options,
                               VisitVector* visits);

  
  
  
  
  
  
  
  void GetDirectVisitsDuringTimes(const VisitFilter& time_filter,
                                   int max_count,
                                   VisitVector* visits);

  
  
  
  
  
  VisitID GetMostRecentVisitForURL(URLID url_id,
                                   VisitRow* visit_row);

  
  
  
  
  
  bool GetMostRecentVisitsForURL(URLID url_id,
                                 int max_results,
                                 VisitVector* visits);

  
  
  
  
  
  
  
  
  
  
  bool GetRedirectFromVisit(VisitID from_visit,
                            VisitID* to_visit,
                            GURL* to_url);

  
  
  bool GetRedirectToVisit(VisitID to_visit,
                          VisitID* from_visit,
                          GURL* from_url);

  
  
  
  
  
  
  bool GetVisibleVisitCountToHost(const GURL& url,
                                  int* count,
                                  base::Time* first_visit);

  
  bool GetStartDate(base::Time* first_visit);

  
  void GetVisitsSource(const VisitVector& visits,
                       VisitSourceMap* sources);

  
  
  void GetBriefVisitInfoOfMostRecentVisits(
      int max_visits,
      std::vector<BriefVisitInfo>* result_vector);

 protected:
  
  virtual sql::Connection& GetDB() = 0;

  
  
  bool InitVisitTable();

  
  
  static void FillVisitRow(sql::Statement& statement, VisitRow* visit);

  
  
  static bool FillVisitVector(sql::Statement& statement, VisitVector* visits);

  
  
  
  
  static bool FillVisitVectorWithOptions(sql::Statement& statement,
                                         const QueryOptions& options,
                                         VisitVector* visits);

  
  
  bool MigrateVisitsWithoutDuration();

 private:

  DISALLOW_COPY_AND_ASSIGN(VisitDatabase);
};

#define HISTORY_VISIT_ROW_FIELDS \
    " id,url,visit_time,from_visit,transition,segment_id,visit_duration "

}  

#endif  
