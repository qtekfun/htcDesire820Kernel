// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_SQL_HANDLER_H_
#define CHROME_BROWSER_HISTORY_ANDROID_SQL_HANDLER_H_

#include "chrome/browser/history/android/android_history_types.h"

namespace history {

struct TableIDRow {
  TableIDRow();
  ~TableIDRow();

  URLID url_id;
  GURL url;
  
  bool bookmarked;
};

typedef std::vector<TableIDRow> TableIDRows;

class SQLHandler {
 public:
  
  
  SQLHandler(const HistoryAndBookmarkRow::ColumnID columns[], int column_count);
  virtual ~SQLHandler();

  
  
  virtual bool Update(const HistoryAndBookmarkRow& row,
                      const TableIDRows& ids_set) = 0;

  
  
  
  virtual bool Insert(HistoryAndBookmarkRow* row) = 0;

  
  
  virtual bool Delete(const TableIDRows& ids_set) = 0;

  
  
  bool HasColumnIn(const HistoryAndBookmarkRow& row);

  
  bool HasColumn(HistoryAndBookmarkRow::ColumnID id);

 private:
  
  const std::set<HistoryAndBookmarkRow::ColumnID> columns_;

  DISALLOW_COPY_AND_ASSIGN(SQLHandler);
};

}  

#endif  
