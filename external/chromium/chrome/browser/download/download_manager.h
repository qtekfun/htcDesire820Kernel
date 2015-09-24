// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_MANAGER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_ptr.h"
#include "base/time.h"
#include "chrome/browser/download/download_status_updater_delegate.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "content/browser/browser_thread.h"

class DownloadFileManager;
class DownloadHistory;
class DownloadItem;
class DownloadPrefs;
class DownloadStatusUpdater;
class GURL;
class Profile;
class ResourceDispatcherHost;
class TabContents;
struct DownloadCreateInfo;
struct DownloadSaveInfo;

namespace net {
class URLRequestContextGetter;
}

class DownloadManager
    : public base::RefCountedThreadSafe<DownloadManager,
                                        BrowserThread::DeleteOnUIThread>,
      public DownloadStatusUpdaterDelegate,
      public SelectFileDialog::Listener {
 public:
  explicit DownloadManager(DownloadStatusUpdater* status_updater);

  
  void Shutdown();

  
  
  class Observer {
   public:
    
    
    virtual void ModelChanged() = 0;

    
    
    virtual void ManagerGoingDown() {}

    
    
    
    virtual void SelectFileDialogDisplayed(int32 id) {}

   protected:
    virtual ~Observer() {}
  };

  
  void GetTemporaryDownloads(const FilePath& dir_path,
                             std::vector<DownloadItem*>* result);

  
  
  void GetAllDownloads(const FilePath& dir_path,
                       std::vector<DownloadItem*>* result);

  
  
  void GetCurrentDownloads(const FilePath& dir_path,
                           std::vector<DownloadItem*>* result);

  
  
  void SearchDownloads(const string16& query,
                       std::vector<DownloadItem*>* result);

  
  bool Init(Profile* profile);

  
  void StartDownload(DownloadCreateInfo* info);
  void UpdateDownload(int32 download_id, int64 size);
  
  
  void OnResponseCompleted(int32 download_id, int64 size, int os_error,
                           const std::string& hash);

  
  void DownloadCancelled(int32 download_id);
  void PauseDownload(int32 download_id, bool pause);
  void RemoveDownload(int64 download_handle);

  
  
  
  bool IsDownloadReadyForCompletion(DownloadItem* download);

  
  
  
  
  void MaybeCompleteDownload(DownloadItem* download);

  
  
  
  void OnDownloadRenamedToFinalName(int download_id,
                                    const FilePath& full_path,
                                    int uniquifier);

  
  
  
  int RemoveDownloadsBetween(const base::Time remove_begin,
                             const base::Time remove_end);

  
  
  
  int RemoveDownloads(const base::Time remove_begin);

  
  
  int RemoveAllDownloads();

  
  
  void DownloadCompleted(int32 download_id);

  
  
  void SavePageAsDownloadStarted(DownloadItem* download_item);

  
  void DownloadUrl(const GURL& url,
                   const GURL& referrer,
                   const std::string& referrer_encoding,
                   TabContents* tab_contents);

  
  
  
  void DownloadUrlToFile(const GURL& url,
                         const GURL& referrer,
                         const std::string& referrer_encoding,
                         const DownloadSaveInfo& save_info,
                         TabContents* tab_contents);

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

  
  void OnQueryDownloadEntriesComplete(
      std::vector<DownloadCreateInfo>* entries);
  void OnCreateDownloadEntryComplete(
      DownloadCreateInfo info, int64 db_handle);

  
  void ShowDownloadInBrowser(const DownloadCreateInfo& info,
                             DownloadItem* download);

  
  int in_progress_count() const {
    return static_cast<int>(in_progress_.size());
  }

  Profile* profile() { return profile_; }

  DownloadHistory* download_history() { return download_history_.get(); }

  DownloadPrefs* download_prefs() { return download_prefs_.get(); }

  
  void CreateDownloadItem(DownloadCreateInfo* info);

  
  void ClearLastDownloadPath();

  
  bool ShouldOpenFileBasedOnExtension(const FilePath& path) const;

  
  virtual bool IsDownloadProgressKnown();
  virtual int64 GetInProgressDownloadCount();
  virtual int64 GetReceivedDownloadBytes();
  virtual int64 GetTotalDownloadBytes();

  
  virtual void FileSelected(const FilePath& path, int index, void* params);
  virtual void FileSelectionCanceled(void* params);

  
  void DangerousDownloadValidated(DownloadItem* download);

  
  void CheckDownloadUrlDone(DownloadCreateInfo* info, bool is_dangerous_url);

  
  
  void CheckDownloadHashDone(int32 download_id, bool is_dangerous_hash);

 private:
  
  friend class DownloadManagerTest;
  friend class MockDownloadManager;

  
  
  
  class OtherDownloadManagerObserver : public Observer {
   public:
    explicit OtherDownloadManagerObserver(
        DownloadManager* observing_download_manager);
    virtual ~OtherDownloadManagerObserver();

    
    virtual void ModelChanged();
    virtual void ManagerGoingDown();

   private:
    
    DownloadManager* observing_download_manager_;

    
    DownloadManager* observed_download_manager_;
  };

  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<DownloadManager>;
  friend class OtherDownloadManagerObserver;

  ~DownloadManager();

  
  
  
  void CheckIfSuggestedPathExists(DownloadCreateInfo* info,
                                  const FilePath& default_path);

  
  
  void OnPathExistenceAvailable(DownloadCreateInfo* info);

  
  
  
  void AttachDownloadItem(DownloadCreateInfo* info);

  
  void DownloadCancelledInternal(int download_id,
                                 int render_process_id,
                                 int request_id);

  
  
  
  void OnAllDataSaved(int32 download_id, int64 size, const std::string& hash);

  
  void OnDownloadError(int32 download_id, int64 size, int os_error);

  
  void UpdateAppIcon();

  
  
  void PauseDownloadRequest(ResourceDispatcherHost* rdh,
                            int render_process_id,
                            int request_id,
                            bool pause);

  
  void NotifyModelChanged();

  DownloadItem* GetDownloadItem(int id);

  
  
  void AssertContainersConsistent() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef std::set<DownloadItem*> DownloadSet;
  typedef base::hash_map<int64, DownloadItem*> DownloadMap;

  DownloadSet downloads_;
  DownloadMap history_downloads_;
  DownloadMap in_progress_;
  DownloadMap active_downloads_;
#if !defined(NDEBUG)
  DownloadSet save_page_as_downloads_;
#endif

  
  bool shutdown_needed_;

  
  ObserverList<Observer> observers_;

  
  Profile* profile_;
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  scoped_ptr<DownloadHistory> download_history_;

  scoped_ptr<DownloadPrefs> download_prefs_;

  
  DownloadFileManager* file_manager_;

  
  base::WeakPtr<DownloadStatusUpdater> status_updater_;

  
  
  FilePath last_download_path_;

  
  
  scoped_refptr<SelectFileDialog> select_file_dialog_;

  scoped_ptr<OtherDownloadManagerObserver> other_download_manager_observer_;

  DISALLOW_COPY_AND_ASSIGN(DownloadManager);
};

#endif  
