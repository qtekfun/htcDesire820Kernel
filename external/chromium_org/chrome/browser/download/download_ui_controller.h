// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_UI_CONTROLLER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_UI_CONTROLLER_H_

#include <set>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/download/all_download_item_notifier.h"

class Profile;

namespace content {
class WebContents;
}

class DownloadUIController : public AllDownloadItemNotifier::Observer {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate();
    virtual void NotifyDownloadStarting(content::DownloadItem* item) = 0;
  };

  
  
  
  
  
  
  
  DownloadUIController(content::DownloadManager* manager,
                       scoped_ptr<Delegate> delegate);

  virtual ~DownloadUIController();

 private:
  virtual void OnDownloadCreated(content::DownloadManager* manager,
                                 content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadUpdated(content::DownloadManager* manager,
                                 content::DownloadItem* item) OVERRIDE;

  AllDownloadItemNotifier download_notifier_;

  scoped_ptr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(DownloadUIController);
};

#endif  
