// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OWNER_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OWNER_MANAGER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "crypto/rsa_private_key.h"
#include "chrome/browser/chromeos/login/owner_key_utils.h"
#include "content/browser/browser_thread.h"

class FilePath;
class NotificationDetails;
class NotificationType;

namespace chromeos {

class OwnerManager : public base::RefCountedThreadSafe<OwnerManager> {
 public:
  
  enum KeyOpCode {
    SUCCESS,
    KEY_UNAVAILABLE,  
    OPERATION_FAILED  
  };

  class Delegate {
   public:
    
    
    
    
    virtual void OnKeyOpComplete(const KeyOpCode return_code,
                                 const std::vector<uint8>& payload) = 0;
  };

  class KeyUpdateDelegate {
   public:
    
    virtual void OnKeyUpdated() = 0;
  };

  OwnerManager();
  virtual ~OwnerManager();

  
  void UpdateOwnerKey(const BrowserThread::ID thread_id,
                      const std::vector<uint8>& key,
                      KeyUpdateDelegate* d);

  
  
  
  void LoadOwnerKey();

  bool EnsurePublicKey();
  bool EnsurePrivateKey();

  
  
  
  
  
  
  
  void Sign(const BrowserThread::ID thread_id,
            const std::string& data,
            Delegate* d);

  
  
  
  
  
  
  
  
  void Verify(const BrowserThread::ID thread_id,
              const std::string& data,
              const std::vector<uint8>& signature,
              Delegate* d);

 private:
  
  void SendNotification(NotificationType type,
                        const NotificationDetails& details);

  
  void CallKeyUpdateDelegate(KeyUpdateDelegate* d) {
    d->OnKeyUpdated();
  }

  
  void CallDelegate(Delegate* d,
                    const KeyOpCode return_code,
                    const std::vector<uint8>& payload) {
    d->OnKeyOpComplete(return_code, payload);
  }

  scoped_ptr<crypto::RSAPrivateKey> private_key_;
  std::vector<uint8> public_key_;

  scoped_refptr<OwnerKeyUtils> utils_;

  friend class OwnerManagerTest;

  DISALLOW_COPY_AND_ASSIGN(OwnerManager);
};

}  

#endif  
