// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_PASSWORD_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_PASSWORD_CHANGE_PROCESSOR_H_
#pragma once

#include "chrome/browser/sync/glue/change_processor.h"

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/glue/password_model_associator.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"

class PasswordStore;
class MessageLoop;
class NotificationService;

namespace browser_sync {

class UnrecoverableErrorHandler;

class PasswordChangeProcessor : public ChangeProcessor,
                                public NotificationObserver {
 public:
  PasswordChangeProcessor(PasswordModelAssociator* model_associator,
                          PasswordStore* password_store,
                          UnrecoverableErrorHandler* error_handler);
  virtual ~PasswordChangeProcessor();

  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual void ApplyChangesFromSyncModel(
      const sync_api::BaseTransaction* trans,
      const sync_api::SyncManager::ChangeRecord* changes,
      int change_count) OVERRIDE;

  
  
  
  
  virtual void CommitChangesFromSyncModel() OVERRIDE;

 protected:
  virtual void StartImpl(Profile* profile) OVERRIDE;
  virtual void StopImpl() OVERRIDE;

 private:
  void StartObserving();
  void StopObserving();

  
  PasswordModelAssociator* model_associator_;

  
  
  
  PasswordStore* password_store_;

  
  
  PasswordModelAssociator::PasswordVector new_passwords_;
  PasswordModelAssociator::PasswordVector updated_passwords_;
  PasswordModelAssociator::PasswordVector deleted_passwords_;

  NotificationRegistrar notification_registrar_;

  bool observing_;

  MessageLoop* expected_loop_;

  DISALLOW_COPY_AND_ASSIGN(PasswordChangeProcessor);
};

}  

#endif  
