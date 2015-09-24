// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_DOWNLOAD_DATABASE_H_
#define CHROME_BROWSER_HISTORY_DOWNLOAD_DATABASE_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/threading/platform_thread.h"
#include "content/public/browser/download_item.h"
#include "sql/meta_table.h"

namespace sql {
class Connection;
}

namespace history {

struct DownloadRow;

class DownloadDatabase {
 public:
  
  DownloadDatabase();
  virtual ~DownloadDatabase();

  void GetNextDownloadId(uint32* id);

  
  void QueryDownloads(
      std::vector<DownloadRow>* results);

  
  
  
  bool UpdateDownload(const DownloadRow& data);

  
  
  bool CreateDownload(const DownloadRow& info);

  
  void RemoveDownload(uint32 id);

  size_t CountDownloads();

 protected:
  
  virtual sql::Connection& GetDB() = 0;

  
  
  
  bool MigrateDownloadsState();

  
  
  bool MigrateDownloadsReasonPathsAndDangerType();

  
  
  bool MigrateReferrer();

  
  
  bool MigrateDownloadedByExtension();

  
  
  bool MigrateDownloadValidators();

  
  bool InitDownloadTable();

  
  
  bool DropDownloadTable();

 private:
  FRIEND_TEST_ALL_PREFIXES(
      HistoryBackendDBTest, ConfirmDownloadInProgressCleanup);

  
  static const int kStateInvalid;
  static const int kStateInProgress;
  static const int kStateComplete;
  static const int kStateCancelled;
  static const int kStateBug140687;
  static const int kStateInterrupted;

  
  static const int kDangerTypeInvalid;
  static const int kDangerTypeNotDangerous;
  static const int kDangerTypeDangerousFile;
  static const int kDangerTypeDangerousUrl;
  static const int kDangerTypeDangerousContent;
  static const int kDangerTypeMaybeDangerousContent;
  static const int kDangerTypeUncommonContent;
  static const int kDangerTypeUserValidated;
  static const int kDangerTypeDangerousHost;
  static const int kDangerTypePotentiallyUnwanted;

  
  
  
  
  
  void EnsureInProgressEntriesCleanedUp();

  bool EnsureColumnExists(const std::string& name, const std::string& type);

  void RemoveDownloadURLs(uint32 id);

  
  
  static int StateToInt(content::DownloadItem::DownloadState state);
  static content::DownloadItem::DownloadState IntToState(int state);
  static int DangerTypeToInt(content::DownloadDangerType danger_type);
  static content::DownloadDangerType IntToDangerType(int danger_type);

  bool owning_thread_set_;
  base::PlatformThreadId owning_thread_;

  
  
  
  
  
  bool in_progress_entry_cleanup_completed_;

  DISALLOW_COPY_AND_ASSIGN(DownloadDatabase);
};

}  

#endif  
