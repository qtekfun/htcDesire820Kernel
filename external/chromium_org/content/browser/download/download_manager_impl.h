// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_MANAGER_IMPL_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_MANAGER_IMPL_H_

#include <map>
#include <set>

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/synchronization/lock.h"
#include "content/browser/download/download_item_impl_delegate.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/download_manager_delegate.h"
#include "content/public/browser/download_url_parameters.h"

namespace net {
class BoundNetLog;
}

namespace content {
class DownloadFileFactory;
class DownloadItemFactory;
class DownloadItemImpl;
class DownloadRequestHandleInterface;

class CONTENT_EXPORT DownloadManagerImpl : public DownloadManager,
                                           private DownloadItemImplDelegate {
 public:
  typedef base::Callback<void(DownloadItemImpl*)> DownloadItemImplCreated;

  
  
  DownloadManagerImpl(net::NetLog* net_log, BrowserContext* browser_context);
  virtual ~DownloadManagerImpl();

  

  
  
  
  virtual void CreateSavePackageDownloadItem(
      const base::FilePath& main_file_path,
      const GURL& page_url,
      const std::string& mime_type,
      scoped_ptr<DownloadRequestHandleInterface> request_handle,
      const DownloadItemImplCreated& item_created);

  
  void OnSavePackageSuccessfullyFinished(DownloadItem* download_item);

  
  virtual void SetDelegate(DownloadManagerDelegate* delegate) OVERRIDE;
  virtual DownloadManagerDelegate* GetDelegate() const OVERRIDE;
  virtual void Shutdown() OVERRIDE;
  virtual void GetAllDownloads(DownloadVector* result) OVERRIDE;
  virtual void StartDownload(
      scoped_ptr<DownloadCreateInfo> info,
      scoped_ptr<ByteStreamReader> stream,
      const DownloadUrlParameters::OnStartedCallback& on_started) OVERRIDE;
  virtual int RemoveDownloadsBetween(base::Time remove_begin,
                                     base::Time remove_end) OVERRIDE;
  virtual int RemoveDownloads(base::Time remove_begin) OVERRIDE;
  virtual int RemoveAllDownloads() OVERRIDE;
  virtual void DownloadUrl(scoped_ptr<DownloadUrlParameters> params) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual content::DownloadItem* CreateDownloadItem(
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
      content::DownloadItem::DownloadState state,
      DownloadDangerType danger_type,
      DownloadInterruptReason interrupt_reason,
      bool opened) OVERRIDE;
  virtual int InProgressCount() const OVERRIDE;
  virtual int NonMaliciousInProgressCount() const OVERRIDE;
  virtual BrowserContext* GetBrowserContext() const OVERRIDE;
  virtual void CheckForHistoryFilesRemoval() OVERRIDE;
  virtual DownloadItem* GetDownload(uint32 id) OVERRIDE;

  
  void SetDownloadItemFactoryForTesting(
      scoped_ptr<DownloadItemFactory> item_factory);
  void SetDownloadFileFactoryForTesting(
      scoped_ptr<DownloadFileFactory> file_factory);
  virtual DownloadFileFactory* GetDownloadFileFactoryForTesting();

 private:
  typedef std::set<DownloadItem*> DownloadSet;
  typedef base::hash_map<uint32, DownloadItemImpl*> DownloadMap;
  typedef std::vector<DownloadItemImpl*> DownloadItemImplVector;

  
  friend class DownloadManagerTest;
  friend class DownloadTest;

  void StartDownloadWithId(
      scoped_ptr<DownloadCreateInfo> info,
      scoped_ptr<ByteStreamReader> stream,
      const DownloadUrlParameters::OnStartedCallback& on_started,
      bool new_download,
      uint32 id);

  void CreateSavePackageDownloadItemWithId(
      const base::FilePath& main_file_path,
      const GURL& page_url,
      const std::string& mime_type,
      scoped_ptr<DownloadRequestHandleInterface> request_handle,
      const DownloadItemImplCreated& on_started,
      uint32 id);

  
  
  DownloadItemImpl* CreateActiveItem(uint32 id,
                                     const DownloadCreateInfo& info);

  
  
  void GetNextId(const DownloadIdCallback& callback);

  
  
  
  void OnFileExistenceChecked(uint32 download_id, bool result);

  
  
  virtual void DetermineDownloadTarget(
      DownloadItemImpl* item, const DownloadTargetCallback& callback) OVERRIDE;
  virtual bool ShouldCompleteDownload(
      DownloadItemImpl* item, const base::Closure& complete_callback) OVERRIDE;
  virtual bool ShouldOpenFileBasedOnExtension(
      const base::FilePath& path) OVERRIDE;
  virtual bool ShouldOpenDownload(
      DownloadItemImpl* item,
      const ShouldOpenDownloadCallback& callback) OVERRIDE;
  virtual void CheckForFileRemoval(DownloadItemImpl* download_item) OVERRIDE;
  virtual void ResumeInterruptedDownload(
      scoped_ptr<content::DownloadUrlParameters> params,
      uint32 id) OVERRIDE;
  virtual void OpenDownload(DownloadItemImpl* download) OVERRIDE;
  virtual void ShowDownloadInShell(DownloadItemImpl* download) OVERRIDE;
  virtual void DownloadRemoved(DownloadItemImpl* download) OVERRIDE;

  
  scoped_ptr<DownloadItemFactory> item_factory_;

  
  scoped_ptr<DownloadFileFactory> file_factory_;

  
  
  
  
  DownloadMap downloads_;

  int history_size_;

  
  bool shutdown_needed_;

  
  ObserverList<Observer> observers_;

  
  BrowserContext* browser_context_;

  
  DownloadManagerDelegate* delegate_;

  net::NetLog* net_log_;

  base::WeakPtrFactory<DownloadManagerImpl> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadManagerImpl);
};

}  

#endif  
