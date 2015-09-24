// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_TEST_DOWNLOAD_SHELF_H_
#define CHROME_BROWSER_DOWNLOAD_TEST_DOWNLOAD_SHELF_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/download/download_shelf.h"
#include "content/public/browser/download_manager.h"

class TestDownloadShelf : public DownloadShelf,
                          public content::DownloadManager::Observer {
 public:
  TestDownloadShelf();
  virtual ~TestDownloadShelf();

  
  virtual bool IsShowing() const OVERRIDE;
  virtual bool IsClosing() const OVERRIDE;
  virtual Browser* browser() const OVERRIDE;

  
  bool did_add_download() const { return did_add_download_; }

  
  void set_download_manager(content::DownloadManager* download_manager);

  
  virtual void ManagerGoingDown(content::DownloadManager* manager) OVERRIDE;

 protected:
  virtual void DoAddDownload(content::DownloadItem* download) OVERRIDE;
  virtual void DoShow() OVERRIDE;
  virtual void DoClose(CloseReason reason) OVERRIDE;
  virtual base::TimeDelta GetTransientDownloadShowDelay() OVERRIDE;
  virtual content::DownloadManager* GetDownloadManager() OVERRIDE;

 private:
  bool is_showing_;
  bool did_add_download_;
  content::DownloadManager* download_manager_;

  DISALLOW_COPY_AND_ASSIGN(TestDownloadShelf);
};

#endif  
