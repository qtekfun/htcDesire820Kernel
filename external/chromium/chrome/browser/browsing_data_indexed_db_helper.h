// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_INDEXED_DB_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_INDEXED_DB_HELPER_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "chrome/common/url_constants.h"
#include "googleurl/src/gurl.h"

class Profile;

class BrowsingDataIndexedDBHelper
    : public base::RefCountedThreadSafe<BrowsingDataIndexedDBHelper> {
 public:
  
  struct IndexedDBInfo {
    IndexedDBInfo(
        const std::string& protocol,
        const std::string& host,
        unsigned short port,
        const std::string& database_identifier,
        const std::string& origin,
        const FilePath& file_path,
        int64 size,
        base::Time last_modified);
    ~IndexedDBInfo();

    bool IsFileSchemeData() {
      return protocol == chrome::kFileScheme;
    }

    std::string protocol;
    std::string host;
    unsigned short port;
    std::string database_identifier;
    std::string origin;
    FilePath file_path;
    int64 size;
    base::Time last_modified;
  };

  
  
  static BrowsingDataIndexedDBHelper* Create(Profile* profile);

  
  
  
  virtual void StartFetching(
      Callback1<const std::vector<IndexedDBInfo>& >::Type* callback) = 0;
  
  
  
  virtual void CancelNotification() = 0;
  
  virtual void DeleteIndexedDBFile(const FilePath& file_path) = 0;

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataIndexedDBHelper>;
  virtual ~BrowsingDataIndexedDBHelper() {}
};

class CannedBrowsingDataIndexedDBHelper
    : public BrowsingDataIndexedDBHelper {
 public:
  explicit CannedBrowsingDataIndexedDBHelper(Profile* profile);

  
  
  
  CannedBrowsingDataIndexedDBHelper* Clone();

  
  
  void AddIndexedDB(const GURL& origin,
                    const string16& description);

  
  void Reset();

  
  bool empty() const;

  
  virtual void StartFetching(
      Callback1<const std::vector<IndexedDBInfo>& >::Type* callback);
  virtual void CancelNotification() {}
  virtual void DeleteIndexedDBFile(const FilePath& file_path) {}

 private:
  struct PendingIndexedDBInfo {
    PendingIndexedDBInfo();
    PendingIndexedDBInfo(const GURL& origin, const string16& description);
    ~PendingIndexedDBInfo();

    GURL origin;
    string16 description;
  };

  virtual ~CannedBrowsingDataIndexedDBHelper();

  
  void ConvertPendingInfoInWebKitThread();

  void NotifyInUIThread();

  Profile* profile_;

  
  mutable base::Lock lock_;

  
  std::vector<PendingIndexedDBInfo> pending_indexed_db_info_;

  
  std::vector<IndexedDBInfo> indexed_db_info_;

  
  scoped_ptr<Callback1<const std::vector<IndexedDBInfo>& >::Type >
      completion_callback_;

  
  
  
  
  bool is_fetching_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataIndexedDBHelper);
};

#endif  
