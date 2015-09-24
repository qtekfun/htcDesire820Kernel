// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_AUTOFILL_PROFILE_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_AUTOFILL_PROFILE_MODEL_ASSOCIATOR_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/autofill/personal_data_manager.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "chrome/browser/sync/protocol/autofill_specifics.pb.h"
#include "chrome/browser/webdata/autofill_entry.h"

class AutofillProfile;

class ProfileSyncService;
class WebDatabase;

namespace sync_api {
class WriteTransaction;
}

namespace browser_sync {

extern const char kAutofillProfileTag[];

class AutofillChangeProcessor;
class UnrecoverableErrorHandler;

class AutofillProfileModelAssociator
    : public PerDataTypeAssociatorInterface<std::string, std::string> {
 public:
  AutofillProfileModelAssociator(ProfileSyncService* sync_service,
                                 WebDatabase* web_database,
                                 PersonalDataManager* data_manager);
  virtual ~AutofillProfileModelAssociator();

  
  
  
  
  struct DataBundle;

  static syncable::ModelType model_type() { return syncable::AUTOFILL_PROFILE; }

  
  
  
  virtual bool AssociateModels();

  
  virtual bool DisassociateModels();

  
  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);

  
  virtual void AbortAssociation();

  
  virtual bool CryptoReadyIfNecessary();

  virtual const std::string* GetChromeNodeFromSyncId(int64 sync_id);

  virtual bool InitSyncNodeFromChromeId(const std::string& node_id,
                                        sync_api::BaseNode* sync_node);

  
  
  virtual int64 GetSyncIdFromChromeId(const std::string& node_id);

  
  virtual void Associate(const std::string* node, int64 sync_id);

  
  virtual void Disassociate(int64 sync_id);

  
  
  
  
  virtual bool GetSyncIdForTaggedNode(const std::string& tag, int64* sync_id);

  static bool OverwriteProfileWithServerData(
      AutofillProfile* merge_into,
      const sync_pb::AutofillProfileSpecifics& specifics);

 protected:
  AutofillProfileModelAssociator();
  bool TraverseAndAssociateChromeAutofillProfiles(
      sync_api::WriteTransaction* write_trans,
      const sync_api::ReadNode& autofill_root,
      const std::vector<AutofillProfile*>& all_profiles_from_db,
      std::set<std::string>* current_profiles,
      std::vector<AutofillProfile*>* updated_profiles,
      std::vector<AutofillProfile*>* new_profiles,
      std::vector<std::string>* profiles_to_delete);

  
  
  virtual void AddNativeProfileIfNeeded(
      const sync_pb::AutofillProfileSpecifics& profile,
      DataBundle* bundle,
      const sync_api::ReadNode& node);

  
  
  
  virtual bool MakeNewAutofillProfileSyncNodeIfNeeded(
      sync_api::WriteTransaction* trans,
      const sync_api::BaseNode& autofill_root,
      const AutofillProfile& profile,
      std::vector<AutofillProfile*>* new_profiles,
      std::set<std::string>* current_profiles,
      std::vector<std::string>* profiles_to_delete);

  
  
  bool TraverseAndAssociateAllSyncNodes(
      sync_api::WriteTransaction* write_trans,
      const sync_api::ReadNode& autofill_root,
      DataBundle* bundle);

 private:
  typedef std::map<std::string, int64> AutofillToSyncIdMap;
  typedef std::map<int64, std::string> SyncIdToAutofillMap;

  
  
  

  
  bool LoadAutofillData(std::vector<AutofillProfile*>* profiles);

  static bool MergeField(FormGroup* f,
                         AutofillFieldType t,
                         const std::string& specifics_field);

  
  
  bool SaveChangesToWebData(const DataBundle& bundle);

  
  
  bool IsAbortPending();

  int64 FindSyncNodeWithProfile(sync_api::WriteTransaction* trans,
      const sync_api::BaseNode& autofill_root,
      const AutofillProfile& profile,
      std::set<std::string>* current_profiles);

  ProfileSyncService* sync_service_;
  WebDatabase* web_database_;
  PersonalDataManager* personal_data_;
  int64 autofill_node_id_;

  AutofillToSyncIdMap id_map_;
  SyncIdToAutofillMap id_map_inverse_;

  
  
  
  base::Lock abort_association_pending_lock_;
  bool abort_association_pending_;

  int number_of_profiles_created_;

  DISALLOW_COPY_AND_ASSIGN(AutofillProfileModelAssociator);
};

struct AutofillProfileModelAssociator::DataBundle {
  DataBundle();
  ~DataBundle();

  std::set<std::string> current_profiles;
  std::vector<std::string> profiles_to_delete;
  std::vector<AutofillProfile*> updated_profiles;
  std::vector<AutofillProfile*> new_profiles;  
};

}  

#endif  

