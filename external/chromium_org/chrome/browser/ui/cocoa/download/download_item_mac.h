// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_MAC_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_MAC_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/download/download_item_model.h"
#include "chrome/browser/icon_manager.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"

@class DownloadItemController;

namespace gfx{
class Image;
}


class DownloadItemMac : content::DownloadItem::Observer {
 public:
  DownloadItemMac(content::DownloadItem* download,
                  DownloadItemController* controller);

  
  virtual ~DownloadItemMac();

  
  virtual void OnDownloadUpdated(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadOpened(content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadDestroyed(content::DownloadItem* download) OVERRIDE;

  DownloadItemModel* download_model() { return &download_model_; }

  
  void LoadIcon();

 private:
  
  void OnExtractIconComplete(gfx::Image* icon_bitmap);

  
  DownloadItemModel download_model_;

  
  DownloadItemController* item_controller_;  

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  base::FilePath lastFilePath_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemMac);
};

#endif  
