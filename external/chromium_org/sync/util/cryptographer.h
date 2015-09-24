// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_UTIL_CRYPTOGRAPHER_H_
#define SYNC_UTIL_CRYPTOGRAPHER_H_

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"
#include "sync/protocol/encryption.pb.h"
#include "sync/util/nigori.h"

namespace sync_pb {
class NigoriKeyBag;
class NigoriSpecifics;
}

namespace syncer {

class Encryptor;

SYNC_EXPORT_PRIVATE extern const char kNigoriTag[];

struct KeyParams {
  std::string hostname;
  std::string username;
  std::string password;
};

class SYNC_EXPORT Cryptographer {
 public:
  
  explicit Cryptographer(Encryptor* encryptor);
  ~Cryptographer();

  
  
  
  
  
  
  
  
  
  void Bootstrap(const std::string& restored_bootstrap_token);

  
  
  bool CanDecrypt(const sync_pb::EncryptedData& encrypted) const;

  
  
  bool CanDecryptUsingDefaultKey(const sync_pb::EncryptedData& encrypted) const;

  
  
  
  
  
  
  bool Encrypt(const ::google::protobuf::MessageLite& message,
               sync_pb::EncryptedData* encrypted) const;

  
  
  
  
  
  
  bool EncryptString(const std::string& serialized,
                     sync_pb::EncryptedData* encrypted) const;

  
  
  bool Decrypt(const sync_pb::EncryptedData& encrypted,
               ::google::protobuf::MessageLite* message) const;

  
  
  std::string DecryptToString(const sync_pb::EncryptedData& encrypted) const;

  
  
  bool GetKeys(sync_pb::EncryptedData* encrypted) const;

  
  
  
  
  
  bool AddKey(const KeyParams& params);

  
  
  
  
  
  
  bool AddKeyFromBootstrapToken(const std::string restored_bootstrap_token);

  
  
  
  
  
  
  bool AddNonDefaultKey(const KeyParams& params);

  
  
  
  
  void InstallKeys(const sync_pb::EncryptedData& encrypted);

  
  
  
  void SetPendingKeys(const sync_pb::EncryptedData& encrypted);

  
  
  
  
  const sync_pb::EncryptedData& GetPendingKeys() const;

  
  
  
  
  bool DecryptPendingKeys(const KeyParams& params);

  
  
  void SetDefaultKey(const std::string& key_name);

  bool is_initialized() const {
    return !nigoris_.empty() && !default_nigori_name_.empty();
  }

  
  bool is_ready() const {
    return is_initialized() && !has_pending_keys();
  }

  
  bool has_pending_keys() const { return NULL != pending_keys_.get(); }

  
  
  
  bool GetBootstrapToken(std::string* token) const;

  Encryptor* encryptor() const { return encryptor_; }

  
  
  bool KeybagIsStale(const sync_pb::EncryptedData& keybag) const;

  
  
  std::string GetDefaultNigoriKey() const;

  
  
  bool ImportNigoriKey(const std::string serialized_nigori_key);

 private:
  typedef std::map<std::string, linked_ptr<const Nigori> > NigoriMap;

  
  
  
  void InstallKeyBag(const sync_pb::NigoriKeyBag& bag);

  
  
  bool AddKeyImpl(scoped_ptr<Nigori> nigori, bool set_as_default);

  
  std::string UnpackBootstrapToken(const std::string& token) const;

  Encryptor* const encryptor_;

  
  NigoriMap nigoris_;
  
  
  std::string default_nigori_name_;

  scoped_ptr<sync_pb::EncryptedData> pending_keys_;

  DISALLOW_COPY_AND_ASSIGN(Cryptographer);
};

}  

#endif  
