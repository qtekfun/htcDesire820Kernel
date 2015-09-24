// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APP_SQL_CONNECTION_H_
#define APP_SQL_CONNECTION_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"

class FilePath;
struct sqlite3;
struct sqlite3_stmt;

namespace sql {

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

class ErrorDelegate : public base::RefCounted<ErrorDelegate> {
 public:
  ErrorDelegate();

  
  
  
  
  
  
  
  
  
  
  
  virtual int OnError(int error, Connection* connection, Statement* stmt) = 0;

 protected:
  friend class base::RefCounted<ErrorDelegate>;

  virtual ~ErrorDelegate();
};

class Connection {
 private:
  class StatementRef;  

 public:
  
  
  Connection();
  ~Connection();

  

  
  
  
  
  
  
  
  void set_page_size(int page_size) { page_size_ = page_size; }

  
  
  
  void set_cache_size(int cache_size) { cache_size_ = cache_size; }

  
  
  
  
  
  
  
  
  
  
  void set_exclusive_locking() { exclusive_locking_ = true; }

  
  
  
  void set_error_delegate(ErrorDelegate* delegate) {
    error_delegate_ = delegate;
  }

  

  
  
  bool Open(const FilePath& path);

  
  
  
  bool OpenInMemory();

  
  bool is_open() const { return !!db_; }

  
  
  
  
  void Close();

  
  
  
  
  
  
  
  
  
  
  
  
  
  void Preload();

  

  
  
  
  
  
  
  
  
  
  bool BeginTransaction();
  void RollbackTransaction();
  bool CommitTransaction();

  
  
  int transaction_nesting() const { return transaction_nesting_; }

  

  
  
  
  bool Execute(const char* sql);

  
  
  
  
  bool HasCachedStatement(const StatementID& id) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_refptr<StatementRef> GetCachedStatement(const StatementID& id,
                                                 const char* sql);

  
  
  
  
  
  scoped_refptr<StatementRef> GetUniqueStatement(const char* sql);

  

  
  bool DoesTableExist(const char* table_name) const;

  
  bool DoesColumnExist(const char* table_name, const char* column_name) const;

  
  
  int64 GetLastInsertRowId() const;

  
  
  
  int GetLastChangeCount() const;

  

  
  int GetErrorCode() const;

  
  
  int GetLastErrno() const;

  
  
  const char* GetErrorMessage() const;

 private:
  
  
  friend class Statement;

  
  
  
  bool OpenInternal(const std::string& file_name);

  
  
  
  
  
  
  
  
  
  
  
  class StatementRef : public base::RefCounted<StatementRef> {
   public:
    
    StatementRef();
    StatementRef(Connection* connection, sqlite3_stmt* stmt);

    
    bool is_valid() const { return !!stmt_; }

    
    
    Connection* connection() const { return connection_; }

    
    
    sqlite3_stmt* stmt() const { return stmt_; }

    
    
    void Close();

   private:
    friend class base::RefCounted<StatementRef>;

    ~StatementRef();

    Connection* connection_;
    sqlite3_stmt* stmt_;

    DISALLOW_COPY_AND_ASSIGN(StatementRef);
  };
  friend class StatementRef;

  
  
  void DoRollback();

  
  
  void StatementRefCreated(StatementRef* ref);
  void StatementRefDeleted(StatementRef* ref);

  
  void ClearCache();

  
  
  int OnSqliteError(int err, Statement* stmt);

  
  bool ExecuteWithTimeout(const char* sql, base::TimeDelta ms_timeout);

  
  
  sqlite3* db_;

  
  
  int page_size_;
  int cache_size_;
  bool exclusive_locking_;

  
  
  typedef std::map<StatementID, scoped_refptr<StatementRef> >
      CachedStatementMap;
  CachedStatementMap statement_cache_;

  
  
  
  typedef std::set<StatementRef*> StatementRefSet;
  StatementRefSet open_statements_;

  
  int transaction_nesting_;

  
  
  
  bool needs_rollback_;

  
  
  scoped_refptr<ErrorDelegate> error_delegate_;

  DISALLOW_COPY_AND_ASSIGN(Connection);
};

}  

#endif  
