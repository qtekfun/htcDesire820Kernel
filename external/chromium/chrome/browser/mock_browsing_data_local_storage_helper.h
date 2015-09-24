// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MOCK_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_
#define CHROME_BROWSER_MOCK_BROWSING_DATA_LOCAL_STORAGE_HELPER_H_
#pragma once

#include <map>
#include <vector>

#include "base/callback.h"
#include "chrome/browser/browsing_data_local_storage_helper.h"

class MockBrowsingDataLocalStorageHelper
    : public BrowsingDataLocalStorageHelper {
 public:
  explicit MockBrowsingDataLocalStorageHelper(Profile* profile);

  virtual void StartFetching(
      Callback1<const std::vector<LocalStorageInfo>& >::Type* callback);

  virtual void CancelNotification();

  virtual void DeleteLocalStorageFile(const FilePath& file_path);

  
  void AddLocalStorageSamples();

  
  void Notify();

  
  void Reset();

  
  
  bool AllDeleted();

  FilePath last_deleted_file_;

 private:
  virtual ~MockBrowsingDataLocalStorageHelper();

  Profile* profile_;

  scoped_ptr<Callback1<const std::vector<LocalStorageInfo>& >::Type >
      callback_;

  std::map<const FilePath::StringType, bool> files_;

  std::vector<LocalStorageInfo> response_;
};

#endif  
