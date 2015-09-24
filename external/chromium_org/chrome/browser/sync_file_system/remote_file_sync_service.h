// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_REMOTE_FILE_SYNC_SERVICE_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_REMOTE_FILE_SYNC_SERVICE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/sync_file_system/conflict_resolution_policy.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_file_metadata.h"
#include "webkit/browser/fileapi/file_system_url.h"

class BrowserContextKeyedServiceFactory;
class GURL;

namespace base {
class ListValue;
}

namespace content {
class BrowserContext;
}

namespace webkit_blob {
class ScopedFile;
}

namespace sync_file_system {

class FileStatusObserver;
class LocalChangeProcessor;
class RemoteChangeProcessor;

enum RemoteServiceState {
  
  
  
  REMOTE_SERVICE_OK,

  
  
  
  
  
  
  REMOTE_SERVICE_TEMPORARY_UNAVAILABLE,

  
  
  
  
  
  REMOTE_SERVICE_AUTHENTICATION_REQUIRED,

  
  
  
  
  REMOTE_SERVICE_DISABLED,
};

class RemoteFileSyncService {
 public:
  enum BackendVersion {
    V1,
    V2,
  };

  class Observer {
   public:
    Observer() {}
    virtual ~Observer() {}

    
    
    
    
    
    virtual void OnRemoteChangeQueueUpdated(int64 pending_changes_hint) = 0;

    
    virtual void OnRemoteServiceStateUpdated(
        RemoteServiceState state,
        const std::string& description) {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  struct Version {
    std::string id;
    SyncFileMetadata metadata;
  };

  enum UninstallFlag {
    UNINSTALL_AND_PURGE_REMOTE,
    UNINSTALL_AND_KEEP_REMOTE,
  };

  
  typedef std::map<GURL, std::string> OriginStatusMap;

  
  typedef base::Callback<void(SyncStatusCode status,
                              const std::vector<Version>& versions)>
      RemoteVersionsCallback;
  typedef base::Callback<void(SyncStatusCode status,
                              webkit_blob::ScopedFile downloaded)>
      DownloadVersionCallback;

  
  
  static scoped_ptr<RemoteFileSyncService> CreateForBrowserContext(
      BackendVersion version,
      content::BrowserContext* context);

  
  
  static void AppendDependsOnFactories(
      BackendVersion version,
      std::set<BrowserContextKeyedServiceFactory*>* factories);

  RemoteFileSyncService() {}
  virtual ~RemoteFileSyncService() {}

  
  virtual void AddServiceObserver(Observer* observer) = 0;
  virtual void AddFileStatusObserver(FileStatusObserver* observer) = 0;

  
  
  
  
  
  virtual void RegisterOrigin(
      const GURL& origin,
      const SyncStatusCallback& callback) = 0;

  
  
  virtual void EnableOrigin(
      const GURL& origin,
      const SyncStatusCallback& callback) = 0;

  virtual void DisableOrigin(
      const GURL& origin,
      const SyncStatusCallback& callback) = 0;

  
  
  virtual void UninstallOrigin(
      const GURL& origin,
      UninstallFlag flag,
      const SyncStatusCallback& callback) = 0;

  
  
  
  
  virtual void ProcessRemoteChange(const SyncFileCallback& callback) = 0;

  
  
  virtual void SetRemoteChangeProcessor(
      RemoteChangeProcessor* processor) = 0;

  
  
  virtual LocalChangeProcessor* GetLocalChangeProcessor() = 0;

  
  virtual bool IsConflicting(const fileapi::FileSystemURL& url) = 0;

  
  
  virtual RemoteServiceState GetCurrentState() const = 0;

  
  
  virtual void GetOriginStatusMap(OriginStatusMap* status_map) = 0;

  
  virtual scoped_ptr<base::ListValue> DumpFiles(const GURL& origin) = 0;

  
  virtual scoped_ptr<base::ListValue> DumpDatabase() = 0;

  
  
  
  
  
  
  virtual void SetSyncEnabled(bool enabled) = 0;

  
  
  
  virtual SyncStatusCode SetConflictResolutionPolicy(
      ConflictResolutionPolicy policy) = 0;

  
  virtual ConflictResolutionPolicy GetConflictResolutionPolicy() const = 0;

  
  
  virtual void GetRemoteVersions(
      const fileapi::FileSystemURL& url,
      const RemoteVersionsCallback& callback) = 0;

  
  
  virtual void DownloadRemoteVersion(
      const fileapi::FileSystemURL& url,
      const std::string& id,
      const DownloadVersionCallback& callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(RemoteFileSyncService);
};

}  

#endif  
