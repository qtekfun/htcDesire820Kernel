// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_DATABASE_DATABASE_CONNECTIONS_H_
#define WEBKIT_COMMON_DATABASE_DATABASE_CONNECTIONS_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "webkit/common/webkit_storage_common_export.h"

namespace base {
class MessageLoopProxy;
}

namespace webkit_database {

class WEBKIT_STORAGE_COMMON_EXPORT DatabaseConnections {
 public:
  DatabaseConnections();
  ~DatabaseConnections();

  bool IsEmpty() const;
  bool IsDatabaseOpened(const std::string& origin_identifier,
                        const base::string16& database_name) const;
  bool IsOriginUsed(const std::string& origin_identifier) const;

  
  bool AddConnection(const std::string& origin_identifier,
                     const base::string16& database_name);

  
  bool RemoveConnection(const std::string& origin_identifier,
                        const base::string16& database_name);

  void RemoveAllConnections();
  void RemoveConnections(
      const DatabaseConnections& connections,
      std::vector<std::pair<std::string, base::string16> >* closed_dbs);

  
  int64 GetOpenDatabaseSize(const std::string& origin_identifier,
                            const base::string16& database_name) const;
  void SetOpenDatabaseSize(const std::string& origin_identifier,
                           const base::string16& database_name,
                           int64 size);

  
  void ListConnections(
      std::vector<std::pair<std::string, base::string16> > *list) const;

 private:
  
  typedef std::map<base::string16, std::pair<int, int64> > DBConnections;
  typedef std::map<std::string, DBConnections> OriginConnections;
  mutable OriginConnections connections_;  

  
  bool RemoveConnectionsHelper(const std::string& origin_identifier,
                               const base::string16& database_name,
                               int num_connections);
};

class WEBKIT_STORAGE_COMMON_EXPORT DatabaseConnectionsWrapper
    : public base::RefCountedThreadSafe<DatabaseConnectionsWrapper> {
 public:
  DatabaseConnectionsWrapper();

  
  
  void WaitForAllDatabasesToClose();
  bool HasOpenConnections();

  
  void AddOpenConnection(const std::string& origin_identifier,
                         const base::string16& database_name);
  void RemoveOpenConnection(const std::string& origin_identifier,
                            const base::string16& database_name);
 private:
  ~DatabaseConnectionsWrapper();
  friend class base::RefCountedThreadSafe<DatabaseConnectionsWrapper>;

  bool waiting_for_dbs_to_close_;
  base::Lock open_connections_lock_;
  DatabaseConnections open_connections_;
  scoped_refptr<base::MessageLoopProxy> main_thread_;
};

}  

#endif  
