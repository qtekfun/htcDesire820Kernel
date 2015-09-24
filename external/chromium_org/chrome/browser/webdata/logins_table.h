// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_LOGINS_TABLE_H_
#define CHROME_BROWSER_WEBDATA_LOGINS_TABLE_H_

#include <vector>

#include "base/compiler_specific.h"
#include "components/webdata/common/web_database_table.h"

#if defined(OS_WIN)
struct IE7PasswordInfo;
#endif

class WebDatabase;

class LoginsTable : public WebDatabaseTable {
 public:
  LoginsTable() {}
  virtual ~LoginsTable() {}

  
  static LoginsTable* FromWebDatabase(WebDatabase* db);

  virtual WebDatabaseTable::TypeKey GetTypeKey() const OVERRIDE;
  virtual bool Init(sql::Connection* db, sql::MetaTable* meta_table) OVERRIDE;
  virtual bool IsSyncable() OVERRIDE;
  virtual bool MigrateToVersion(int version,
                                bool* update_compatible_version) OVERRIDE;

#if defined(OS_WIN)
  
  bool AddIE7Login(const IE7PasswordInfo& info);

  
  bool RemoveIE7Login(const IE7PasswordInfo& info);

  
  bool GetIE7Login(const IE7PasswordInfo& info, IE7PasswordInfo* result);
#endif

 private:
  DISALLOW_COPY_AND_ASSIGN(LoginsTable);
};

#endif  
