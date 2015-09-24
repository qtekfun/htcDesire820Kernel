// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_PREFERENCE_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_PREFERENCE_MODEL_ASSOCIATOR_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/prefs/pref_service.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "chrome/browser/sync/unrecoverable_error_handler.h"

class ProfileSyncService;
class Value;

namespace sync_api {
class WriteNode;
class WriteTransaction;
}

namespace browser_sync {

class PreferenceChangeProcessor;

static const char kPreferencesTag[] = "google_chrome_preferences";

class PreferenceModelAssociator
    : public PerDataTypeAssociatorInterface<PrefService::Preference,
                                            std::string> {
 public:
  static syncable::ModelType model_type() { return syncable::PREFERENCES; }
  explicit PreferenceModelAssociator(ProfileSyncService* sync_service);
  virtual ~PreferenceModelAssociator();

  
  
  
  const std::set<std::string>& synced_preferences() {
    return synced_preferences_;
  }

  
  // necessary and the value is read from or written to the node as appropriate.
  bool InitPrefNodeAndAssociate(sync_api::WriteTransaction* trans,
                                const sync_api::BaseNode& root,
                                const PrefService::Preference* pref);

  
  
  
  virtual bool AssociateModels();

  
  virtual bool DisassociateModels();

  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);

  virtual void AbortAssociation() {
    
    
  }

  
  virtual bool CryptoReadyIfNecessary();

  
  virtual const PrefService::Preference* GetChromeNodeFromSyncId(int64 sync_id);

  
  virtual bool InitSyncNodeFromChromeId(const std::string& node_id,
                                        sync_api::BaseNode* sync_node);

  
  
  virtual int64 GetSyncIdFromChromeId(const std::string& node_id);

  
  virtual void Associate(const PrefService::Preference* node, int64 sync_id);

  
  virtual void Disassociate(int64 sync_id);

  
  
  virtual bool GetSyncIdForTaggedNode(const std::string& tag, int64* sync_id);

  
  
  
  
  
  
  static Value* MergePreference(const PrefService::Preference& local_pref,
                                const Value& server_value);

  
  
  static bool WritePreferenceToNode(const std::string& name,
                                    const Value& value,
                                    sync_api::WriteNode* node);

  
  
  void AfterUpdateOperations(const std::string& pref_name);

 private:
  typedef std::map<std::string, int64> PreferenceNameToSyncIdMap;
  typedef std::map<int64, std::string> SyncIdToPreferenceNameMap;

  static Value* MergeListValues(const Value& from_value, const Value& to_value);
  static Value* MergeDictionaryValues(const Value& from_value,
                                      const Value& to_value);

  ProfileSyncService* sync_service_;
  std::set<std::string> synced_preferences_;
  int64 preferences_node_id_;

  PreferenceNameToSyncIdMap id_map_;
  SyncIdToPreferenceNameMap id_map_inverse_;

  DISALLOW_COPY_AND_ASSIGN(PreferenceModelAssociator);
};

}  

#endif  
