// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_TAG_H_
#define DEVICE_NFC_NFC_TAG_H_

#include "device/nfc/nfc_tag_technology.h"

namespace device {

class NfcTag {
 public:
  
  enum TagType {
    kTagType1,
    kTagType2,
    kTagType3,
    kTagType4
  };

  
  
  enum Protocol {
    kProtocolFelica,
    kProtocolIsoDep,
    kProtocolJewel,
    kProtocolMifare,
    kProtocolNfcDep
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    
    
    virtual void RecordsReceived(NfcTag* tag, const NfcNdefMessage& message) {}
  };

  virtual ~NfcTag();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual std::string GetIdentifier() const = 0;

  
  virtual TagType GetType() const = 0;

  // Returns true, if this tag is read-only and cannot be written to.
  virtual bool IsReadOnly() const = 0;

  
  virtual Protocol GetSupportedProtocol() const = 0;

  
  virtual NfcTagTechnology::TechnologyTypeMask
      GetSupportedTechnologies() const = 0;

 protected:
  NfcTag();

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcTag);
};

}  

#endif  
