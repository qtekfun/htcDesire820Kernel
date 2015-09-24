// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_SHELF_MAC_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_SHELF_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/compiler_specific.h"
#include "chrome/browser/download/download_shelf.h"

class DownloadItemModel;

@class DownloadShelfController;


class DownloadShelfMac : public DownloadShelf {
 public:
  DownloadShelfMac(Browser* browser, DownloadShelfController* controller);

  
  virtual bool IsShowing() const OVERRIDE;
  virtual bool IsClosing() const OVERRIDE;
  virtual Browser* browser() const OVERRIDE;

 protected:
  virtual void DoAddDownload(content::DownloadItem* download) OVERRIDE;
  virtual void DoShow() OVERRIDE;
  virtual void DoClose(CloseReason reason) OVERRIDE;

 private:
  
  Browser* browser_;

  DownloadShelfController* shelf_controller_;  
};

#endif  
