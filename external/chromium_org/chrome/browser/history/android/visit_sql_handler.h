// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_VISIT_SQL_HANDLER_H_
#define CHROME_BROWSER_HISTORY_ANDROID_VISIT_SQL_HANDLER_H_

#include "chrome/browser/history/android/sql_handler.h"

namespace base {
class Time;
}

namespace history {

class HistoryDatabase;

class VisitSQLHandler : public SQLHandler {
 public:
  explicit VisitSQLHandler(HistoryDatabase* history_db);
  virtual ~VisitSQLHandler();

  
  virtual bool Update(const HistoryAndBookmarkRow& row,
                      const TableIDRows& ids_set) OVERRIDE;
  virtual bool Insert(HistoryAndBookmarkRow* row) OVERRIDE;
  virtual bool Delete(const TableIDRows& ids_set) OVERRIDE;

 private:
  
  bool AddVisit(URLID url_id, const base::Time& visit_time);

  
  
  bool AddVisitRows(URLID url_id,
                    int visit_count,
                    const base::Time& last_visit_time);

  
  bool DeleteVisitsForURL(URLID url_id);

  HistoryDatabase* history_db_;

  DISALLOW_COPY_AND_ASSIGN(VisitSQLHandler);
};

}  

#endif  
