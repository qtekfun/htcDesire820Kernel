// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_DATABASE_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_DATABASE_MESSAGE_FILTER_H_

#include "base/containers/hash_tables.h"
#include "base/strings/string16.h"
#include "content/public/browser/browser_message_filter.h"
#include "webkit/browser/database/database_tracker.h"
#include "webkit/common/database/database_connections.h"
#include "webkit/common/quota/quota_types.h"

namespace content {

class DatabaseMessageFilter
    : public BrowserMessageFilter,
      public webkit_database::DatabaseTracker::Observer {
 public:
  explicit DatabaseMessageFilter(webkit_database::DatabaseTracker* db_tracker);

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual void OverrideThreadForMessage(
      const IPC::Message& message,
      BrowserThread::ID* thread) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  webkit_database::DatabaseTracker* database_tracker() const {
    return db_tracker_.get();
  }

 private:
  virtual ~DatabaseMessageFilter();

  class PromptDelegate;

  void AddObserver();
  void RemoveObserver();

  
  void OnDatabaseOpenFile(const base::string16& vfs_file_name,
                          int desired_flags,
                          IPC::Message* reply_msg);
  void OnDatabaseDeleteFile(const base::string16& vfs_file_name,
                            const bool& sync_dir,
                            IPC::Message* reply_msg);
  void OnDatabaseGetFileAttributes(const base::string16& vfs_file_name,
                                   IPC::Message* reply_msg);
  void OnDatabaseGetFileSize(const base::string16& vfs_file_name,
                             IPC::Message* reply_msg);

  
  void OnDatabaseGetSpaceAvailable(const std::string& origin_identifier,
                                   IPC::Message* reply_msg);
  void OnDatabaseGetUsageAndQuota(IPC::Message* reply_msg,
                                  quota::QuotaStatusCode status,
                                  int64 usage,
                                  int64 quota);

  
  void OnDatabaseOpened(const std::string& origin_identifier,
                        const base::string16& database_name,
                        const base::string16& description,
                        int64 estimated_size);
  void OnDatabaseModified(const std::string& origin_identifier,
                          const base::string16& database_name);
  void OnDatabaseClosed(const std::string& origin_identifier,
                        const base::string16& database_name);
  void OnHandleSqliteError(const std::string& origin_identifier,
                           const base::string16& database_name,
                           int error);

  
  virtual void OnDatabaseSizeChanged(const std::string& origin_identifier,
                                     const base::string16& database_name,
                                     int64 database_size) OVERRIDE;
  virtual void OnDatabaseScheduledForDeletion(
      const std::string& origin_identifier,
      const base::string16& database_name) OVERRIDE;

  void DatabaseDeleteFile(const base::string16& vfs_file_name,
                          bool sync_dir,
                          IPC::Message* reply_msg,
                          int reschedule_count);

  
  scoped_refptr<webkit_database::DatabaseTracker> db_tracker_;

  
  
  bool observer_added_;

  
  webkit_database::DatabaseConnections database_connections_;
};

}  

#endif  
