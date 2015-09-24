// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_DOWNLOAD_DATABASE_H_
#define CHROME_BROWSER_HISTORY_DOWNLOAD_DATABASE_H_
#pragma once

#include "chrome/browser/history/history_types.h"

struct DownloadCreateInfo;
class FilePath;

namespace sql {
class Connection;
}

namespace history {

class DownloadDatabase {
 public:
  
  DownloadDatabase();
  virtual ~DownloadDatabase();

  
  void QueryDownloads(std::vector<DownloadCreateInfo>* results);

  
  bool UpdateDownload(int64 received_bytes, int32 state, DownloadID db_handle);

  
  bool UpdateDownloadPath(const FilePath& path, DownloadID db_handle);

  
  
  
  
  bool CleanUpInProgressEntries();

  
  int64 CreateDownload(const DownloadCreateInfo& info);

  
  void RemoveDownload(DownloadID db_handle);

  
  
  
  
  void RemoveDownloadsBetween(base::Time remove_begin, base::Time remove_end);

 protected:
  
  virtual sql::Connection& GetDB() = 0;

  
  bool InitDownloadTable();

  
  
  bool DropDownloadTable();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadDatabase);
};

}  

#endif  
