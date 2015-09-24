// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_MANAGER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_FILE_MANAGER_H_
#pragma once

#include <map>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/timer.h"
#include "ui/gfx/native_widget_types.h"

struct DownloadBuffer;
struct DownloadCreateInfo;
struct DownloadSaveInfo;
class DownloadFile;
class DownloadManager;
class FilePath;
class GURL;
class ResourceDispatcherHost;

namespace net {
class URLRequestContextGetter;
}

class DownloadFileManager
    : public base::RefCountedThreadSafe<DownloadFileManager> {
 public:
  explicit DownloadFileManager(ResourceDispatcherHost* rdh);

  
  void Shutdown();

  
  int GetNextId();

  
  void StartDownload(DownloadCreateInfo* info);

  
  
  void UpdateDownload(int id, DownloadBuffer* buffer);
  
  
  
  
  
  void OnResponseCompleted(int id,
                           DownloadBuffer* buffer,
                           int os_error,
                           const std::string& security_info);

  
  
  
  
  
  void CancelDownload(int id);
  void CompleteDownload(int id);

  
  void OnDownloadManagerShutdown(DownloadManager* manager);

  
  
  void RenameInProgressDownloadFile(int id, const FilePath& full_path);

  
  
  
  
  
  void RenameCompletingDownloadFile(int id,
                                    const FilePath& full_path,
                                    bool overwrite_existing_file);

  
  
  int NumberOfActiveDownloads() const {
    return downloads_.size();
  }

 private:
  friend class base::RefCountedThreadSafe<DownloadFileManager>;
  friend class DownloadManagerTest;
  FRIEND_TEST_ALL_PREFIXES(DownloadManagerTest, StartDownload);

  ~DownloadFileManager();

  
  void StartUpdateTimer();
  void StopUpdateTimer();
  void UpdateInProgressDownloads();

  
  void OnShutdown();

  
  
  void CreateDownloadFile(DownloadCreateInfo* info,
                          DownloadManager* download_manager,
                          bool hash_needed);

  
  
  void ResumeDownloadRequest(int child_id, int request_id);

  
  DownloadFile* GetDownloadFile(int id);

  
  
  void CancelDownloadOnRename(int id);

  
  
  void EraseDownload(int id);

  
  int next_id_;

  typedef base::hash_map<int, DownloadFile*> DownloadFileMap;

  
  DownloadFileMap downloads_;

  
  
  base::RepeatingTimer<DownloadFileManager> update_timer_;

  ResourceDispatcherHost* resource_dispatcher_host_;

  DISALLOW_COPY_AND_ASSIGN(DownloadFileManager);
};

#endif  
