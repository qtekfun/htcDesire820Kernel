// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "chrome/common/url_constants.h"
#include "googleurl/src/gurl.h"

class Profile;

class BrowsingDataLocalStorageHelper
    : public base::RefCountedThreadSafe<BrowsingDataLocalStorageHelper> {
 public:
  
  struct LocalStorageInfo {
    LocalStorageInfo();
    LocalStorageInfo(
        const std::string& protocol,
        const std::string& host,
        unsigned short port,
        const std::string& database_identifier,
        const std::string& origin,
        const FilePath& file_path,
        int64 size,
        base::Time last_modified);
    ~LocalStorageInfo();

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

  explicit BrowsingDataLocalStorageHelper(Profile* profile);

  
  
  
  virtual void StartFetching(
      Callback1<const std::vector<LocalStorageInfo>& >::Type* callback);
  
  
  
  virtual void CancelNotification();
  
  virtual void DeleteLocalStorageFile(const FilePath& file_path);

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataLocalStorageHelper>;
  virtual ~BrowsingDataLocalStorageHelper();

  
  void NotifyInUIThread();

  Profile* profile_;

  
  scoped_ptr<Callback1<const std::vector<LocalStorageInfo>& >::Type >
      completion_callback_;

  
  
  
  
  bool is_fetching_;

  
  std::vector<LocalStorageInfo> local_storage_info_;

 private:
  
  void FetchLocalStorageInfoInWebKitThread();
  
  void DeleteLocalStorageFileInWebKitThread(const FilePath& file_path);

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataLocalStorageHelper);
};

class CannedBrowsingDataLocalStorageHelper
    : public BrowsingDataLocalStorageHelper {
 public:
  explicit CannedBrowsingDataLocalStorageHelper(Profile* profile);

  
  
  
  CannedBrowsingDataLocalStorageHelper* Clone();

  
  
  void AddLocalStorage(const GURL& origin);

  
  void Reset();

  
  bool empty() const;

  
  virtual void StartFetching(
      Callback1<const std::vector<LocalStorageInfo>& >::Type* callback);
  virtual void CancelNotification() {}

 private:
  virtual ~CannedBrowsingDataLocalStorageHelper();

  
  void ConvertPendingInfoInWebKitThread();

  
  mutable base::Lock lock_;

  
  std::vector<GURL> pending_local_storage_info_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataLocalStorageHelper);
};

#endif  
