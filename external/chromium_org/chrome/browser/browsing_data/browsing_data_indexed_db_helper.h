// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_INDEXED_DB_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_INDEXED_DB_HELPER_H_

#include <list>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "content/public/browser/indexed_db_context.h"
#include "url/gurl.h"

class Profile;

class BrowsingDataIndexedDBHelper
    : public base::RefCountedThreadSafe<BrowsingDataIndexedDBHelper> {
 public:
  
  
  static BrowsingDataIndexedDBHelper* Create(
      content::IndexedDBContext* context);

  
  
  
  virtual void StartFetching(
      const base::Callback<void(const std::list<content::IndexedDBInfo>&)>&
          callback) = 0;
  
  virtual void DeleteIndexedDB(const GURL& origin) = 0;

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataIndexedDBHelper>;
  virtual ~BrowsingDataIndexedDBHelper() {}
};

class CannedBrowsingDataIndexedDBHelper
    : public BrowsingDataIndexedDBHelper {
 public:
  
  struct PendingIndexedDBInfo {
    PendingIndexedDBInfo(const GURL& origin, const base::string16& name);
    ~PendingIndexedDBInfo();

    bool operator<(const PendingIndexedDBInfo& other) const;

    GURL origin;
    base::string16 name;
  };

  CannedBrowsingDataIndexedDBHelper();

  
  
  
  CannedBrowsingDataIndexedDBHelper* Clone();

  
  
  void AddIndexedDB(const GURL& origin,
                    const base::string16& name);

  
  void Reset();

  
  bool empty() const;

  
  size_t GetIndexedDBCount() const;

  
  const std::set<CannedBrowsingDataIndexedDBHelper::PendingIndexedDBInfo>&
      GetIndexedDBInfo() const;

  
  virtual void StartFetching(
      const base::Callback<void(const std::list<content::IndexedDBInfo>&)>&
          callback) OVERRIDE;

  virtual void DeleteIndexedDB(const GURL& origin) OVERRIDE {}

 private:
  virtual ~CannedBrowsingDataIndexedDBHelper();

  
  void ConvertPendingInfo();

  std::set<PendingIndexedDBInfo> pending_indexed_db_info_;

  
  
  
  
  
  
  std::list<content::IndexedDBInfo> indexed_db_info_;

  
  base::Callback<void(const std::list<content::IndexedDBInfo>&)>
      completion_callback_;

  
  
  
  
  bool is_fetching_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataIndexedDBHelper);
};

#endif  
