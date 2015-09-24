// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_WEBDATA_AUTOCOMPLETE_SYNCABLE_SERVICE_H_
#define CHROME_BROWSER_WEBDATA_AUTOCOMPLETE_SYNCABLE_SERVICE_H_

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/scoped_observer.h"
#include "base/supports_user_data.h"
#include "base/threading/non_thread_safe.h"
#include "components/autofill/core/browser/webdata/autofill_change.h"
#include "components/autofill/core/browser/webdata/autofill_entry.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_backend.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service_observer.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/api/syncable_service.h"

class ProfileSyncServiceAutofillTest;

namespace syncer {
class SyncErrorFactory;
}

namespace sync_pb {
class AutofillSpecifics;
}

class AutocompleteSyncableService
    : public base::SupportsUserData::Data,
      public syncer::SyncableService,
      public autofill::AutofillWebDataServiceObserverOnDBThread,
      public base::NonThreadSafe {
 public:
  virtual ~AutocompleteSyncableService();

  
  
  static void CreateForWebDataServiceAndBackend(
      autofill::AutofillWebDataService* web_data_service,
      autofill::AutofillWebDataBackend* webdata_backend);

  
  static AutocompleteSyncableService* FromWebDataService(
      autofill::AutofillWebDataService* web_data_service);

  static syncer::ModelType model_type() { return syncer::AUTOFILL; }

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  virtual void AutofillEntriesChanged(
      const autofill::AutofillChangeList& changes) OVERRIDE;

  
  
  void UpdateCullSetting(bool cull_expired_entries);
  bool cull_expired_entries() const { return cull_expired_entries_; }

  
  
  void InjectStartSyncFlare(
      const syncer::SyncableService::StartSyncFlare& flare);

 protected:
  explicit AutocompleteSyncableService(
      autofill::AutofillWebDataBackend* webdata_backend);

  
  
  virtual bool LoadAutofillData(
      std::vector<autofill::AutofillEntry>* entries) const;

  
  
  
  virtual bool SaveChangesToWebData(
      const std::vector<autofill::AutofillEntry>& entries);

 private:
  friend class ProfileSyncServiceAutofillTest;
  friend class MockAutocompleteSyncableService;
  friend class FakeServerUpdater;
  FRIEND_TEST_ALL_PREFIXES(AutocompleteSyncableServiceTest,
                           MergeDataAndStartSyncing);
  FRIEND_TEST_ALL_PREFIXES(AutocompleteSyncableServiceTest, GetAllSyncData);
  FRIEND_TEST_ALL_PREFIXES(AutocompleteSyncableServiceTest,
                           ProcessSyncChanges);
  FRIEND_TEST_ALL_PREFIXES(AutocompleteSyncableServiceTest,
                           ActOnChange);

  
  
  
  typedef std::map<autofill::AutofillKey,
                   std::pair<syncer::SyncChange::SyncChangeType,
                             std::vector<autofill::AutofillEntry>::iterator> >
      AutocompleteEntryMap;

  
  
  
  
  
  
  void CreateOrUpdateEntry(const syncer::SyncData& data,
                           AutocompleteEntryMap* loaded_data,
                           std::vector<autofill::AutofillEntry>* new_entries);

  
  static void WriteAutofillEntry(const autofill::AutofillEntry& entry,
                                 sync_pb::EntitySpecifics* autofill_specifics);

  
  syncer::SyncError AutofillEntryDelete(
      const sync_pb::AutofillSpecifics& autofill);

  syncer::SyncData CreateSyncData(const autofill::AutofillEntry& entry) const;

  
  void ActOnChanges(const autofill::AutofillChangeList& changes);

  static std::string KeyToTag(const std::string& name,
                              const std::string& value);

  
  AutocompleteSyncableService();
  void set_sync_processor(syncer::SyncChangeProcessor* sync_processor) {
    sync_processor_.reset(sync_processor);
  }

  
  
  autofill::AutofillWebDataBackend* webdata_backend_;

  ScopedObserver<autofill::AutofillWebDataBackend, AutocompleteSyncableService>
      scoped_observer_;

  
  
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;

  
  
  scoped_ptr<syncer::SyncErrorFactory> error_handler_;

  
  
  bool cull_expired_entries_;

  syncer::SyncableService::StartSyncFlare flare_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteSyncableService);
};

#endif  
