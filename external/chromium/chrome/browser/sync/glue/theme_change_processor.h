// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_THEME_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_THEME_CHANGE_PROCESSOR_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"

class NotificationDetails;
class NotificationSource;
class Profile;

namespace browser_sync {

class UnrecoverableErrorHandler;

class ThemeChangeProcessor : public ChangeProcessor,
                             public NotificationObserver {
 public:
  explicit ThemeChangeProcessor(UnrecoverableErrorHandler* error_handler);
  virtual ~ThemeChangeProcessor();

  
  
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

  NotificationRegistrar notification_registrar_;
  
  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ThemeChangeProcessor);
};

}  

#endif  
