// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MOCK_BROWSING_DATA_DATABASE_HELPER_H_
#define CHROME_BROWSER_MOCK_BROWSING_DATA_DATABASE_HELPER_H_
#pragma once

#include <map>
#include <vector>

#include "base/callback.h"

#include "chrome/browser/browsing_data_database_helper.h"

class MockBrowsingDataDatabaseHelper : public BrowsingDataDatabaseHelper {
 public:
  explicit MockBrowsingDataDatabaseHelper(Profile* profile);

  virtual void StartFetching(
      Callback1<const std::vector<DatabaseInfo>& >::Type* callback);

  virtual void CancelNotification();

  virtual void DeleteDatabase(const std::string& origin,
      const std::string& name);

  
  void AddDatabaseSamples();

  
  void Notify();

  
  void Reset();

  
  
  bool AllDeleted();

  std::string last_deleted_origin_;

  std::string last_deleted_db_;

 private:
  virtual ~MockBrowsingDataDatabaseHelper();

  Profile* profile_;

  scoped_ptr<Callback1<const std::vector<DatabaseInfo>& >::Type >
      callback_;

  
  std::map<const std::string, bool> databases_;

  std::vector<DatabaseInfo> response_;
};

#endif  
