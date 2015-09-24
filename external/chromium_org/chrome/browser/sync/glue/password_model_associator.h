// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_PASSWORD_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_PASSWORD_MODEL_ASSOCIATOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "sync/protocol/password_specifics.pb.h"

class PasswordStore;
class ProfileSyncService;

namespace autofill {
struct PasswordForm;
}

namespace base {
class MessageLoop;
}

namespace syncer {
class WriteNode;
class WriteTransaction;
}

namespace browser_sync {

extern const char kPasswordTag[];

class PasswordModelAssociator
  : public PerDataTypeAssociatorInterface<std::string, std::string> {
 public:
  typedef std::vector<autofill::PasswordForm> PasswordVector;

  static syncer::ModelType model_type() { return syncer::PASSWORDS; }
  PasswordModelAssociator(ProfileSyncService* sync_service,
                          PasswordStore* password_store,
                          DataTypeErrorHandler* error_handler);
  virtual ~PasswordModelAssociator();

  
  
  
  virtual syncer::SyncError AssociateModels(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result) OVERRIDE;

  
  bool DeleteAllNodes(syncer::WriteTransaction* trans);

  
  virtual syncer::SyncError DisassociateModels() OVERRIDE;

  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes) OVERRIDE;

  
  virtual void AbortAssociation() OVERRIDE;

  
  virtual bool CryptoReadyIfNecessary() OVERRIDE;

  
  virtual const std::string* GetChromeNodeFromSyncId(int64 sync_id) OVERRIDE;

  
  virtual bool InitSyncNodeFromChromeId(const std::string& node_id,
                                        syncer::BaseNode* sync_node) OVERRIDE;

  
  
  virtual int64 GetSyncIdFromChromeId(const std::string& node_id) OVERRIDE;

  
  virtual void Associate(const std::string* node, int64 sync_id) OVERRIDE;

  
  virtual void Disassociate(int64 sync_id) OVERRIDE;

  
  
  virtual bool GetSyncIdForTaggedNode(const std::string& tag, int64* sync_id);

  syncer::SyncError WriteToPasswordStore(const PasswordVector* new_passwords,
                                 const PasswordVector* updated_passwords,
                                 const PasswordVector* deleted_passwords);

  static std::string MakeTag(const autofill::PasswordForm& password);
  static std::string MakeTag(const sync_pb::PasswordSpecificsData& password);
  static std::string MakeTag(const std::string& origin_url,
                             const std::string& username_element,
                             const std::string& username_value,
                             const std::string& password_element,
                             const std::string& signon_realm);

  static void CopyPassword(const sync_pb::PasswordSpecificsData& password,
                           autofill::PasswordForm* new_password);

  static bool MergePasswords(const sync_pb::PasswordSpecificsData& password,
                             const autofill::PasswordForm& password_form,
                             autofill::PasswordForm* new_password);
  static void WriteToSyncNode(const autofill::PasswordForm& password_form,
                              syncer::WriteNode* node);

 private:
  typedef std::map<std::string, int64> PasswordToSyncIdMap;
  typedef std::map<int64, std::string> SyncIdToPasswordMap;

  ProfileSyncService* sync_service_;
  scoped_refptr<PasswordStore> password_store_;
  int64 password_node_id_;

  
  bool abort_association_requested_;
  base::Lock association_lock_;

  base::MessageLoop* expected_loop_;

  PasswordToSyncIdMap id_map_;
  SyncIdToPasswordMap id_map_inverse_;
  DataTypeErrorHandler* error_handler_;

  DISALLOW_COPY_AND_ASSIGN(PasswordModelAssociator);
};

}  

#endif  
