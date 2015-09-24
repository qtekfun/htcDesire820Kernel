// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_AUTOFILL_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_AUTOFILL_CHANGE_PROCESSOR_H_
#pragma once

#include <vector>

#include "chrome/browser/autofill/autofill_profile.h"
#include "chrome/browser/autofill/credit_card.h"
#include "chrome/browser/autofill/personal_data_manager.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "chrome/browser/sync/protocol/autofill_specifics.pb.h"
#include "chrome/browser/webdata/web_data_service.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class AutofillCreditCardChange;
class AutofillEntry;
class AutofillProfileChange;
class PersonalDataManager;
class WebDatabase;

namespace browser_sync {

class AutofillModelAssociator;
class UnrecoverableErrorHandler;

class AutofillChangeProcessor : public ChangeProcessor,
                                public NotificationObserver {
 public:
  AutofillChangeProcessor(AutofillModelAssociator* model_associator,
                          WebDatabase* web_database,
                          PersonalDataManager* personal_data,
                          UnrecoverableErrorHandler* error_handler);
  virtual ~AutofillChangeProcessor();

  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void ApplyChangesFromSyncModel(
      const sync_api::BaseTransaction* trans,
      const sync_api::SyncManager::ChangeRecord* changes,
      int change_count);

  
  
  virtual void CommitChangesFromSyncModel();

  
  
  static void WriteAutofillEntry(const AutofillEntry& entry,
                                 sync_api::WriteNode* node);
  
  

 protected:
  virtual void StartImpl(Profile* profile);
  virtual void StopImpl();

 private:
  void StartObserving();
  void StopObserving();

  
  void RemoveSyncNode(const std::string& tag,
                      sync_api::WriteTransaction* trans);

  
  void ObserveAutofillEntriesChanged(AutofillChangeList* changes,
      sync_api::WriteTransaction* trans,
      const sync_api::ReadNode& autofill_root);
  void ObserveAutofillProfileChanged(AutofillProfileChange* change,
      sync_api::WriteTransaction* trans,
      const sync_api::ReadNode& autofill_root);

  
  
  void ApplySyncAutofillEntryChange(
      sync_api::SyncManager::ChangeRecord::Action action,
      const sync_pb::AutofillSpecifics& autofill,
      std::vector<AutofillEntry>* new_entries,
      int64 sync_id);
  void ApplySyncAutofillProfileChange(
      sync_api::SyncManager::ChangeRecord::Action action,
      const sync_pb::AutofillProfileSpecifics& profile,
      int64 sync_id);

  
  void ApplySyncAutofillEntryDelete(
      const sync_pb::AutofillSpecifics& autofill);
  void ApplySyncAutofillProfileDelete(
      int64 sync_id);

  
  
  void PostOptimisticRefreshTask();

  
  bool HasNotMigratedYet(const sync_api::BaseTransaction* trans);

  
  AutofillModelAssociator* model_associator_;

  
  
  
  WebDatabase* web_database_;

  
  
  PersonalDataManager* personal_data_;

  NotificationRegistrar notification_registrar_;

  bool observing_;

  
  
  struct AutofillChangeRecord;
  std::vector<AutofillChangeRecord> autofill_changes_;

  DISALLOW_COPY_AND_ASSIGN(AutofillChangeProcessor);
};

}  

#endif  
