// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_DATABASE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_DATABASE_HELPER_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/common/url_constants.h"
#include "googleurl/src/gurl.h"
#include "webkit/database/database_tracker.h"

class Profile;

class BrowsingDataDatabaseHelper
    : public base::RefCountedThreadSafe<BrowsingDataDatabaseHelper> {
 public:
  
  struct DatabaseInfo {
    DatabaseInfo();
    DatabaseInfo(const std::string& host,
                 const std::string& database_name,
                 const std::string& origin_identifier,
                 const std::string& description,
                 const std::string& origin,
                 int64 size,
                 base::Time last_modified);
    ~DatabaseInfo();

    bool IsFileSchemeData();

    std::string host;
    std::string database_name;
    std::string origin_identifier;
    std::string description;
    std::string origin;
    int64 size;
    base::Time last_modified;
  };

  explicit BrowsingDataDatabaseHelper(Profile* profile);

  
  
  
  virtual void StartFetching(
      Callback1<const std::vector<DatabaseInfo>& >::Type* callback);

  
  
  
  virtual void CancelNotification();

  
  
  virtual void DeleteDatabase(const std::string& origin,
                              const std::string& name);

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataDatabaseHelper>;
  virtual ~BrowsingDataDatabaseHelper();

  
  void NotifyInUIThread();

  
  std::vector<DatabaseInfo> database_info_;

  
  scoped_ptr<Callback1<const std::vector<DatabaseInfo>& >::Type >
      completion_callback_;

  
  
  
  
  bool is_fetching_;

 private:
  
  void FetchDatabaseInfoInWebKitThread();

  
  void DeleteDatabaseInWebKitThread(const std::string& origin,
                                  const std::string& name);

  scoped_refptr<webkit_database::DatabaseTracker> tracker_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataDatabaseHelper);
};

class CannedBrowsingDataDatabaseHelper : public BrowsingDataDatabaseHelper {
 public:
  explicit CannedBrowsingDataDatabaseHelper(Profile* profile);

  
  
  
  CannedBrowsingDataDatabaseHelper* Clone();

  
  
  void AddDatabase(const GURL& origin,
                   const std::string& name,
                   const std::string& description);

  
  void Reset();

  
  bool empty() const;

  
  virtual void StartFetching(
      Callback1<const std::vector<DatabaseInfo>& >::Type* callback);
  virtual void CancelNotification() {}

 private:
  struct PendingDatabaseInfo {
    PendingDatabaseInfo();
    PendingDatabaseInfo(const GURL& origin,
                        const std::string& name,
                        const std::string& description);
    ~PendingDatabaseInfo();

    GURL origin;
    std::string name;
    std::string description;
  };

  virtual ~CannedBrowsingDataDatabaseHelper();

  
  void ConvertInfoInWebKitThread();

  
  mutable base::Lock lock_;

  
  std::vector<PendingDatabaseInfo> pending_database_info_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataDatabaseHelper);
};

#endif  
