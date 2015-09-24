// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_PEER_H_
#define DEVICE_NFC_NFC_PEER_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "device/nfc/nfc_ndef_record.h"

namespace device {

class NfcPeer {
 public:
  
  enum HandoverType {
    kHandoverTypeBluetooth,
    kHandoverTypeWiFi
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    
    virtual void RecordsReceived(NfcPeer* peer,
                                 const NfcNdefMessage& message) {}
  };

  
  typedef base::Closure ErrorCallback;

  virtual ~NfcPeer();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual std::string GetIdentifier() const = 0;

  
  
  
  
  
  virtual NfcNdefMessage GetNdefMessage() const = 0;

  
  
  
  virtual void PushNdef(NfcNdefMessage* message,
                        const base::Closure& callback,
                        const ErrorCallback& error_callback) = 0;

  
  
  
  virtual void StartHandover(HandoverType handover_type,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) = 0;

 protected:
  NfcPeer();

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcPeer);
};

}  

#endif  
