// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_

#include <list>
#include <set>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/dom_storage_context.h"
#include "url/gurl.h"

class Profile;

class BrowsingDataLocalStorageHelper
    : public base::RefCounted<BrowsingDataLocalStorageHelper> {
 public:
  
  struct LocalStorageInfo {
    LocalStorageInfo(
        const GURL& origin_url,
        int64 size,
        base::Time last_modified);
    ~LocalStorageInfo();

    GURL origin_url;
    int64 size;
    base::Time last_modified;
  };

  explicit BrowsingDataLocalStorageHelper(Profile* profile);

  
  
  virtual void StartFetching(
      const base::Callback<void(const std::list<LocalStorageInfo>&)>& callback);

  
  virtual void DeleteOrigin(const GURL& origin);

 protected:
  friend class base::RefCounted<BrowsingDataLocalStorageHelper>;
  virtual ~BrowsingDataLocalStorageHelper();

  void CallCompletionCallback();

  content::DOMStorageContext* dom_storage_context_;  
  base::Callback<void(const std::list<LocalStorageInfo>&)> completion_callback_;
  bool is_fetching_;
  std::list<LocalStorageInfo> local_storage_info_;

 private:
  void GetUsageInfoCallback(
      const std::vector<content::LocalStorageUsageInfo>& infos);

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

  
  size_t GetLocalStorageCount() const;

  
  const std::set<GURL>& GetLocalStorageInfo() const;

  
  virtual void StartFetching(
      const base::Callback<void(const std::list<LocalStorageInfo>&)>& callback)
          OVERRIDE;

 private:
  virtual ~CannedBrowsingDataLocalStorageHelper();

  
  void ConvertPendingInfo();

  std::set<GURL> pending_local_storage_info_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataLocalStorageHelper);
};

#endif  
