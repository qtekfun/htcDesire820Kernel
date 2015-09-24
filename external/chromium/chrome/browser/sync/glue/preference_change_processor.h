// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_PREFERENCE_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_PREFERENCE_CHANGE_PROCESSOR_H_
#pragma once

#include <string>

#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/prefs/pref_service.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "content/common/notification_observer.h"

namespace browser_sync {

class PreferenceModelAssociator;
class UnrecoverableErrorHandler;

class PreferenceChangeProcessor : public ChangeProcessor,
                                  public NotificationObserver {
 public:
  PreferenceChangeProcessor(PreferenceModelAssociator* model_associator,
                            UnrecoverableErrorHandler* error_handler);
  virtual ~PreferenceChangeProcessor();

  
  
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
  Value* ReadPreference(sync_api::ReadNode* node, std::string* name);

  void StartObserving();
  void StopObserving();

  
  PrefService* pref_service_;

  
  PreferenceModelAssociator* model_associator_;

  
  bool processing_pref_change_;

  PrefChangeRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PreferenceChangeProcessor);
};

}  

#endif  
