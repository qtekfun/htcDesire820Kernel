// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_EVENT_ROUTER_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_EVENT_ROUTER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/drive/drive_integration_service.h"
#include "chrome/browser/chromeos/drive/file_system_observer.h"
#include "chrome/browser/chromeos/drive/job_list.h"
#include "chrome/browser/chromeos/file_manager/file_watcher.h"
#include "chrome/browser/chromeos/file_manager/volume_manager_observer.h"
#include "chrome/browser/drive/drive_service_interface.h"
#include "chromeos/disks/disk_mount_manager.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "webkit/browser/fileapi/file_system_operation.h"

class PrefChangeRegistrar;
class Profile;

namespace base {
class ListValue;
}

namespace chromeos {
class NetworkState;
}

namespace file_manager {

class DesktopNotifications;

class EventRouter
    : public chromeos::NetworkStateHandlerObserver,
      public drive::FileSystemObserver,
      public drive::JobListObserver,
      public drive::DriveServiceObserver,
      public VolumeManagerObserver {
 public:
  explicit EventRouter(Profile* profile);
  virtual ~EventRouter();

  void Shutdown();

  
  void ObserveFileSystemEvents();

  typedef base::Callback<void(bool success)> BoolCallback;

  
  
  
  
  
  void AddFileWatch(const base::FilePath& local_path,
                    const base::FilePath& virtual_path,
                    const std::string& extension_id,
                    const BoolCallback& callback);

  
  void RemoveFileWatch(const base::FilePath& local_path,
                       const std::string& extension_id);

  
  void OnCopyCompleted(
      int copy_id, const GURL& source_url, const GURL& destination_url,
      base::PlatformFileError error);

  
  void OnCopyProgress(int copy_id,
                      fileapi::FileSystemOperation::CopyProgressType type,
                      const GURL& source_url,
                      const GURL& destination_url,
                      int64 size);

  
  virtual void DefaultNetworkChanged(
      const chromeos::NetworkState* network) OVERRIDE;

  
  virtual void OnJobAdded(const drive::JobInfo& job_info) OVERRIDE;
  virtual void OnJobUpdated(const drive::JobInfo& job_info) OVERRIDE;
  virtual void OnJobDone(const drive::JobInfo& job_info,
                         drive::FileError error) OVERRIDE;

  
  virtual void OnRefreshTokenInvalid() OVERRIDE;

  
  virtual void OnDirectoryChanged(
      const base::FilePath& directory_path) OVERRIDE;

  
  virtual void OnDiskAdded(
      const chromeos::disks::DiskMountManager::Disk& disk,
      bool mounting) OVERRIDE;
  virtual void OnDiskRemoved(
      const chromeos::disks::DiskMountManager::Disk& disk) OVERRIDE;
  virtual void OnDeviceAdded(const std::string& device_path) OVERRIDE;
  virtual void OnDeviceRemoved(const std::string& device_path) OVERRIDE;
  virtual void OnVolumeMounted(chromeos::MountError error_code,
                               const VolumeInfo& volume_info,
                               bool is_remounting) OVERRIDE;
  virtual void OnVolumeUnmounted(chromeos::MountError error_code,
                                 const VolumeInfo& volume_info) OVERRIDE;
  virtual void OnFormatStarted(
      const std::string& device_path, bool success) OVERRIDE;
  virtual void OnFormatCompleted(
      const std::string& device_path, bool success) OVERRIDE;

 private:
  typedef std::map<base::FilePath, FileWatcher*> WatcherMap;

  
  void OnFileManagerPrefsChanged();

  
  void HandleFileWatchNotification(const base::FilePath& path,
                                   bool got_error);

  
  void DispatchDirectoryChangeEvent(
      const base::FilePath& path,
      bool error,
      const std::vector<std::string>& extension_ids);

  
  
  
  void ShowRemovableDeviceInFileManager(const base::FilePath& mount_path);

  
  
  
  void SendDriveFileTransferEvent(bool always);

  
  struct DriveJobInfoWithStatus {
    DriveJobInfoWithStatus();
    DriveJobInfoWithStatus(const drive::JobInfo& info,
                           const std::string& status);
    drive::JobInfo job_info;
    std::string status;
  };
  std::map<drive::JobID, DriveJobInfoWithStatus> drive_jobs_;
  base::Time last_file_transfer_event_;

  WatcherMap file_watchers_;
  scoped_ptr<DesktopNotifications> notifications_;
  scoped_ptr<PrefChangeRegistrar> pref_change_registrar_;
  Profile* profile_;

  
  
  base::WeakPtrFactory<EventRouter> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(EventRouter);
};

}  

#endif  
