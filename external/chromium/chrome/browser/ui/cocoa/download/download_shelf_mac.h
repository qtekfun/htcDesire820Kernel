// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_SHELF_MAC_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_SHELF_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "chrome/browser/download/download_shelf.h"

class BaseDownloadItemModel;
class CustomDrawButton;
class DownloadItemMac;

@class ShelfView;
@class DownloadShelfController;


class DownloadShelfMac : public DownloadShelf {
 public:
  explicit DownloadShelfMac(Browser* browser,
                            DownloadShelfController* controller);

  
  virtual void AddDownload(BaseDownloadItemModel* download_model);
  virtual bool IsShowing() const;
  virtual bool IsClosing() const;
  virtual void Show();
  virtual void Close();
  virtual Browser* browser() const;

 private:
  
  Browser* browser_;

  DownloadShelfController* shelf_controller_;  
};

#endif  
