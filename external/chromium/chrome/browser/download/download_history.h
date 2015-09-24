// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_HISTORY_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_HISTORY_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/history/history.h"
#include "content/browser/cancelable_request.h"

class DownloadItem;
class Profile;

namespace base {
class Time;
}

class DownloadHistory {
 public:
  
  
  static const int kUninitializedHandle;

  explicit DownloadHistory(Profile* profile);
  ~DownloadHistory();

  
  void Load(HistoryService::DownloadQueryCallback* callback);

  
  void AddEntry(const DownloadCreateInfo& info,
                DownloadItem* download_item,
                HistoryService::DownloadCreateCallback* callback);

  
  void UpdateEntry(DownloadItem* download_item);

  
  void UpdateDownloadPath(DownloadItem* download_item,
                          const FilePath& new_path);

  
  void RemoveEntry(DownloadItem* download_item);

  
  void RemoveEntriesBetween(const base::Time remove_begin,
                            const base::Time remove_end);

  
  int64 GetNextFakeDbHandle();

 private:
  Profile* profile_;

  
  
  
  
  int64 next_fake_db_handle_;

  CancelableRequestConsumer history_consumer_;

  DISALLOW_COPY_AND_ASSIGN(DownloadHistory);
};

#endif  
