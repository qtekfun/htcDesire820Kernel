// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_DRIVE_INTEGRATION_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_DRIVE_INTEGRATION_SERVICE_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/file_system_util.h"
#include "chrome/browser/chromeos/drive/job_scheduler.h"
#include "chrome/browser/drive/drive_notification_observer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace drive {

class DebugInfoCollector;
class DownloadHandler;
class DriveAppRegistry;
class DriveServiceInterface;
class FileSystemInterface;
class JobListInterface;

namespace internal {
class FileCache;
class ResourceMetadata;
class ResourceMetadataStorage;
}  

class DriveIntegrationServiceObserver {
 public:
  
  virtual void OnFileSystemMounted() {
  }

  
  virtual void OnFileSystemBeingUnmounted() {
  }

 protected:
  virtual ~DriveIntegrationServiceObserver() {}
};

class DriveIntegrationService
    : public BrowserContextKeyedService,
      public DriveNotificationObserver {
 public:
  class PreferenceWatcher;

  
  
  
  
  
  
  DriveIntegrationService(
      Profile* profile,
      PreferenceWatcher* preference_watcher,
      DriveServiceInterface* test_drive_service,
      const base::FilePath& test_cache_root,
      FileSystemInterface* test_file_system);
  virtual ~DriveIntegrationService();

  
  virtual void Shutdown() OVERRIDE;

  void SetEnabled(bool enabled);
  bool is_enabled() const { return enabled_; }

  bool IsMounted() const;

  
  void AddObserver(DriveIntegrationServiceObserver* observer);
  void RemoveObserver(DriveIntegrationServiceObserver* observer);

  
  virtual void OnNotificationReceived() OVERRIDE;
  virtual void OnPushNotificationEnabled(bool enabled) OVERRIDE;

  DriveServiceInterface* drive_service() {
    return drive_service_.get();
  }

  DebugInfoCollector* debug_info_collector() {
    return debug_info_collector_.get();
  }
  FileSystemInterface* file_system() { return file_system_.get(); }
  DownloadHandler* download_handler() { return download_handler_.get(); }
  DriveAppRegistry* drive_app_registry() { return drive_app_registry_.get(); }
  JobListInterface* job_list() { return scheduler_.get(); }

  
  
  
  
  void ClearCacheAndRemountFileSystem(
      const base::Callback<void(bool)>& callback);

 private:
  enum State {
    NOT_INITIALIZED,
    INITIALIZING,
    INITIALIZED,
    REMOUNTING,
  };

  
  
  bool IsDriveEnabled();

  
  void AddDriveMountPoint();
  
  void RemoveDriveMountPoint();

  
  
  void AddBackDriveMountPoint(const base::Callback<void(bool)>& callback,
                              FileError error);

  
  
  void Initialize();

  
  
  void InitializeAfterMetadataInitialized(FileError error);

  
  
  void AvoidDriveAsDownloadDirecotryPreference();

  friend class DriveIntegrationServiceFactory;

  Profile* profile_;
  State state_;
  bool enabled_;

  base::FilePath cache_root_directory_;
  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  scoped_ptr<internal::ResourceMetadataStorage,
             util::DestroyHelper> metadata_storage_;
  scoped_ptr<internal::FileCache, util::DestroyHelper> cache_;
  scoped_ptr<DriveServiceInterface> drive_service_;
  scoped_ptr<JobScheduler> scheduler_;
  scoped_ptr<DriveAppRegistry> drive_app_registry_;
  scoped_ptr<internal::ResourceMetadata,
             util::DestroyHelper> resource_metadata_;
  scoped_ptr<FileSystemInterface> file_system_;
  scoped_ptr<DownloadHandler> download_handler_;
  scoped_ptr<DebugInfoCollector> debug_info_collector_;

  ObserverList<DriveIntegrationServiceObserver> observers_;
  scoped_ptr<PreferenceWatcher> preference_watcher_;

  
  
  base::WeakPtrFactory<DriveIntegrationService> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(DriveIntegrationService);
};

class DriveIntegrationServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  typedef base::Callback<DriveIntegrationService*(Profile* profile)>
      FactoryCallback;

  
  
  static DriveIntegrationService* GetForProfile(Profile* profile);

  
  static DriveIntegrationService* GetForProfileRegardlessOfStates(
      Profile* profile);

  
  
  static DriveIntegrationService* FindForProfile(Profile* profile);

  
  static DriveIntegrationService* FindForProfileRegardlessOfStates(
      Profile* profile);

  
  static DriveIntegrationServiceFactory* GetInstance();

  
  static void SetFactoryForTest(const FactoryCallback& factory_for_test);

 private:
  friend struct DefaultSingletonTraits<DriveIntegrationServiceFactory>;

  DriveIntegrationServiceFactory();
  virtual ~DriveIntegrationServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

  FactoryCallback factory_for_test_;
};

}  

#endif  
