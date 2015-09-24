// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_TEST_FAKE_SYNC_ENCRYPTION_HANDLER_H_
#define SYNC_SYNCABLE_TEST_FAKE_SYNC_ENCRYPTION_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "sync/internal_api/public/sync_encryption_handler.h"
#include "sync/syncable/nigori_handler.h"
#include "sync/test/fake_encryptor.h"
#include "sync/util/cryptographer.h"

namespace syncer {

class FakeSyncEncryptionHandler : public SyncEncryptionHandler,
                                  public syncable::NigoriHandler {
 public:
  FakeSyncEncryptionHandler();
  virtual ~FakeSyncEncryptionHandler();

  
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

  Cryptographer* cryptographer() { return &cryptographer_; }

 private:
  ObserverList<SyncEncryptionHandler::Observer> observers_;
  ModelTypeSet encrypted_types_;
  bool encrypt_everything_;
  PassphraseType passphrase_type_;

  FakeEncryptor encryptor_;
  Cryptographer cryptographer_;
  std::string keystore_key_;
};

}  

#endif  
