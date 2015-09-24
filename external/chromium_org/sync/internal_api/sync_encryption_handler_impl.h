// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_SYNC_ENCRYPTION_HANDLER_IMPL_H_
#define SYNC_INTERNAL_API_SYNC_ENCRYPTION_HANDLER_IMPL_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/sync_encryption_handler.h"
#include "sync/syncable/nigori_handler.h"
#include "sync/util/cryptographer.h"

namespace syncer {

class Encryptor;
struct UserShare;
class WriteNode;
class WriteTransaction;

// any data read/written is properly decrypted/encrypted.
class SYNC_EXPORT_PRIVATE SyncEncryptionHandlerImpl
    : public SyncEncryptionHandler,
      public syncable::NigoriHandler {
 public:
  SyncEncryptionHandlerImpl(
      UserShare* user_share,
      Encryptor* encryptor,
      const std::string& restored_key_for_bootstrapping,
      const std::string& restored_keystore_key_for_bootstrapping);
  virtual ~SyncEncryptionHandlerImpl();

  
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual void Init() OVERRIDE;
  virtual void SetEncryptionPassphrase(const std::string& passphrase,
                                       bool is_explicit) OVERRIDE;
  virtual void SetDecryptionPassphrase(const std::string& passphrase) OVERRIDE;
  virtual void EnableEncryptEverything() OVERRIDE;
  virtual bool EncryptEverythingEnabled() const OVERRIDE;
  virtual PassphraseType GetPassphraseType() const OVERRIDE;

  
  
  virtual void ApplyNigoriUpdate(
      const sync_pb::NigoriSpecifics& nigori,
      syncable::BaseTransaction* const trans) OVERRIDE;
  virtual void UpdateNigoriFromEncryptedTypes(
      sync_pb::NigoriSpecifics* nigori,
      syncable::BaseTransaction* const trans) const OVERRIDE;
  virtual bool NeedKeystoreKey(
      syncable::BaseTransaction* const trans) const OVERRIDE;
  virtual bool SetKeystoreKeys(
      const google::protobuf::RepeatedPtrField<google::protobuf::string>& keys,
      syncable::BaseTransaction* const trans) OVERRIDE;
  
  virtual ModelTypeSet GetEncryptedTypes(
      syncable::BaseTransaction* const trans) const OVERRIDE;

  
  
  Cryptographer* GetCryptographerUnsafe();
  ModelTypeSet GetEncryptedTypesUnsafe();

  bool MigratedToKeystore();
  base::Time migration_time() const;
  base::Time custom_passphrase_time() const;

 private:
  friend class SyncEncryptionHandlerImplTest;
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           NigoriEncryptionTypes);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           EncryptEverythingExplicit);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           EncryptEverythingImplicit);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           UnknownSensitiveTypes);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           GetKeystoreDecryptor);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           ReceiveMigratedNigoriKeystorePass);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           ReceiveUmigratedNigoriAfterMigration);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           ReceiveOldMigratedNigori);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           SetKeystoreAfterReceivingMigratedNigori);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           SetCustomPassAfterMigration);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           SetCustomPassAfterMigrationNoKeystoreKey);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           SetImplicitPassAfterMigrationNoKeystoreKey);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           MigrateOnEncryptEverythingKeystorePassphrase);
  FRIEND_TEST_ALL_PREFIXES(SyncEncryptionHandlerImplTest,
                           ReceiveMigratedNigoriWithOldPassphrase);

  
  
  
  
  struct Vault {
    Vault(Encryptor* encryptor, ModelTypeSet encrypted_types);
    ~Vault();

    
    Cryptographer cryptographer;
    
    ModelTypeSet encrypted_types;

   private:
    DISALLOW_COPY_AND_ASSIGN(Vault);
  };

  
  void ReEncryptEverything(WriteTransaction* trans);

  
  
  
  
  bool ApplyNigoriUpdateImpl(const sync_pb::NigoriSpecifics& nigori,
                             syncable::BaseTransaction* const trans);

  
  
  void RewriteNigori();

  
  
  
  void WriteEncryptionStateToNigori(WriteTransaction* trans);

  
  
  
  
  
  
  bool UpdateEncryptedTypesFromNigori(
      const sync_pb::NigoriSpecifics& nigori,
      syncable::BaseTransaction* const trans);

  
  
  
  
  
  
  
  
  
  void SetCustomPassphrase(const std::string& passphrase,
                           WriteTransaction* trans,
                           WriteNode* nigori_node);
  
  
  
  
  void DecryptPendingKeysWithExplicitPassphrase(const std::string& passphrase,
                                                WriteTransaction* trans,
                                                WriteNode* nigori_node);

  
  
  
  
  
  
  
  
  
  
  
  void FinishSetPassphrase(bool success,
                           const std::string& bootstrap_token,
                           WriteTransaction* trans,
                           WriteNode* nigori_node);

  
  
  
  void MergeEncryptedTypes(ModelTypeSet new_encrypted_types,
                           syncable::BaseTransaction* const trans);

  
  
  Vault* UnlockVaultMutable(syncable::BaseTransaction* const trans);
  const Vault& UnlockVault(syncable::BaseTransaction* const trans) const;

  
  
  
  
  
  
  
  
  bool ShouldTriggerMigration(const sync_pb::NigoriSpecifics& nigori,
                              const Cryptographer& cryptographer) const;

  
  
  bool AttemptToMigrateNigoriToKeystore(WriteTransaction* trans,
                                        WriteNode* nigori_node);

  
  
  
  
  bool GetKeystoreDecryptor(
      const Cryptographer& cryptographer,
      const std::string& keystore_key,
      sync_pb::EncryptedData* encrypted_blob);

  
  
  
  
  bool AttemptToInstallKeybag(const sync_pb::EncryptedData& keybag,
                              bool update_default,
                              Cryptographer* cryptographer);

  
  
  
  bool DecryptPendingKeysWithKeystoreKey(
      const std::string& keystore_key,
      const sync_pb::EncryptedData& keystore_bootstrap,
      Cryptographer* cryptographer);

  
  
  void EnableEncryptEverythingImpl(syncable::BaseTransaction* const trans);

  
  
  base::Time GetExplicitPassphraseTime() const;

  base::ThreadChecker thread_checker_;

  ObserverList<SyncEncryptionHandler::Observer> observers_;

  
  UserShare* user_share_;

  
  
  
  Vault vault_unsafe_;

  
  
  
  bool encrypt_everything_;
  
  
  PassphraseType passphrase_type_;

  
  std::string keystore_key_;

  
  
  
  
  std::vector<std::string> old_keystore_keys_;

  
  
  
  int nigori_overwrite_count_;

  
  base::Time migration_time_;

  
  
  
  base::Time custom_passphrase_time_;

  base::WeakPtrFactory<SyncEncryptionHandlerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncEncryptionHandlerImpl);
};

}  

#endif  
