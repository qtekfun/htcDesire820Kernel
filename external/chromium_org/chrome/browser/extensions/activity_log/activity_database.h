// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_DATABASE_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_DATABASE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted_memory.h"
#include "base/synchronization/lock.h"
#include "base/timer/timer.h"
#include "chrome/browser/extensions/activity_log/activity_actions.h"
#include "content/public/browser/browser_thread.h"
#include "extensions/common/extension.h"
#include "sql/connection.h"
#include "sql/init_status.h"

namespace base {
class Clock;
class FilePath;
}

namespace extensions {

class ActivityDatabase {
 public:
  
  
  
  
  class Delegate {
   protected:
    friend class ActivityDatabase;

    
    
    virtual ~Delegate() {}

    
    
    
    virtual bool InitDatabase(sql::Connection* db) = 0;

    
    
    
    
    virtual bool FlushDatabase(sql::Connection* db) = 0;

    
    
    
    virtual void OnDatabaseFailure() = 0;

    
    
    
    
    virtual void OnDatabaseClose() = 0;
  };

  
  static const int kFlushImmediately = -1;

  
  
  
  explicit ActivityDatabase(Delegate* delegate);

  
  
  void Init(const base::FilePath& db_name);

  
  void Close();

  
  // written out.  The size parameter should indicate (approximately) how many
  // records are queued to be written; the database may use this information to
  
  
  
  
  void AdviseFlush(int size);

  
  
  
  void SetBatchModeForTesting(bool batch_mode);

  bool is_db_valid() const { return valid_db_; }

  
  
  
  
  
  static bool InitializeTable(sql::Connection* db,
                              const char* table_name,
                              const char* content_fields[],
                              const char* field_types[],
                              const int num_content_fields);

  
  
  
  void RunOnDatabase(const base::Callback<void(sql::Connection*)>& callback);

 private:
  
  
  virtual ~ActivityDatabase();

  
  
  
  void LogInitFailure();

  
  
  
  void RecordBatchedActions();

  
  
  
  
  void HardFailureClose();

  
  
  void SoftFailureClose();

  
  
  
  void DatabaseErrorCallback(int error, sql::Statement* stmt);

  
  void RecordBatchedActionsWhileTesting();
  void SetTimerForTesting(int milliseconds);

  
  
  
  sql::Connection* GetSqlConnection();

  
  
  Delegate* delegate_;

  sql::Connection db_;
  bool valid_db_;
  bool batch_mode_;
  base::TimeDelta batching_period_;
  base::RepeatingTimer<ActivityDatabase> timer_;
  bool already_closed_;
  bool did_init_;

  friend class ActivityLogDatabasePolicy;
  FRIEND_TEST_ALL_PREFIXES(ActivityDatabaseTest, BatchModeOff);
  FRIEND_TEST_ALL_PREFIXES(ActivityDatabaseTest, BatchModeOn);
  FRIEND_TEST_ALL_PREFIXES(ActivityDatabaseTest, BatchModeFlush);
  DISALLOW_COPY_AND_ASSIGN(ActivityDatabase);
};

}  
#endif  
