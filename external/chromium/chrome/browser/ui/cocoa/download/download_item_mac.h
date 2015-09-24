// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_MAC_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/download/download_manager.h"
#include "chrome/browser/icon_manager.h"
#include "content/browser/cancelable_request.h"

class BaseDownloadItemModel;
@class DownloadItemController;

namespace gfx{
class Image;
}


class DownloadItemMac : DownloadItem::Observer {
 public:
  
  DownloadItemMac(BaseDownloadItemModel* download_model,
                  DownloadItemController* controller);

  
  ~DownloadItemMac();

  
  virtual void OnDownloadUpdated(DownloadItem* download);
  virtual void OnDownloadOpened(DownloadItem* download);

  BaseDownloadItemModel* download_model() { return download_model_.get(); }

  
  void LoadIcon();

 private:
  
  void OnExtractIconComplete(IconManager::Handle handle,
                             gfx::Image* icon_bitmap);

  
  scoped_ptr<BaseDownloadItemModel> download_model_;

  
  DownloadItemController* item_controller_;  

  
  CancelableRequestConsumerT<int, 0> icon_consumer_;

  
  FilePath lastFilePath_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemMac);
};

#endif  
