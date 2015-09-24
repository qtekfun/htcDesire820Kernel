// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_COMPONENTS_FACTORY_H__
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_COMPONENTS_FACTORY_H__

#include <string>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "sync/api/sync_merge_result.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"
#include "sync/internal_api/public/util/weak_handle.h"

class PasswordStore;
class Profile;
class ProfileSyncService;
class WebDataService;

namespace browser_sync {
class AssociatorInterface;
class ChangeProcessor;
class DataTypeEncryptionHandler;
class DataTypeManager;
class DataTypeManagerObserver;
class FailedDataTypesHandler;
class GenericChangeProcessor;
class SharedChangeProcessor;
class SyncBackendHost;
class SyncPrefs;
class DataTypeErrorHandler;
}  

namespace syncer {
class DataTypeDebugInfoListener;
class SyncableService;
}

namespace history {
class HistoryBackend;
}

class ProfileSyncComponentsFactory {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct SyncComponents {
    browser_sync::AssociatorInterface* model_associator;
    browser_sync::ChangeProcessor* change_processor;
    SyncComponents(browser_sync::AssociatorInterface* ma,
                   browser_sync::ChangeProcessor* cp)
        : model_associator(ma), change_processor(cp) {}
  };

  virtual ~ProfileSyncComponentsFactory() {}

  
  
  virtual void RegisterDataTypes(ProfileSyncService* pss) = 0;

  
  
  
  virtual browser_sync::DataTypeManager* CreateDataTypeManager(
      const syncer::WeakHandle<syncer::DataTypeDebugInfoListener>&
          debug_info_listener,
      const browser_sync::DataTypeController::TypeMap* controllers,
      const browser_sync::DataTypeEncryptionHandler* encryption_handler,
      browser_sync::SyncBackendHost* backend,
      browser_sync::DataTypeManagerObserver* observer,
      browser_sync::FailedDataTypesHandler* failed_data_types_handler) = 0;

  
  virtual browser_sync::SyncBackendHost* CreateSyncBackendHost(
      const std::string& name,
      Profile* profile,
      const base::WeakPtr<browser_sync::SyncPrefs>& sync_prefs) = 0;

  
  virtual browser_sync::GenericChangeProcessor* CreateGenericChangeProcessor(
      ProfileSyncService* profile_sync_service,
      browser_sync::DataTypeErrorHandler* error_handler,
      const base::WeakPtr<syncer::SyncableService>& local_service,
      const base::WeakPtr<syncer::SyncMergeResult>& merge_result) = 0;

  virtual browser_sync::SharedChangeProcessor*
      CreateSharedChangeProcessor() = 0;

  
  
  
  virtual base::WeakPtr<syncer::SyncableService> GetSyncableServiceForType(
      syncer::ModelType type) = 0;

  
  virtual SyncComponents CreateBookmarkSyncComponents(
      ProfileSyncService* profile_sync_service,
      browser_sync::DataTypeErrorHandler* error_handler) = 0;
  virtual SyncComponents CreatePasswordSyncComponents(
      ProfileSyncService* profile_sync_service,
      PasswordStore* password_store,
      browser_sync::DataTypeErrorHandler* error_handler) = 0;
  virtual SyncComponents CreateTypedUrlSyncComponents(
      ProfileSyncService* profile_sync_service,
      history::HistoryBackend* history_backend,
      browser_sync::DataTypeErrorHandler* error_handler) = 0;
  virtual SyncComponents CreateSessionSyncComponents(
      ProfileSyncService* profile_sync_service,
      browser_sync::DataTypeErrorHandler* error_handler) = 0;
};

#endif  
