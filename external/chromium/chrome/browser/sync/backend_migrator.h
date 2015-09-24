// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_BACKEND_MIGRATOR_H_
#define CHROME_BROWSER_SYNC_BACKEND_MIGRATOR_H_

#include "base/task.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ProfileSyncService;

namespace browser_sync {

class DataTypeManager;

class BackendMigrator : public NotificationObserver,
                        public ProfileSyncServiceObserver {
 public:
  enum State {
    IDLE,
    WAITING_TO_START,   
    DISABLING_TYPES,    
                        
    WAITING_FOR_PURGE,  
                        
    REENABLING_TYPES,   
                        
  };

  BackendMigrator(ProfileSyncService* service, DataTypeManager* manager);
  virtual ~BackendMigrator();

  
  void MigrateTypes(const syncable::ModelTypeSet& types);

  
  virtual void OnStateChanged();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  State state() const;

 private:
  bool HasStartedMigrating() const;

  State state_;
  ProfileSyncService* service_;
  DataTypeManager* manager_;
  NotificationRegistrar registrar_;

  syncable::ModelTypeSet to_migrate_;
  bool restart_migration_;

  
  ScopedRunnableMethodFactory<BackendMigrator> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(BackendMigrator);
};

}  

#endif  
