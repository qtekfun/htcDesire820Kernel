// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_HISTORY_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_HISTORY_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/download/all_download_item_notifier.h"
#include "chrome/browser/history/history_service.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"

namespace history {
struct DownloadRow;
}  

class DownloadHistory : public AllDownloadItemNotifier::Observer {
 public:
  typedef std::set<uint32> IdSet;

  
  class HistoryAdapter {
   public:
    explicit HistoryAdapter(HistoryService* history);
    virtual ~HistoryAdapter();

    virtual void QueryDownloads(
        const HistoryService::DownloadQueryCallback& callback);

    virtual void CreateDownload(
        const history::DownloadRow& info,
        const HistoryService::DownloadCreateCallback& callback);

    virtual void UpdateDownload(const history::DownloadRow& data);

    virtual void RemoveDownloads(const std::set<uint32>& ids);

   private:
    HistoryService* history_;
    DISALLOW_COPY_AND_ASSIGN(HistoryAdapter);
  };

  class Observer {
   public:
    Observer();
    virtual ~Observer();

    
    
    virtual void OnDownloadStored(content::DownloadItem* item,
                                  const history::DownloadRow& info) {}

    
    virtual void OnDownloadsRemoved(const IdSet& ids) {}

    
    
    virtual void OnDownloadHistoryDestroyed() {}
  };

  
  static bool IsPersisted(content::DownloadItem* item);

  
  
  
  DownloadHistory(
      content::DownloadManager* manager,
      scoped_ptr<HistoryAdapter> history);

  virtual ~DownloadHistory();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  typedef std::set<content::DownloadItem*> ItemSet;

  
  
  void QueryCallback(
      scoped_ptr<std::vector<history::DownloadRow> > infos);

  
  void MaybeAddToHistory(content::DownloadItem* item);

  
  
  void ItemAdded(uint32 id, bool success);

  
  virtual void OnDownloadCreated(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadUpdated(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadOpened(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadRemoved(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;

  
  
  
  void ScheduleRemoveDownload(uint32 download_id);

  
  void RemoveDownloadsBatch();

  AllDownloadItemNotifier notifier_;

  scoped_ptr<HistoryAdapter> history_;

  
  
  
  uint32 loading_id_;

  
  
  IdSet removing_ids_;

  
  
  
  
  IdSet removed_while_adding_;

  
  int64 history_size_;

  ObserverList<Observer> observers_;

  base::WeakPtrFactory<DownloadHistory> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadHistory);
};

#endif  
