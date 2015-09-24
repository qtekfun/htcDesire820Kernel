// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_DATABASE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_DATABASE_HELPER_H_

#include <list>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/common/url_constants.h"
#include "url/gurl.h"
#include "webkit/browser/database/database_tracker.h"
#include "webkit/common/database/database_identifier.h"

class Profile;

class BrowsingDataDatabaseHelper
    : public base::RefCountedThreadSafe<BrowsingDataDatabaseHelper> {
 public:
  
  struct DatabaseInfo {
    DatabaseInfo(const webkit_database::DatabaseIdentifier& identifier,
                 const std::string& database_name,
                 const std::string& description,
                 int64 size,
                 base::Time last_modified);
    ~DatabaseInfo();

    webkit_database::DatabaseIdentifier identifier;
    std::string database_name;
    std::string description;
    int64 size;
    base::Time last_modified;
  };

  explicit BrowsingDataDatabaseHelper(Profile* profile);

  
  
  
  virtual void StartFetching(
      const base::Callback<void(const std::list<DatabaseInfo>&)>& callback);

  
  
  virtual void DeleteDatabase(const std::string& origin_identifier,
                              const std::string& name);

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataDatabaseHelper>;
  virtual ~BrowsingDataDatabaseHelper();

  
  void NotifyInUIThread();

  
  
  
  
  
  
  std::list<DatabaseInfo> database_info_;

  
  base::Callback<void(const std::list<DatabaseInfo>&)> completion_callback_;

  
  
  
  
  bool is_fetching_;

 private:
  
  void FetchDatabaseInfoOnFileThread();

  
  void DeleteDatabaseOnFileThread(const std::string& origin,
                                  const std::string& name);

  scoped_refptr<webkit_database::DatabaseTracker> tracker_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataDatabaseHelper);
};

class CannedBrowsingDataDatabaseHelper : public BrowsingDataDatabaseHelper {
 public:
  struct PendingDatabaseInfo {
    PendingDatabaseInfo(const GURL& origin,
                        const std::string& name,
                        const std::string& description);
    ~PendingDatabaseInfo();

    
    bool operator<(const PendingDatabaseInfo& other) const;

    GURL origin;
    std::string name;
    std::string description;
  };

  explicit CannedBrowsingDataDatabaseHelper(Profile* profile);

  
  
  
  CannedBrowsingDataDatabaseHelper* Clone();

  
  
  void AddDatabase(const GURL& origin,
                   const std::string& name,
                   const std::string& description);

  
  void Reset();

  
  bool empty() const;

  
  size_t GetDatabaseCount() const;

  
  const std::set<PendingDatabaseInfo>& GetPendingDatabaseInfo();

  
  virtual void StartFetching(
      const base::Callback<void(const std::list<DatabaseInfo>&)>& callback)
          OVERRIDE;

 private:
  virtual ~CannedBrowsingDataDatabaseHelper();

  std::set<PendingDatabaseInfo> pending_database_info_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataDatabaseHelper);
};

#endif  
