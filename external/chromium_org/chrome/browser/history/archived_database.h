// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ARCHIVED_DATABASE_H_
#define CHROME_BROWSER_HISTORY_ARCHIVED_DATABASE_H_

#include "base/basictypes.h"
#include "chrome/browser/history/url_database.h"
#include "chrome/browser/history/visit_database.h"
#include "sql/connection.h"
#include "sql/init_status.h"
#include "sql/meta_table.h"

namespace base {
class FilePath;
}

namespace history {

class ArchivedDatabase : public URLDatabase,
                         public VisitDatabase {
 public:
  
  ArchivedDatabase();
  virtual ~ArchivedDatabase();

  
  
  bool Init(const base::FilePath& file_name);

  
  
  void TrimMemory(bool aggressively);

  
  
  
  void BeginTransaction();
  void CommitTransaction();

  
  static int GetCurrentVersion();

 private:
  bool InitTables();

  
  virtual sql::Connection& GetDB() OVERRIDE;

  
  
  
  
  
  
  
  sql::InitStatus EnsureCurrentVersion();

  
  sql::Connection db_;
  sql::MetaTable meta_table_;

  DISALLOW_COPY_AND_ASSIGN(ArchivedDatabase);
};

}  

#endif  
