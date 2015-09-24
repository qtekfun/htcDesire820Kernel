// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_ANDROID_URLS_DATABASE_H_
#define CHROME_BROWSER_HISTORY_ANDROID_ANDROID_URLS_DATABASE_H_

#include "chrome/browser/history/android/android_history_types.h"

namespace history {

class AndroidURLsDatabase {
 public:
  AndroidURLsDatabase();
  virtual ~AndroidURLsDatabase();

  
  
  bool CreateAndroidURLsTable();

  
  
  AndroidURLID AddAndroidURLRow(const std::string& raw_url, URLID url_id);

  
  
  
  bool GetAndroidURLRow(URLID url_id, AndroidURLRow* row);

  
  
  bool DeleteAndroidURLRows(const std::vector<URLID>& url_ids);

  
  
  bool DeleteUnusedAndroidURLs();

  
  
  bool UpdateAndroidURLRow(AndroidURLID id,
                           const std::string&raw_url,
                           URLID url_id);

  
  
  bool ClearAndroidURLRows();

  
  bool MigrateToVersion22();

 protected:
  
  
  virtual sql::Connection& GetDB() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AndroidURLsDatabase);
};

}  

#endif  
