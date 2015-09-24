// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APP_SQL_META_TABLE_H_
#define APP_SQL_META_TABLE_H_
#pragma once

#include <string>

#include "base/basictypes.h"

namespace sql {

class Connection;
class Statement;

class MetaTable {
 public:
  MetaTable();
  ~MetaTable();

  
  static bool DoesTableExist(Connection* db);

  
  
  
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

 private:
  
  
  bool PrepareSetStatement(Statement* statement, const char* key);
  bool PrepareGetStatement(Statement* statement, const char* key);

  Connection* db_;

  DISALLOW_COPY_AND_ASSIGN(MetaTable);
};

}  

#endif  
