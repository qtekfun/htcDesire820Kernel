// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_

#include <list>
#include <map>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/browsing_data/browsing_data_local_storage_helper.h"

class MockBrowsingDataLocalStorageHelper
    : public BrowsingDataLocalStorageHelper {
 public:
  explicit MockBrowsingDataLocalStorageHelper(Profile* profile);

  
  virtual void StartFetching(
      const base::Callback<void(const std::list<LocalStorageInfo>&)>& callback)
          OVERRIDE;
  virtual void DeleteOrigin(const GURL& origin) OVERRIDE;

  
  void AddLocalStorageSamples();

  
  void Notify();

  
  void Reset();

  
  
  bool AllDeleted();

  GURL last_deleted_origin_;

 private:
  virtual ~MockBrowsingDataLocalStorageHelper();

  base::Callback<void(const std::list<LocalStorageInfo>&)> callback_;

  std::map<const GURL, bool> origins_;

  std::list<LocalStorageInfo> response_;
};

#endif  
