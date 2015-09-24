// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_TEST_TEST_HELPERS_H_
#define SQL_TEST_TEST_HELPERS_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"


namespace base {
class FilePath;
}

namespace sql {
class Connection;
}

namespace sql {
namespace test {

bool CorruptSizeInHeader(const base::FilePath& db_path) WARN_UNUSED_RESULT;

bool CorruptTableOrIndex(const base::FilePath& db_path,
                         const char* tree_name,
                         const char* update_sql) WARN_UNUSED_RESULT;

size_t CountSQLTables(sql::Connection* db) WARN_UNUSED_RESULT;

size_t CountSQLIndices(sql::Connection* db) WARN_UNUSED_RESULT;

size_t CountTableColumns(sql::Connection* db, const char* table)
    WARN_UNUSED_RESULT;

bool CountTableRows(sql::Connection* db, const char* table, size_t* count);

bool CreateDatabaseFromSQL(const base::FilePath& db_path,
                           const base::FilePath& sql_path) WARN_UNUSED_RESULT;

std::string IntegrityCheck(sql::Connection* db) WARN_UNUSED_RESULT;

}  
}  

#endif  
