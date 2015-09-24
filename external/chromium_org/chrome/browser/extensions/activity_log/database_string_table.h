// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_DATABASE_STRING_TABLE_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_DATABASE_STRING_TABLE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

namespace sql {
class Connection;
}  

namespace extensions {

class DatabaseStringTable {
 public:
  explicit DatabaseStringTable(const std::string& table);

  ~DatabaseStringTable();

  
  
  bool Initialize(sql::Connection* connection);

  
  
  
  
  bool StringToInt(sql::Connection* connection,
                   const std::string& value,
                   int64* id);

  
  
  
  
  bool IntToString(sql::Connection* connection, int64 id, std::string* value);

  
  
  void ClearCache();

 private:
  
  void PruneCache();

  
  std::map<int64, std::string> id_to_value_;
  std::map<std::string, int64> value_to_id_;

  
  std::string table_;

  FRIEND_TEST_ALL_PREFIXES(DatabaseStringTableTest, Prune);

  DISALLOW_COPY_AND_ASSIGN(DatabaseStringTable);
};

}  

#endif  
