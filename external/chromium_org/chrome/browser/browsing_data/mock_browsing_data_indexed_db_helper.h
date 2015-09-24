// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_INDEXED_DB_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_MOCK_BROWSING_DATA_INDEXED_DB_HELPER_H_

#include <list>
#include <map>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/browsing_data/browsing_data_indexed_db_helper.h"

class MockBrowsingDataIndexedDBHelper
    : public BrowsingDataIndexedDBHelper {
 public:
  MockBrowsingDataIndexedDBHelper();

  
  void AddIndexedDBSamples();

  
  void Notify();

  
  void Reset();

  
  
  bool AllDeleted();

  
  virtual void StartFetching(
      const base::Callback<void(const std::list<content::IndexedDBInfo>&)>&
          callback) OVERRIDE;
  virtual void DeleteIndexedDB(const GURL& origin) OVERRIDE;

 private:
  virtual ~MockBrowsingDataIndexedDBHelper();

  base::Callback<void(const std::list<content::IndexedDBInfo>&)> callback_;
  std::map<GURL, bool> origins_;
  std::list<content::IndexedDBInfo> response_;
};

#endif  
