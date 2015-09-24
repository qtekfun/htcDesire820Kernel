// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_TOKEN_SERVICE_TABLE_H_
#define CHROME_BROWSER_WEBDATA_TOKEN_SERVICE_TABLE_H_
#pragma once

#include "chrome/browser/webdata/web_database_table.h"

class TokenServiceTable : public WebDatabaseTable {
 public:
  TokenServiceTable(sql::Connection* db, sql::MetaTable* meta_table)
      : WebDatabaseTable(db, meta_table) {}
  virtual ~TokenServiceTable() {}
  virtual bool Init();
  virtual bool IsSyncable();

  
  bool RemoveAllTokens();

  
  
  
  bool GetAllTokens(std::map<std::string, std::string>* tokens);

  
  
  
  bool SetTokenForService(const std::string& service,
                          const std::string& token);

 private:
  DISALLOW_COPY_AND_ASSIGN(TokenServiceTable);
};


#endif  
