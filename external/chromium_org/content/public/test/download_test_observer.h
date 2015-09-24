// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_DOWNLOAD_TEST_OBSERVER_H_
#define CONTENT_TEST_DOWNLOAD_TEST_OBSERVER_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/download_url_parameters.h"
#include "net/base/net_errors.h"

namespace content {

class DownloadUpdatedObserver : public DownloadItem::Observer {
 public:
  typedef base::Callback<bool(DownloadItem*)> EventFilter;

  
  
  DownloadUpdatedObserver(DownloadItem* item, EventFilter filter);
  virtual ~DownloadUpdatedObserver();

  
  
  
  
  bool WaitForEvent();

 private:
  
  virtual void OnDownloadUpdated(DownloadItem* item) OVERRIDE;
  virtual void OnDownloadDestroyed(DownloadItem* item) OVERRIDE;

  DownloadItem* item_;
  EventFilter filter_;
  bool waiting_;
  bool event_seen_;

  DISALLOW_COPY_AND_ASSIGN(DownloadUpdatedObserver);
};

class DownloadTestObserver : public DownloadManager::Observer,
                             public DownloadItem::Observer {
 public:
  
  enum DangerousDownloadAction {
    ON_DANGEROUS_DOWNLOAD_ACCEPT,  
    ON_DANGEROUS_DOWNLOAD_DENY,    
    ON_DANGEROUS_DOWNLOAD_FAIL,    
    ON_DANGEROUS_DOWNLOAD_IGNORE,  
    ON_DANGEROUS_DOWNLOAD_QUIT     
  };

  
  
  DownloadTestObserver(DownloadManager* download_manager,
                       size_t wait_count,
                       DangerousDownloadAction dangerous_download_action);

  virtual ~DownloadTestObserver();

  
  void WaitForFinished();

  
  bool IsFinished() const;

  
  virtual void OnDownloadUpdated(DownloadItem* download) OVERRIDE;
  virtual void OnDownloadDestroyed(DownloadItem* download) OVERRIDE;

  
  virtual void OnDownloadCreated(
      DownloadManager* manager, DownloadItem* item) OVERRIDE;
  virtual void ManagerGoingDown(DownloadManager* manager) OVERRIDE;

  size_t NumDangerousDownloadsSeen() const;

  size_t NumDownloadsSeenInState(DownloadItem::DownloadState state) const;

 protected:
  
  virtual void Init();

  
  virtual bool IsDownloadInFinalState(DownloadItem* download) = 0;

 private:
  typedef std::set<DownloadItem*> DownloadSet;

  
  typedef std::map<DownloadItem::DownloadState, size_t> StateMap;

  
  
  
  
  void DownloadInFinalState(DownloadItem* download);

  void SignalIfFinished();

  
  void AcceptDangerousDownload(uint32 download_id);

  
  void DenyDangerousDownload(uint32 download_id);

  
  DownloadManager* download_manager_;

  
  
  
  DownloadSet finished_downloads_;

  
  
  
  DownloadSet downloads_observed_;

  
  
  
  
  StateMap states_observed_;

  
  size_t wait_count_;

  
  
  
  
  
  
  
  int finished_downloads_at_construction_;

  
  
  bool waiting_;

  
  DangerousDownloadAction dangerous_download_action_;

  
  std::set<uint32> dangerous_downloads_seen_;

  base::WeakPtrFactory<DownloadTestObserver> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadTestObserver);
};

class DownloadTestObserverTerminal : public DownloadTestObserver {
 public:
  
  
  
  DownloadTestObserverTerminal(
      DownloadManager* download_manager,
      size_t wait_count,
      DangerousDownloadAction dangerous_download_action);

  virtual ~DownloadTestObserverTerminal();

 private:
  virtual bool IsDownloadInFinalState(DownloadItem* download) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DownloadTestObserverTerminal);
};

class DownloadTestObserverInProgress : public DownloadTestObserver {
 public:
  
  
  DownloadTestObserverInProgress(
      DownloadManager* download_manager, size_t wait_count);

  virtual ~DownloadTestObserverInProgress();

 private:
  virtual bool IsDownloadInFinalState(DownloadItem* download) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DownloadTestObserverInProgress);
};

class DownloadTestObserverInterrupted : public DownloadTestObserver {
 public:
  
  
  DownloadTestObserverInterrupted(
      DownloadManager* download_manager,
      size_t wait_count,
      DangerousDownloadAction dangerous_download_action);

  virtual ~DownloadTestObserverInterrupted();

 private:
  virtual bool IsDownloadInFinalState(DownloadItem* download) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DownloadTestObserverInterrupted);
};

class DownloadTestFlushObserver
    : public DownloadManager::Observer,
      public DownloadItem::Observer,
      public base::RefCountedThreadSafe<DownloadTestFlushObserver> {
 public:
  explicit DownloadTestFlushObserver(DownloadManager* download_manager);

  void WaitForFlush();

  
  virtual void OnDownloadCreated(
      DownloadManager* manager,
      DownloadItem* item) OVERRIDE;

  
  virtual void OnDownloadUpdated(DownloadItem* download) OVERRIDE;
  virtual void OnDownloadDestroyed(DownloadItem* download) OVERRIDE;

 protected:
  friend class base::RefCountedThreadSafe<DownloadTestFlushObserver>;

  virtual ~DownloadTestFlushObserver();

 private:
  typedef std::set<DownloadItem*> DownloadSet;

  
  
  
  void CheckDownloadsInProgress(bool observe_downloads);

  void PingFileThread(int cycle);

  void PingIOThread(int cycle);

  DownloadManager* download_manager_;
  DownloadSet downloads_observed_;
  bool waiting_for_zero_inprogress_;

  DISALLOW_COPY_AND_ASSIGN(DownloadTestFlushObserver);
};

class DownloadTestItemCreationObserver
    : public base::RefCountedThreadSafe<DownloadTestItemCreationObserver> {
 public:
  DownloadTestItemCreationObserver();

  void WaitForDownloadItemCreation();

  uint32 download_id() const { return download_id_; }
  net::Error error() const { return error_; }
  bool started() const { return called_back_count_ > 0; }
  bool succeeded() const { return started() && (error_ == net::OK); }

  const DownloadUrlParameters::OnStartedCallback callback();

 private:
  friend class base::RefCountedThreadSafe<DownloadTestItemCreationObserver>;

  ~DownloadTestItemCreationObserver();

  void DownloadItemCreationCallback(DownloadItem* item, net::Error error);

  
  uint32 download_id_;
  net::Error error_;

  
  size_t called_back_count_;

  
  bool waiting_;

  DISALLOW_COPY_AND_ASSIGN(DownloadTestItemCreationObserver);
};

}  

#endif  
