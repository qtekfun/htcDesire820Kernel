// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATABASE_TABLE_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATABASE_TABLE_H_

#include "base/logging.h"
#include "components/webdata/common/webdata_export.h"

namespace sql {
class Connection;
class MetaTable;
}

class WEBDATA_EXPORT WebDatabaseTable {
 public:
  
  
  
  typedef void* TypeKey;

  
  WebDatabaseTable();
  virtual ~WebDatabaseTable();

  
  virtual TypeKey GetTypeKey() const = 0;

  
  
  
  
  virtual bool Init(sql::Connection* db, sql::MetaTable* meta_table);

  
  
  
  
  
  virtual bool IsSyncable() = 0;

  
  
  
  
  
  
  virtual bool MigrateToVersion(int version,
                                bool* update_compatible_version) = 0;

 protected:
  
  
  
  
  sql::Connection* db_;
  sql::MetaTable* meta_table_;

 private:
  DISALLOW_COPY_AND_ASSIGN(WebDatabaseTable);
};

#endif  
