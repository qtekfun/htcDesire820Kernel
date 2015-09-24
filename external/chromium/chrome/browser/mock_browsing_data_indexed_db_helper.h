// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MOCK_BROWSING_DATA_INDEXED_DB_HELPER_H_
#define CHROME_BROWSER_MOCK_BROWSING_DATA_INDEXED_DB_HELPER_H_
#pragma once

#include <map>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/browsing_data_indexed_db_helper.h"

class MockBrowsingDataIndexedDBHelper
    : public BrowsingDataIndexedDBHelper {
 public:
  explicit MockBrowsingDataIndexedDBHelper(Profile* profile);

  
  void AddIndexedDBSamples();

  
  void Notify();

  
  void Reset();

  
  
  bool AllDeleted();

  
  virtual void StartFetching(
      Callback1<const std::vector<IndexedDBInfo>& >::Type* callback);
  virtual void CancelNotification();
  virtual void DeleteIndexedDBFile(const FilePath& file_path);

  FilePath last_deleted_file_;

 private:
  virtual ~MockBrowsingDataIndexedDBHelper();

  Profile* profile_;

  scoped_ptr<Callback1<const std::vector<IndexedDBInfo>& >::Type >
      callback_;

  std::map<const FilePath::StringType, bool> files_;

  std::vector<IndexedDBInfo> response_;
};

#endif  
