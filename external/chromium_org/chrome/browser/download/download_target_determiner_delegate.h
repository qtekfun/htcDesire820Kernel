// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TARGET_DETERMINER_DELEGATE_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_TARGET_DETERMINER_DELEGATE_H_

#include "base/callback_forward.h"

#include "chrome/browser/download/download_path_reservation_tracker.h"
#include "content/public/browser/download_danger_type.h"

class ExtensionDownloadsEventRouter;

namespace base {
class FilePath;
}

namespace content {
class DownloadItem;
}

class DownloadTargetDeterminerDelegate {
 public:
  
  
  
  
  
  
  typedef base::Callback<void(
      const base::FilePath& new_virtual_path,
      DownloadPathReservationTracker::FilenameConflictAction conflict_action)>
  NotifyExtensionsCallback;

  
  
  
  
  typedef base::Callback<void(const base::FilePath& reserved_path,
                              bool successful)> ReservedPathCallback;

  
  
  
  
  
  typedef base::Callback<void(const base::FilePath& virtual_path)>
  FileSelectedCallback;

  
  
  
  
  typedef base::Callback<void(const base::FilePath&)> LocalPathCallback;

  
  
  
  typedef base::Callback<void(content::DownloadDangerType danger_type)>
  CheckDownloadUrlCallback;

  
  
  
  typedef base::Callback<void(const std::string&)> GetFileMimeTypeCallback;

  
  
  
  virtual void NotifyExtensions(content::DownloadItem* download,
                                const base::FilePath& virtual_path,
                                const NotifyExtensionsCallback& callback) = 0;

  
  // - Whether |virtual_path| can be written to by the user. If not, the
  
  
  
  
  
  
  
  
  
  
  
  virtual void ReserveVirtualPath(
      content::DownloadItem* download,
      const base::FilePath& virtual_path,
      bool create_directory,
      DownloadPathReservationTracker::FilenameConflictAction conflict_action,
      const ReservedPathCallback& callback) = 0;

  
  
  virtual void PromptUserForDownloadPath(
      content::DownloadItem* download,
      const base::FilePath& virtual_path,
      const FileSelectedCallback& callback) = 0;

  
  
  
  
  virtual void DetermineLocalPath(content::DownloadItem* download,
                                  const base::FilePath& virtual_path,
                                  const LocalPathCallback& callback) = 0;

  
  
  virtual void CheckDownloadUrl(content::DownloadItem* download,
                                const base::FilePath& virtual_path,
                                const CheckDownloadUrlCallback& callback) = 0;

  
  virtual void GetFileMimeType(const base::FilePath& path,
                               const GetFileMimeTypeCallback& callback) = 0;
 protected:
  virtual ~DownloadTargetDeterminerDelegate();
};

#endif  
