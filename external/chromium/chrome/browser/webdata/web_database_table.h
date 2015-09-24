// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_WEB_DATABASE_TABLE_H_
#define CHROME_BROWSER_WEBDATA_WEB_DATABASE_TABLE_H_
#pragma once

#include "app/sql/connection.h"
#include "app/sql/init_status.h"
#include "app/sql/meta_table.h"

class WebDatabaseTable {
 protected:
  explicit WebDatabaseTable(sql::Connection* db, sql::MetaTable* meta_table)
      : db_(db), meta_table_(meta_table) {}
  virtual ~WebDatabaseTable() {
    db_ = NULL;
    meta_table_ = NULL;
  };

  
  virtual bool Init() = 0;

  
  
  
  
  
  virtual bool IsSyncable() = 0;

  sql::Connection* db_;
  sql::MetaTable* meta_table_;

 private:
  DISALLOW_COPY_AND_ASSIGN(WebDatabaseTable);
};

#endif  
