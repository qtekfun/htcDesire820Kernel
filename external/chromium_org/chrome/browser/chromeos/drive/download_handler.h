// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_DOWNLOAD_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_DOWNLOAD_HANDLER_H_

#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/download/all_download_item_notifier.h"
#include "content/public/browser/download_manager_delegate.h"

class Profile;

namespace content {
class DownloadItem;
class DownloadManager;
}

namespace drive {

class FileSystemInterface;
class ResourceEntry;

class DownloadHandler : public AllDownloadItemNotifier::Observer {
 public:
  explicit DownloadHandler(FileSystemInterface* file_system);
  virtual ~DownloadHandler();

  
  static DownloadHandler* GetForProfile(Profile* profile);

  
  void Initialize(content::DownloadManager* download_manager,
                  const base::FilePath& drive_tmp_download_path);

  
  
  typedef base::Callback<void(const base::FilePath&)>
      SubstituteDriveDownloadPathCallback;

  void SubstituteDriveDownloadPath(
      const base::FilePath& drive_path,
      content::DownloadItem* download,
      const SubstituteDriveDownloadPathCallback& callback);

  
  
  
  void SetDownloadParams(const base::FilePath& drive_path,
                         content::DownloadItem* download);

  
  base::FilePath GetTargetPath(const content::DownloadItem* download);

  
  base::FilePath GetCacheFilePath(const content::DownloadItem* download);

  
  bool IsDriveDownload(const content::DownloadItem* download);

  
  void CheckForFileExistence(
      const content::DownloadItem* download,
      const content::CheckForFileExistenceCallback& callback);

 private:
  
  virtual void OnDownloadCreated(content::DownloadManager* manager,
                                 content::DownloadItem* download) OVERRIDE;
  virtual void OnDownloadUpdated(content::DownloadManager* manager,
                                 content::DownloadItem* download) OVERRIDE;

  
  void RemoveDownload(int id);

  
  
  void OnCreateDirectory(const SubstituteDriveDownloadPathCallback& callback,
                         FileError error);

  
  void UploadDownloadItem(content::DownloadItem* download);

  
  void SetCacheFilePath(int id,
                        const base::FilePath* cache_file_path,
                        FileError error);

  FileSystemInterface* file_system_;  
  
  scoped_ptr<AllDownloadItemNotifier> notifier_;

  
  base::FilePath drive_tmp_download_path_;

  
  
  base::WeakPtrFactory<DownloadHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadHandler);
};

}  

#endif  
