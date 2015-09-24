// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_TOKEN_SERVICE_TABLE_H_
#define CHROME_BROWSER_WEBDATA_TOKEN_SERVICE_TABLE_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "components/webdata/common/web_database_table.h"

class WebDatabase;

class TokenServiceTable : public WebDatabaseTable {
 public:
  TokenServiceTable() {}
  virtual ~TokenServiceTable() {}

  
  static TokenServiceTable* FromWebDatabase(WebDatabase* db);

  virtual WebDatabaseTable::TypeKey GetTypeKey() const OVERRIDE;
  virtual bool Init(sql::Connection* db, sql::MetaTable* meta_table) OVERRIDE;
  virtual bool IsSyncable() OVERRIDE;
  virtual bool MigrateToVersion(int version,
                                bool* update_compatible_version) OVERRIDE;

  
  bool RemoveAllTokens();

  
  bool RemoveTokenForService(const std::string& service);

  
  
  
  bool GetAllTokens(std::map<std::string, std::string>* tokens);

  
  
  
  bool SetTokenForService(const std::string& service,
                          const std::string& token);

 private:
  DISALLOW_COPY_AND_ASSIGN(TokenServiceTable);
};

#endif  
