// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_AUTOFILL_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_AUTOFILL_MODEL_ASSOCIATOR_H_
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

class AutofillChangeProcessor;
class UnrecoverableErrorHandler;

extern const char kAutofillTag[];
extern const char kAutofillProfileNamespaceTag[];
extern const char kAutofillEntryNamespaceTag[];

class AutofillModelAssociator
    : public PerDataTypeAssociatorInterface<std::string, std::string> {
 public:
  static syncable::ModelType model_type() { return syncable::AUTOFILL; }
  AutofillModelAssociator(ProfileSyncService* sync_service,
                          WebDatabase* web_database,
                          PersonalDataManager* data_manager);
  virtual ~AutofillModelAssociator();

  
  
  
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

  static std::string KeyToTag(const string16& name, const string16& value);

  static bool MergeTimestamps(const sync_pb::AutofillSpecifics& autofill,
                              const std::vector<base::Time>& timestamps,
                              std::vector<base::Time>* new_timestamps);
  static bool FillProfileWithServerData(
      AutofillProfile* merge_into,
      const sync_pb::AutofillProfileSpecifics& specifics);

  
  

  
  
  
  
  virtual bool HasNotMigratedYet(const sync_api::BaseTransaction* trans);

 protected:
  
  
  AutofillProfile* FindCorrespondingNodeFromWebDB(
      const sync_pb::AutofillProfileSpecifics& profile,
      const std::vector<AutofillProfile*>& all_profiles_from_db);

 private:
  typedef std::map<std::string, int64> AutofillToSyncIdMap;
  typedef std::map<int64, std::string> SyncIdToAutofillMap;

  
  
  struct DataBundle;

  
  bool LoadAutofillData(std::vector<AutofillEntry>* entries,
                        std::vector<AutofillProfile*>* profiles);

  
  
  bool TraverseAndAssociateChromeAutofillEntries(
      sync_api::WriteTransaction* write_trans,
      const sync_api::ReadNode& autofill_root,
      const std::vector<AutofillEntry>& all_entries_from_db,
      std::set<AutofillKey>* current_entries,
      std::vector<AutofillEntry>* new_entries);
  bool TraverseAndAssociateChromeAutofillProfiles(
      sync_api::WriteTransaction* write_trans,
      const sync_api::ReadNode& autofill_root,
      const std::vector<AutofillProfile*>& all_profiles_from_db,
      std::set<string16>* current_profiles,
      std::vector<AutofillProfile*>* updated_profiles);

  
  
  bool TraverseAndAssociateAllSyncNodes(
      sync_api::WriteTransaction* write_trans,
      const sync_api::ReadNode& autofill_root,
      DataBundle* bundle,
      const std::vector<AutofillProfile*>& all_profiles_from_db);

  
  
  bool SaveChangesToWebData(const DataBundle& bundle);

  
  
  void AddNativeEntryIfNeeded(const sync_pb::AutofillSpecifics& autofill,
                              DataBundle* bundle,
                              const sync_api::ReadNode& node);

  
  
  void AddNativeProfileIfNeeded(
      const sync_pb::AutofillProfileSpecifics& profile,
      DataBundle* bundle,
      const sync_api::ReadNode& node,
      const std::vector<AutofillProfile*>& all_profiles_from_db);

  
  
  bool IsAbortPending();

  ProfileSyncService* sync_service_;
  WebDatabase* web_database_;
  PersonalDataManager* personal_data_;
  int64 autofill_node_id_;

  AutofillToSyncIdMap id_map_;
  SyncIdToAutofillMap id_map_inverse_;

  
  
  
  base::Lock abort_association_pending_lock_;
  bool abort_association_pending_;
  int number_of_entries_created_;

  DISALLOW_COPY_AND_ASSIGN(AutofillModelAssociator);
};

}  

#endif  
