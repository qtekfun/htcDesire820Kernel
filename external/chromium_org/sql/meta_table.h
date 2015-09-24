// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_META_TABLE_H_
#define SQL_META_TABLE_H_

#include <string>

#include "base/basictypes.h"
#include "sql/sql_export.h"

namespace sql {

class Connection;
class Statement;

class SQL_EXPORT MetaTable {
 public:
  MetaTable();
  ~MetaTable();

  
  static bool DoesTableExist(Connection* db);

  
  
  
  
  
  
  
  
  
  
  
  
  static void RazeIfDeprecated(Connection* db, int deprecated_version);

  
  
  
  
  bool Init(Connection* db, int version, int compatible_version);

  
  void Reset();

  
  
  
  
  
  void SetVersionNumber(int version);
  int GetVersionNumber();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetCompatibleVersionNumber(int version);
  int GetCompatibleVersionNumber();

  
  bool SetValue(const char* key, const std::string& value);
  bool SetValue(const char* key, int value);
  bool SetValue(const char* key, int64 value);

  
  
  bool GetValue(const char* key, std::string* value);
  bool GetValue(const char* key, int* value);
  bool GetValue(const char* key, int64* value);

  
  bool DeleteKey(const char* key);

 private:
  
  
  void PrepareSetStatement(Statement* statement, const char* key);
  bool PrepareGetStatement(Statement* statement, const char* key);

  Connection* db_;

  DISALLOW_COPY_AND_ASSIGN(MetaTable);
};

}  

#endif  
