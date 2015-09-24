// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_WEB_INTENTS_TABLE_H_
#define CHROME_BROWSER_WEBDATA_WEB_INTENTS_TABLE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "components/webdata/common/web_database_table.h"

namespace sql {
class Connection;
class MetaTable;
}

struct DefaultWebIntentService;
class WebDatabase;


class WebIntentsTable : public WebDatabaseTable {
 public:
  WebIntentsTable();
  virtual ~WebIntentsTable();

  
  static WebIntentsTable* FromWebDatabase(WebDatabase* database);

  
  virtual WebDatabaseTable::TypeKey GetTypeKey() const OVERRIDE;
  virtual bool Init(sql::Connection* db, sql::MetaTable* meta_table) OVERRIDE;
  virtual bool IsSyncable() OVERRIDE;
  virtual bool MigrateToVersion(int version,
                                bool* update_compatible_version) OVERRIDE;

  
  bool MigrateToVersion46AddSchemeColumn();

 private:
  DISALLOW_COPY_AND_ASSIGN(WebIntentsTable);
};

#endif  
