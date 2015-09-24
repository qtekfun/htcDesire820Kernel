// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_BACKEND_MIGRATOR_H_
#define CHROME_BROWSER_SYNC_BACKEND_MIGRATOR_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/sync/glue/data_type_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "sync/internal_api/public/base/model_type.h"

class ProfileSyncService;

namespace syncer {
struct UserShare;
}  

namespace browser_sync {

class MigrationObserver {
 public:
  virtual void OnMigrationStateChange() = 0;

 protected:
  virtual ~MigrationObserver();
};

class BackendMigrator {
 public:
  enum State {
    IDLE,
    WAITING_TO_START,   
    DISABLING_TYPES,    
                        
                        
                        
    REENABLING_TYPES,   
                        
  };

  
  BackendMigrator(const std::string& name,
                  syncer::UserShare* user_share,
                  ProfileSyncService* service,
                  DataTypeManager* manager,
                  const base::Closure &migration_done_callback);
  virtual ~BackendMigrator();

  
  void MigrateTypes(syncer::ModelTypeSet types);

  void AddMigrationObserver(MigrationObserver* observer);
  bool HasMigrationObserver(MigrationObserver* observer) const;
  void RemoveMigrationObserver(MigrationObserver* observer);

  State state() const;

  
  
  void OnConfigureDone(const DataTypeManager::ConfigureResult& result);

  
  syncer::ModelTypeSet GetPendingMigrationTypesForTest() const;

 private:
  void ChangeState(State new_state);

  
  
  
  
  bool TryStart();

  
  void RestartMigration();

  
  void OnConfigureDoneImpl(const DataTypeManager::ConfigureResult& result);

  const std::string name_;
  syncer::UserShare* user_share_;
  ProfileSyncService* service_;
  DataTypeManager* manager_;

  State state_;

  ObserverList<MigrationObserver> migration_observers_;

  syncer::ModelTypeSet to_migrate_;

  base::Closure migration_done_callback_;

  base::WeakPtrFactory<BackendMigrator> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BackendMigrator);
};

}  

#endif  
