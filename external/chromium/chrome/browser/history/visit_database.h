// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_VISIT_DATABASE_H_
#define CHROME_BROWSER_HISTORY_VISIT_DATABASE_H_
#pragma once

#include "chrome/browser/history/history_types.h"

namespace sql {
class Connection;
class Statement;
}

namespace history {

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

  
  
  
  
  
  
  
  
  
  void GetAllVisitsInRange(base::Time begin_time, base::Time end_time,
                           int max_results, VisitVector* visits);

  
  
  
  
  
  
  
  
  
  void GetVisitsInRangeForTransition(base::Time begin_time,
                                     base::Time end_time,
                                     int max_results,
                                     PageTransition::Type transition,
                                     VisitVector* visits);

  
  
  
  
  
  
  
  
  
  
  
  void GetVisibleVisitsInRange(base::Time begin_time, base::Time end_time,
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

  
  
  
  
  
  bool GetVisitCountToHost(const GURL& url, int* count,
                           base::Time* first_visit);

  
  bool GetStartDate(base::Time* first_visit);

  
  void GetVisitsSource(const VisitVector& visits,
                       VisitSourceMap* sources);

 protected:
  
  virtual sql::Connection& GetDB() = 0;

  
  
  bool InitVisitTable();

  
  
  static void FillVisitRow(sql::Statement& statement, VisitRow* visit);

  
  
  static void FillVisitVector(sql::Statement& statement, VisitVector* visits);

 private:
  DISALLOW_COPY_AND_ASSIGN(VisitDatabase);
};

}  

#endif  
