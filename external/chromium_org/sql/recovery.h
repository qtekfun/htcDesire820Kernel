// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_RECOVERY_H_
#define SQL_RECOVERY_H_

#include "base/basictypes.h"

#include "sql/connection.h"

namespace base {
class FilePath;
}

namespace sql {


class SQL_EXPORT Recovery {
 public:
  ~Recovery();

  
  
  
  
  
  
  
  static bool FullRecoverySupported();

  
  
  
  
  
  
  
  
  
  
  
  static scoped_ptr<Recovery> Begin(
      Connection* connection,
      const base::FilePath& db_path) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  
  
  
  static bool Recovered(scoped_ptr<Recovery> r) WARN_UNUSED_RESULT;

  
  
  static void Unrecoverable(scoped_ptr<Recovery> r);

  
  
  
  
  
  
  static void Rollback(scoped_ptr<Recovery> r);

  
  sql::Connection* db() { return &recover_db_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool AutoRecoverTable(const char* table_name,
                        size_t extend_columns,
                        size_t* rows_recovered);

  
  
  
  
  
  bool SetupMeta();

  
  
  
  
  
  bool GetMetaVersionNumber(int* version_number);

 private:
  explicit Recovery(Connection* connection);

  
  
  bool Init(const base::FilePath& db_path) WARN_UNUSED_RESULT;

  
  bool Backup() WARN_UNUSED_RESULT;

  
  
  
  enum Disposition {
    RAZE_AND_POISON,
    POISON,
  };
  void Shutdown(Disposition raze);

  Connection* db_;         
  Connection recover_db_;  

  DISALLOW_COPY_AND_ASSIGN(Recovery);
};

}  

#endif  
