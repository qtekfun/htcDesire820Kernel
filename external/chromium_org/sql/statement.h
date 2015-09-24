// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_STATEMENT_H_
#define SQL_STATEMENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "sql/connection.h"
#include "sql/sql_export.h"

namespace sql {

enum ColType {
  COLUMN_TYPE_INTEGER = 1,
  COLUMN_TYPE_FLOAT = 2,
  COLUMN_TYPE_TEXT = 3,
  COLUMN_TYPE_BLOB = 4,
  COLUMN_TYPE_NULL = 5,
};

class SQL_EXPORT Statement {
 public:
  
  
  Statement();

  explicit Statement(scoped_refptr<Connection::StatementRef> ref);
  ~Statement();

  
  
  void Assign(scoped_refptr<Connection::StatementRef> ref);

  
  
  void Clear();

  
  
  
  
  
  bool is_valid() const { return ref_->is_valid(); }

  

  
  
  bool Run();

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Step();

  
  
  void Reset(bool clear_bound_vars);

  
  
  
  bool Succeeded() const;

  

  
  
  
  
  bool BindNull(int col);
  bool BindBool(int col, bool val);
  bool BindInt(int col, int val);
  bool BindInt64(int col, int64 val);
  bool BindDouble(int col, double val);
  bool BindCString(int col, const char* val);
  bool BindString(int col, const std::string& val);
  bool BindString16(int col, const base::string16& value);
  bool BindBlob(int col, const void* value, int value_len);

  

  
  int ColumnCount() const;

  
  
  
  
  
  
  ColType ColumnType(int col) const;
  ColType DeclaredColumnType(int col) const;

  
  bool ColumnBool(int col) const;
  int ColumnInt(int col) const;
  int64 ColumnInt64(int col) const;
  double ColumnDouble(int col) const;
  std::string ColumnString(int col) const;
  base::string16 ColumnString16(int col) const;

  
  
  
  int ColumnByteLength(int col) const;
  const void* ColumnBlob(int col) const;
  bool ColumnBlobAsString(int col, std::string* blob);
  bool ColumnBlobAsString16(int col, base::string16* val) const;
  bool ColumnBlobAsVector(int col, std::vector<char>* val) const;
  bool ColumnBlobAsVector(int col, std::vector<unsigned char>* val) const;

  

  
  const char* GetSQLStatement();

 private:
  
  
  
  
  int CheckError(int err);

  
  
  bool CheckOk(int err) const;

  
  
  
  
  
  
  
  
  
  
  
  
  bool CheckValid() const;

  
  
  
  scoped_refptr<Connection::StatementRef> ref_;

  
  
  
  bool stepped_;

  
  bool succeeded_;

  DISALLOW_COPY_AND_ASSIGN(Statement);
};

}  

#endif  
