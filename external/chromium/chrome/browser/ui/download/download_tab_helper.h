// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_DOWNLOAD_DOWNLOAD_TAB_HELPER_H_
#define CHROME_BROWSER_UI_DOWNLOAD_DOWNLOAD_TAB_HELPER_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/download/save_package.h"
#include "content/browser/tab_contents/tab_contents_observer.h"

class DownloadTabHelper : public TabContentsObserver {
 public:
  explicit DownloadTabHelper(TabContents* tab_contents);
  virtual ~DownloadTabHelper();

  
  void OnSavePage();

  
  
  void OnSaveURL(const GURL& url);

  
  
  
  bool SavePage(const FilePath& main_file, const FilePath& dir_path,
                SavePackage::SavePackageType save_type);

  
  SavePackage* save_package() const { return save_package_.get(); }

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  scoped_refptr<SavePackage> save_package_;

  DISALLOW_COPY_AND_ASSIGN(DownloadTabHelper);
};

#endif  
