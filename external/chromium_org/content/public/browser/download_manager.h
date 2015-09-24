// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DOWNLOAD_MANAGER_H_
#define CONTENT_PUBLIC_BROWSER_DOWNLOAD_MANAGER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/time/time.h"
#include "content/public/browser/download_interrupt_reasons.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_url_parameters.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"

class GURL;

namespace content {

class BrowserContext;
class ByteStreamReader;
class DownloadManagerDelegate;
class DownloadQuery;
class DownloadRequestHandle;
struct DownloadCreateInfo;

class CONTENT_EXPORT DownloadManager : public base::SupportsUserData::Data {
 public:
  virtual ~DownloadManager() {}

  
  
  virtual void SetDelegate(DownloadManagerDelegate* delegate) = 0;
  virtual DownloadManagerDelegate* GetDelegate() const = 0;

  
  
  
  
  virtual void Shutdown() = 0;

  
  
  class CONTENT_EXPORT Observer {
   public:
    
    
    
    
    
    
    
    virtual void OnDownloadCreated(
        DownloadManager* manager, DownloadItem* item) {}

    
    virtual void OnSavePackageSuccessfullyFinished(
        DownloadManager* manager, DownloadItem* item) {}

    
    
    virtual void ManagerGoingDown(DownloadManager* manager) {}

   protected:
    virtual ~Observer() {}
  };

  typedef std::vector<DownloadItem*> DownloadVector;

  
  
  virtual void GetAllDownloads(DownloadVector* downloads) = 0;

  
  
  
  
  virtual void StartDownload(
      scoped_ptr<DownloadCreateInfo> info,
      scoped_ptr<ByteStreamReader> stream,
      const DownloadUrlParameters::OnStartedCallback& on_started) = 0;

  
  
  
  virtual int RemoveDownloadsBetween(base::Time remove_begin,
                                     base::Time remove_end) = 0;

  
  
  
  virtual int RemoveDownloads(base::Time remove_begin) = 0;

  
  
  virtual int RemoveAllDownloads() = 0;

  
  virtual void DownloadUrl(scoped_ptr<DownloadUrlParameters> parameters) = 0;

  
  virtual void AddObserver(Observer* observer) = 0;

  
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  virtual DownloadItem* CreateDownloadItem(
      uint32 id,
      const base::FilePath& current_path,
      const base::FilePath& target_path,
      const std::vector<GURL>& url_chain,
      const GURL& referrer_url,
      const base::Time& start_time,
      const base::Time& end_time,
      const std::string& etag,
      const std::string& last_modified,
      int64 received_bytes,
      int64 total_bytes,
      DownloadItem::DownloadState state,
      DownloadDangerType danger_type,
      DownloadInterruptReason interrupt_reason,
      bool opened) = 0;

  
  
  
  virtual int InProgressCount() const = 0;

  
  
  
  
  virtual int NonMaliciousInProgressCount() const = 0;

  virtual BrowserContext* GetBrowserContext() const = 0;

  
  
  
  virtual void CheckForHistoryFilesRemoval() = 0;

  
  
  virtual DownloadItem* GetDownload(uint32 id) = 0;
};

}  

#endif  
