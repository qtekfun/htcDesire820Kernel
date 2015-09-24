// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DOWNLOAD_MANAGER_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_DOWNLOAD_MANAGER_DELEGATE_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/save_page_type.h"

namespace content {

class BrowserContext;
class WebContents;

typedef base::Callback<void(DownloadItem*)>
    SavePackageDownloadCreatedCallback;

typedef base::Callback<void(const base::FilePath&,
                            SavePageType,
                            const SavePackageDownloadCreatedCallback&)>
    SavePackagePathPickedCallback;

typedef base::Callback<void(
    const base::FilePath& target_path,
    DownloadItem::TargetDisposition disposition,
    DownloadDangerType danger_type,
    const base::FilePath& intermediate_path)> DownloadTargetCallback;

typedef base::Callback<void(bool)> DownloadOpenDelayedCallback;

typedef base::Callback<void(bool result)> CheckForFileExistenceCallback;

typedef base::Callback<void(uint32)> DownloadIdCallback;

class CONTENT_EXPORT DownloadManagerDelegate {
 public:
  
  virtual void Shutdown() {}

  
  
  virtual void GetNextId(const DownloadIdCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool DetermineDownloadTarget(DownloadItem* item,
                                       const DownloadTargetCallback& callback);

  
  virtual bool ShouldOpenFileBasedOnExtension(const base::FilePath& path);

  
  
  
  
  
  
  virtual bool ShouldCompleteDownload(
      DownloadItem* item,
      const base::Closure& complete_callback);

  
  
  
  
  virtual bool ShouldOpenDownload(DownloadItem* item,
                                  const DownloadOpenDelayedCallback& callback);

  
  virtual bool GenerateFileHash();

  
  virtual void GetSaveDir(BrowserContext* browser_context,
                          base::FilePath* website_save_dir,
                          base::FilePath* download_save_dir,
                          bool* skip_dir_check) {}

  
  
  virtual void ChooseSavePath(
      WebContents* web_contents,
      const base::FilePath& suggested_path,
      const base::FilePath::StringType& default_extension,
      bool can_save_as_complete,
      const SavePackagePathPickedCallback& callback) {
  }

  
  virtual void OpenDownload(DownloadItem* download) {}

  
  virtual void ShowDownloadInShell(DownloadItem* download) {}

  
  virtual void CheckForFileExistence(
      DownloadItem* download,
      const CheckForFileExistenceCallback& callback) {}

  
  
  
  
  virtual std::string ApplicationClientIdForFileScanning() const;

 protected:
  virtual ~DownloadManagerDelegate();
};

}  

#endif  
