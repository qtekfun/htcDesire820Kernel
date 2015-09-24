// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATABASE_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATABASE_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "components/webdata/common/web_database_table.h"
#include "components/webdata/common/webdata_export.h"
#include "sql/connection.h"
#include "sql/init_status.h"
#include "sql/meta_table.h"

namespace base {
class FilePath;
}

class WEBDATA_EXPORT WebDatabase {
 public:
  enum State {
    COMMIT_NOT_NEEDED,
    COMMIT_NEEDED
  };
  
  static const int kCurrentVersionNumber;

  WebDatabase();
  virtual ~WebDatabase();

  
  
  
  void AddTable(WebDatabaseTable* table);

  
  WebDatabaseTable* GetTable(WebDatabaseTable::TypeKey key);

  
  
  
  
  
  
  sql::InitStatus Init(const base::FilePath& db_name);

  
  void BeginTransaction();
  void CommitTransaction();

  
  sql::Connection* GetSQLConnection();

 private:
  
  
  sql::InitStatus MigrateOldVersionsAsNeeded();

  sql::Connection db_;
  sql::MetaTable meta_table_;

  
  
  typedef std::map<WebDatabaseTable::TypeKey, WebDatabaseTable*> TableMap;
  TableMap tables_;

  DISALLOW_COPY_AND_ASSIGN(WebDatabase);
};

#endif  
