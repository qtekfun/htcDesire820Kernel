// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_KEYWORD_TABLE_H_
#define CHROME_BROWSER_WEBDATA_KEYWORD_TABLE_H_
#pragma once

#include <vector>

#include "chrome/browser/webdata/web_database_table.h"
#include "chrome/browser/search_engines/template_url_id.h"

class GURL;
class TemplateURL;

namespace base {
class Time;
}

class KeywordTable : public WebDatabaseTable {
 public:
  KeywordTable(sql::Connection* db, sql::MetaTable* meta_table)
      : WebDatabaseTable(db, meta_table) {}
  virtual ~KeywordTable();
  virtual bool Init();
  virtual bool IsSyncable();

  
  
  bool AddKeyword(const TemplateURL& url);

  
  
  bool RemoveKeyword(TemplateURLID id);

  
  
  
  bool GetKeywords(std::vector<TemplateURL*>* urls);

  
  
  bool UpdateKeyword(const TemplateURL& url);

  
  bool SetDefaultSearchProviderID(int64 id);
  int64 GetDefaulSearchProviderID();

  
  bool SetBuitinKeywordVersion(int version);
  int GetBuitinKeywordVersion();

  
  bool MigrateToVersion21AutoGenerateKeywordColumn();
  bool MigrateToVersion25AddLogoIDColumn();
  bool MigrateToVersion26AddCreatedByPolicyColumn();
  bool MigrateToVersion28SupportsInstantColumn();
  bool MigrateToVersion29InstantUrlToSupportsInstant();

 private:
  DISALLOW_COPY_AND_ASSIGN(KeywordTable);
};

#endif  
