// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_CONNECTION_H_
#define SQL_CONNECTION_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread_restrictions.h"
#include "base/time/time.h"
#include "sql/sql_export.h"

struct sqlite3;
struct sqlite3_stmt;

namespace base {
class FilePath;
}

namespace sql {

class Recovery;
class Statement;

class StatementID {
 public:
  
  
  StatementID(const char* file, int line)
      : number_(line),
        str_(file) {
  }

  
  explicit StatementID(const char* unique_name)
      : number_(-1),
        str_(unique_name) {
  }

  
  
  
  
  explicit StatementID(const std::string& dont_ever_do_this);

  
  bool operator<(const StatementID& other) const;

 private:
  int number_;
  const char* str_;
};

#define SQL_FROM_HERE sql::StatementID(__FILE__, __LINE__)

class Connection;

class SQL_EXPORT Connection {
 private:
  class StatementRef;  

 public:
  
  
  Connection();
  ~Connection();

  

  
  
  
  
  
  
  
  void set_page_size(int page_size) { page_size_ = page_size; }

  
  
  
  void set_cache_size(int cache_size) { cache_size_ = cache_size; }

  
  
  
  
  
  
  
  
  
  
  void set_exclusive_locking() { exclusive_locking_ = true; }

  
  
  
  
  void set_restrict_to_user() { restrict_to_user_ = true; }

  
  
  
  
  
  typedef base::Callback<void(int, Statement*)> ErrorCallback;
  void set_error_callback(const ErrorCallback& callback) {
    error_callback_ = callback;
  }
  bool has_error_callback() const {
    return !error_callback_.is_null();
  }
  void reset_error_callback() {
    error_callback_.Reset();
  }

  
  
  void set_histogram_tag(const std::string& tag) {
    histogram_tag_ = tag;
  }

  
  
  
  void AddTaggedHistogram(const std::string& name, size_t sample) const;

  
  
  
  
  bool FullIntegrityCheck(std::vector<std::string>* messages);

  
  
  
  bool QuickIntegrityCheck() WARN_UNUSED_RESULT;

  

  
  
  bool Open(const base::FilePath& path) WARN_UNUSED_RESULT;

  
  
  
  bool OpenInMemory() WARN_UNUSED_RESULT;

  
  
  
  
  bool OpenTemporary() WARN_UNUSED_RESULT;

  
  bool is_open() const { return !!db_; }

  
  
  
  
  void Close();

  
  
  
  
  
  
  
  
  
  
  
  
  
  void Preload();

  
  
  
  
  void TrimMemory(bool aggressively);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Raze();
  bool RazeWithTimout(base::TimeDelta timeout);

  
  
  
  
  
  
  
  void Poison();

  
  
  
  bool RazeAndClose();

  
  
  
  
  
  
  
  
  
  
  static bool Delete(const base::FilePath& path);

  

  
  
  
  
  
  
  
  
  
  bool BeginTransaction();
  void RollbackTransaction();
  bool CommitTransaction();

  
  
  void RollbackAllTransactions();

  
  
  int transaction_nesting() const { return transaction_nesting_; }

  

  
  
  
  
  
  
  
  bool AttachDatabase(const base::FilePath& other_db_path,
                      const char* attachment_point);
  bool DetachDatabase(const char* attachment_point);

  

  
  
  
  
  
  
  
  
  bool Execute(const char* sql) WARN_UNUSED_RESULT;

  
  int ExecuteAndReturnErrorCode(const char* sql) WARN_UNUSED_RESULT;

  
  
  
  
  bool HasCachedStatement(const StatementID& id) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_refptr<StatementRef> GetCachedStatement(const StatementID& id,
                                                 const char* sql);

  
  
  bool IsSQLValid(const char* sql);

  
  
  
  
  
  scoped_refptr<StatementRef> GetUniqueStatement(const char* sql);

  

  
  bool DoesTableExist(const char* table_name) const;

  
  bool DoesIndexExist(const char* index_name) const;

  
  bool DoesColumnExist(const char* table_name, const char* column_name) const;

  
  
  int64 GetLastInsertRowId() const;

  
  
  
  int GetLastChangeCount() const;

  

  
  int GetErrorCode() const;

  
  
  int GetLastErrno() const;

  
  
  const char* GetErrorMessage() const;

  
  
  
  std::string GetSchema() const;

  
  
  
  
  static bool ShouldIgnoreSqliteError(int error);

 private:
  
  friend class Recovery;

  
  friend class ScopedErrorIgnorer;

  
  
  friend class Statement;

  
  
  
  
  
  
  enum Retry {
    NO_RETRY = 0,
    RETRY_ON_POISON
  };
  bool OpenInternal(const std::string& file_name, Retry retry_flag);

  
  
  void CloseInternal(bool forced);

  
  
  
  void AssertIOAllowed() {
    if (!in_memory_)
      base::ThreadRestrictions::AssertIOAllowed();
  }

  
  bool DoesTableOrIndexExist(const char* name, const char* type) const;

  
  
  typedef base::Callback<bool(int)> ErrorIgnorerCallback;
  static ErrorIgnorerCallback* current_ignorer_cb_;
  static void SetErrorIgnorer(ErrorIgnorerCallback* ignorer);
  static void ResetErrorIgnorer();

  
  
  
  
  
  
  
  
  
  
  
  class SQL_EXPORT StatementRef : public base::RefCounted<StatementRef> {
   public:
    
    
    
    
    
    
    
    
    StatementRef(Connection* connection, sqlite3_stmt* stmt, bool was_valid);

    
    bool is_valid() const { return !!stmt_; }

    
    
    
    bool was_valid() const { return was_valid_; }

    
    
    
    Connection* connection() const { return connection_; }

    
    
    sqlite3_stmt* stmt() const { return stmt_; }

    
    
    
    void Close(bool forced);

    
    
    void AssertIOAllowed() { if (connection_) connection_->AssertIOAllowed(); }

   private:
    friend class base::RefCounted<StatementRef>;

    ~StatementRef();

    Connection* connection_;
    sqlite3_stmt* stmt_;
    bool was_valid_;

    DISALLOW_COPY_AND_ASSIGN(StatementRef);
  };
  friend class StatementRef;

  
  
  void DoRollback();

  
  
  void StatementRefCreated(StatementRef* ref);
  void StatementRefDeleted(StatementRef* ref);

  
  
  
  
  
  
  
  
  
  
  int OnSqliteError(int err, Statement* stmt, const char* sql);

  
  bool ExecuteWithTimeout(const char* sql, base::TimeDelta ms_timeout)
      WARN_UNUSED_RESULT;

  
  
  
  
  
  
  scoped_refptr<StatementRef> GetUntrackedStatement(const char* sql) const;

  bool IntegrityCheckHelper(
      const char* pragma_sql,
      std::vector<std::string>* messages) WARN_UNUSED_RESULT;

  
  
  sqlite3* db_;

  
  
  int page_size_;
  int cache_size_;
  bool exclusive_locking_;
  bool restrict_to_user_;

  
  
  typedef std::map<StatementID, scoped_refptr<StatementRef> >
      CachedStatementMap;
  CachedStatementMap statement_cache_;

  
  
  
  typedef std::set<StatementRef*> StatementRefSet;
  StatementRefSet open_statements_;

  
  int transaction_nesting_;

  
  
  
  bool needs_rollback_;

  
  
  bool in_memory_;

  
  
  
  
  bool poisoned_;

  ErrorCallback error_callback_;

  
  std::string histogram_tag_;

  DISALLOW_COPY_AND_ASSIGN(Connection);
};

}  

#endif  
