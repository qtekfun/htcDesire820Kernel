// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_ALL_DOWNLOAD_ITEM_NOTIFIER_H_
#define CHROME_BROWSER_DOWNLOAD_ALL_DOWNLOAD_ITEM_NOTIFIER_H_

#include <set>

#include "content/public/browser/download_manager.h"
#include "content/public/browser/download_item.h"



class AllDownloadItemNotifier : public content::DownloadManager::Observer,
                                public content::DownloadItem::Observer {
 public:
  
  
  
  class Observer {
   public:
    Observer() {}
    virtual ~Observer() {}

    virtual void OnDownloadCreated(
        content::DownloadManager* manager, content::DownloadItem* item) {}
    virtual void OnDownloadUpdated(
        content::DownloadManager* manager, content::DownloadItem* item) {}
    virtual void OnDownloadOpened(
        content::DownloadManager* manager, content::DownloadItem* item) {}
    virtual void OnDownloadRemoved(
        content::DownloadManager* manager, content::DownloadItem* item) {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  AllDownloadItemNotifier(content::DownloadManager* manager,
                          Observer* observer);

  virtual ~AllDownloadItemNotifier();

  
  content::DownloadManager* GetManager() const { return manager_; }

 private:
  
  virtual void ManagerGoingDown(content::DownloadManager* manager) OVERRIDE;
  virtual void OnDownloadCreated(content::DownloadManager* manager,
                                content::DownloadItem* item) OVERRIDE;

  
  virtual void OnDownloadUpdated(content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadOpened(content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadRemoved(content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadDestroyed(content::DownloadItem* item) OVERRIDE;

  content::DownloadManager* manager_;
  AllDownloadItemNotifier::Observer* observer_;
  std::set<content::DownloadItem*> observing_;

  DISALLOW_COPY_AND_ASSIGN(AllDownloadItemNotifier);
};

#endif  
