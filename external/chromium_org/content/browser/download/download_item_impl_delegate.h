// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_IMPL_DELEGATE_H_
#define CONTENT_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_IMPL_DELEGATE_H_

#include "base/callback.h"
#include "base/files/file_path.h"
#include "content/common/content_export.h"
#include "content/public/browser/download_danger_type.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_url_parameters.h"

namespace content {
class DownloadItemImpl;
class BrowserContext;

class CONTENT_EXPORT DownloadItemImplDelegate {
 public:
  typedef base::Callback<void(
      const base::FilePath&,            
      DownloadItem::TargetDisposition,  
      DownloadDangerType,
      const base::FilePath&             
                              )> DownloadTargetCallback;

  
  
  typedef base::Callback<void(bool)> ShouldOpenDownloadCallback;

  DownloadItemImplDelegate();
  virtual ~DownloadItemImplDelegate();

  
  void Attach();
  void Detach();

  
  virtual void DetermineDownloadTarget(
      DownloadItemImpl* download, const DownloadTargetCallback& callback);

  
  
  
  
  virtual bool ShouldCompleteDownload(
      DownloadItemImpl* download,
      const base::Closure& complete_callback);

  
  
  virtual bool ShouldOpenDownload(
      DownloadItemImpl* download, const ShouldOpenDownloadCallback& callback);

  
  virtual bool ShouldOpenFileBasedOnExtension(const base::FilePath& path);

  
  
  
  
  virtual void CheckForFileRemoval(DownloadItemImpl* download_item);

  
  virtual void ResumeInterruptedDownload(
      scoped_ptr<content::DownloadUrlParameters> params,
      uint32 id);

  
  virtual BrowserContext* GetBrowserContext() const;

  
  virtual void UpdatePersistence(DownloadItemImpl* download);

  
  virtual void OpenDownload(DownloadItemImpl* download);

  
  virtual void ShowDownloadInShell(DownloadItemImpl* download);

  
  
  virtual void DownloadRemoved(DownloadItemImpl* download);

  
  virtual void AssertStateConsistent(DownloadItemImpl* download) const;

 private:
  
  int count_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemImplDelegate);
};

}  

#endif  
