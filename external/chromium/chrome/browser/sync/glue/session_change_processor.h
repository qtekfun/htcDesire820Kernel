// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SESSION_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_SESSION_CHANGE_PROCESSOR_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/sessions/session_backend.h"
#include "chrome/browser/sessions/session_service.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"

class NotificationDetails;
class NotificationSource;
class Profile;

namespace browser_sync {

class SessionModelAssociator;
class UnrecoverableErrorHandler;

class SessionChangeProcessor : public ChangeProcessor,
                               public NotificationObserver {
 public:
  
  SessionChangeProcessor(
      UnrecoverableErrorHandler* error_handler,
      SessionModelAssociator* session_model_associator);
  SessionChangeProcessor(
      UnrecoverableErrorHandler* error_handler,
      SessionModelAssociator* session_model_associator,
      bool setup_for_test);
  virtual ~SessionChangeProcessor();

  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  virtual void ApplyChangesFromSyncModel(
      const sync_api::BaseTransaction* trans,
      const sync_api::SyncManager::ChangeRecord* changes,
      int change_count);

 protected:
  
  virtual void StartImpl(Profile* profile);
  virtual void StopImpl();
 private:
  void StartObserving();
  void StopObserving();
  SessionModelAssociator* session_model_associator_;
  NotificationRegistrar notification_registrar_;

  
  Profile* profile_;

  
  bool setup_for_test_;

  DISALLOW_COPY_AND_ASSIGN(SessionChangeProcessor);
};

}  

#endif  
