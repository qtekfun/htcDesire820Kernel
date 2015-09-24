// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_STATUS_UPDATER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_STATUS_UPDATER_H_

#include <set>

#include "base/basictypes.h"
#include "chrome/browser/download/all_download_item_notifier.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"

class DownloadStatusUpdater
  : public AllDownloadItemNotifier::Observer {
 public:
  DownloadStatusUpdater();
  virtual ~DownloadStatusUpdater();

  
  
  
  
  bool GetProgress(float* progress, int* download_count) const;

  
  
  
  
  
  void AddManager(content::DownloadManager* manager);

  
  virtual void OnDownloadCreated(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadUpdated(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;

 protected:
  
  
  
  
  virtual void UpdateAppIconDownloadProgress(content::DownloadItem* download);

 private:
  std::vector<AllDownloadItemNotifier*> notifiers_;

  DISALLOW_COPY_AND_ASSIGN(DownloadStatusUpdater);
};

#endif  
