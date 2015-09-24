// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_WEBDATA_AUTOFILL_PROFILE_SYNCABLE_SERVICE_H_
#define CHROME_BROWSER_WEBDATA_AUTOFILL_PROFILE_SYNCABLE_SERVICE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "base/scoped_observer.h"
#include "base/supports_user_data.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/browser/webdata/autofill_change.h"
#include "components/autofill/core/browser/webdata/autofill_entry.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_backend.h"
#include "components/autofill/core/browser/webdata/autofill_webdata_service_observer.h"
#include "sync/api/sync_change.h"
#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/api/syncable_service.h"
#include "sync/protocol/autofill_specifics.pb.h"

class ProfileSyncServiceAutofillTest;
class WebDataServiceBase;

namespace autofill {
class AutofillProfile;
class AutofillTable;
class AutofillWebDataService;
class FormGroup;
}  

extern const char kAutofillProfileTag[];

class AutofillProfileSyncableService
    : public base::SupportsUserData::Data,
      public syncer::SyncableService,
      public autofill::AutofillWebDataServiceObserverOnDBThread,
      public base::NonThreadSafe {
 public:
  virtual ~AutofillProfileSyncableService();

  
  
  static void CreateForWebDataServiceAndBackend(
      autofill::AutofillWebDataService* web_data_service,
      autofill::AutofillWebDataBackend* webdata_backend,
      const std::string& app_locale);

  
  static AutofillProfileSyncableService* FromWebDataService(
      autofill::AutofillWebDataService* web_data_service);

  static syncer::ModelType model_type() { return syncer::AUTOFILL_PROFILE; }

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> sync_error_factory) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  virtual void AutofillProfileChanged(
      const autofill::AutofillProfileChange& change) OVERRIDE;

  
  
  void InjectStartSyncFlare(
      const syncer::SyncableService::StartSyncFlare& flare);

 protected:
  AutofillProfileSyncableService(
      autofill::AutofillWebDataBackend* webdata_backend,
      const std::string& app_locale);

  
  
  
  
  struct DataBundle;

  
  
  
  virtual bool LoadAutofillData(
      std::vector<autofill::AutofillProfile*>* profiles);

  
  
  
  virtual bool SaveChangesToWebData(const DataBundle& bundle);

  
  AutofillProfileSyncableService();
  void set_sync_processor(syncer::SyncChangeProcessor* sync_processor) {
    sync_processor_.reset(sync_processor);
  }

  
  
  static syncer::SyncData CreateData(const autofill::AutofillProfile& profile);

 private:
  friend class ProfileSyncServiceAutofillTest;
  FRIEND_TEST_ALL_PREFIXES(AutofillProfileSyncableServiceTest,
                           UpdateField);
  FRIEND_TEST_ALL_PREFIXES(AutofillProfileSyncableServiceTest,
                           UpdateMultivaluedField);
  FRIEND_TEST_ALL_PREFIXES(AutofillProfileSyncableServiceTest,
                           MergeProfile);

  
  typedef std::map<std::string, autofill::AutofillProfile*> GUIDToProfileMap;

  
  
  static bool OverwriteProfileWithServerData(
      const sync_pb::AutofillProfileSpecifics& specifics,
      autofill::AutofillProfile* profile,
      const std::string& app_locale);

  
  static void WriteAutofillProfile(const autofill::AutofillProfile& profile,
                                   sync_pb::EntitySpecifics* profile_specifics);

  
  
  void CreateGUIDToProfileMap(
      const std::vector<autofill::AutofillProfile*>& profiles,
      GUIDToProfileMap* profile_map);

  
  
  
  
  
  GUIDToProfileMap::iterator CreateOrUpdateProfile(
      const syncer::SyncData& data,
      GUIDToProfileMap* profile_map,
      DataBundle* bundle);

  
  void ActOnChange(const autofill::AutofillProfileChange& change);

  autofill::AutofillTable* GetAutofillTable() const;

  
  
  static bool UpdateField(autofill::ServerFieldType field_type,
                          const std::string& new_value,
                          autofill::AutofillProfile* autofill_profile);
  
  static bool UpdateMultivaluedField(
      autofill::ServerFieldType field_type,
      const ::google::protobuf::RepeatedPtrField<std::string>& new_value,
      autofill::AutofillProfile* autofill_profile);

  
  
  
  
  
  
  static bool MergeProfile(const autofill::AutofillProfile& merge_from,
                           autofill::AutofillProfile* merge_into,
                           const std::string& app_locale);

  autofill::AutofillWebDataBackend* webdata_backend_;  
  std::string app_locale_;
  ScopedObserver<autofill::AutofillWebDataBackend,
                 AutofillProfileSyncableService> scoped_observer_;

  
  
  ScopedVector<autofill::AutofillProfile> profiles_;
  GUIDToProfileMap profiles_map_;

  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;

  scoped_ptr<syncer::SyncErrorFactory> sync_error_factory_;

  syncer::SyncableService::StartSyncFlare flare_;

  DISALLOW_COPY_AND_ASSIGN(AutofillProfileSyncableService);
};

struct AutofillProfileSyncableService::DataBundle {
  DataBundle();
  ~DataBundle();

  std::vector<std::string> profiles_to_delete;
  std::vector<autofill::AutofillProfile*> profiles_to_update;
  std::vector<autofill::AutofillProfile*> profiles_to_add;

  
  
  GUIDToProfileMap candidates_to_merge;
  
  std::vector<autofill::AutofillProfile*> profiles_to_sync_back;
};

#endif  
