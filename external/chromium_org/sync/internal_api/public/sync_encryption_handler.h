// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_SYNC_ENCRYPTION_HANDLER_H_
#define SYNC_INTERNAL_API_PUBLIC_SYNC_ENCRYPTION_HANDLER_H_

#include <string>

#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"

namespace sync_pb {
class EncryptedData;
}

namespace syncer {

class Cryptographer;

enum PassphraseRequiredReason {
  REASON_PASSPHRASE_NOT_REQUIRED = 0,  
  REASON_ENCRYPTION = 1,               
                                       
                                       
                                       
  REASON_DECRYPTION = 2,               
                                       
                                       
};

enum PassphraseType {
  IMPLICIT_PASSPHRASE = 0,             
  KEYSTORE_PASSPHRASE = 1,             
  FROZEN_IMPLICIT_PASSPHRASE = 2,      
  CUSTOM_PASSPHRASE = 3,               
};

enum BootstrapTokenType {
  PASSPHRASE_BOOTSTRAP_TOKEN,
  KEYSTORE_BOOTSTRAP_TOKEN
};

class SYNC_EXPORT SyncEncryptionHandler {
 public:
  
  
  class SYNC_EXPORT Observer {
   public:
    Observer();

    
    
    
    
    
    
    
    
    
    
    virtual void OnPassphraseRequired(
        PassphraseRequiredReason reason,
        const sync_pb::EncryptedData& pending_keys) = 0;
    
    

    virtual void OnPassphraseAccepted() = 0;
    
    
    
    
    
    
    
    
    virtual void OnBootstrapTokenUpdated(
        const std::string& bootstrap_token,
        BootstrapTokenType type) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnEncryptedTypesChanged(
        ModelTypeSet encrypted_types,
        bool encrypt_everything) = 0;

    
    
    virtual void OnEncryptionComplete() = 0;

    
    
    
    virtual void OnCryptographerStateChanged(Cryptographer* cryptographer) = 0;

    
    
    
    
    virtual void OnPassphraseTypeChanged(PassphraseType type,
                                         base::Time passphrase_time) = 0;

   protected:
    virtual ~Observer();
  };

  SyncEncryptionHandler();
  virtual ~SyncEncryptionHandler();

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  
  
  
  
  virtual void Init() = 0;

  
  
  
  
  
  
  
  
  virtual void SetEncryptionPassphrase(const std::string& passphrase,
                                       bool is_explicit) = 0;

  
  
  
  
  
  virtual void SetDecryptionPassphrase(const std::string& passphrase) = 0;

  
  virtual void EnableEncryptEverything() = 0;

  
  
  virtual bool EncryptEverythingEnabled() const = 0;

  
  
  virtual PassphraseType GetPassphraseType() const = 0;

  
  static ModelTypeSet SensitiveTypes();
};

}  

#endif  
