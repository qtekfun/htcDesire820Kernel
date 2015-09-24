// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_UTIL_CRYPTOGRAPHER_H_
#define CHROME_BROWSER_SYNC_UTIL_CRYPTOGRAPHER_H_
#pragma once

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/protocol/nigori_specifics.pb.h"
#include "chrome/browser/sync/util/nigori.h"

namespace browser_sync {

extern const char kNigoriTag[];

struct KeyParams {
  std::string hostname;
  std::string username;
  std::string password;
};

class Cryptographer {
 public:
  Cryptographer();
  ~Cryptographer();

  
  
  
  
  
  
  
  
  
  void Bootstrap(const std::string& restored_bootstrap_token);

  
  
  bool CanDecrypt(const sync_pb::EncryptedData& encrypted) const;

  
  
  bool CanDecryptUsingDefaultKey(const sync_pb::EncryptedData& encrypted) const;

  
  
  
  bool Encrypt(const ::google::protobuf::MessageLite& message,
               sync_pb::EncryptedData* encrypted) const;

  
  
  bool Decrypt(const sync_pb::EncryptedData& encrypted,
               ::google::protobuf::MessageLite* message) const;

  
  
  std::string DecryptToString(const sync_pb::EncryptedData& encrypted) const;

  
  
  bool GetKeys(sync_pb::EncryptedData* encrypted) const;

  
  
  
  bool AddKey(const KeyParams& params);

  
  
  
  bool SetKeys(const sync_pb::EncryptedData& encrypted);

  
  
  
  void SetPendingKeys(const sync_pb::EncryptedData& encrypted);

  
  
  
  bool DecryptPendingKeys(const KeyParams& params);

  
  bool is_ready() const { return !nigoris_.empty() && default_nigori_; }

  
  bool has_pending_keys() const { return NULL != pending_keys_.get(); }

  
  
  
  bool GetBootstrapToken(std::string* token) const;

 private:
  FRIEND_TEST_ALL_PREFIXES(CryptographerTest, PackUnpack);
  typedef std::map<std::string, linked_ptr<const Nigori> > NigoriMap;

  
  
  
  void InstallKeys(const std::string& default_key_name,
                   const sync_pb::NigoriKeyBag& bag);

  bool AddKeyImpl(Nigori* nigori);

  
  
  bool PackBootstrapToken(const Nigori* nigori, std::string* pack_into) const;
  Nigori* UnpackBootstrapToken(const std::string& token) const;

  NigoriMap nigoris_;  
  NigoriMap::value_type* default_nigori_;  

  scoped_ptr<sync_pb::EncryptedData> pending_keys_;

  DISALLOW_COPY_AND_ASSIGN(Cryptographer);
};

}  

#endif  
