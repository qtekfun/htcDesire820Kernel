// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_TAG_TECHNOLOGY_H_
#define DEVICE_NFC_NFC_TAG_TECHNOLOGY_H_

#include "base/callback.h"
#include "device/nfc/nfc_ndef_record.h"

namespace device {

class NfcTag;

class NfcTagTechnology {
 public:
  
  enum TechnologyType {
    kTechnologyTypeNfcA = 1 << 0,
    kTechnologyTypeNfcB = 1 << 1,
    kTechnologyTypeNfcF = 1 << 2,
    kTechnologyTypeNfcV = 1 << 3,
    kTechnologyTypeIsoDep = 1 << 4,
    kTechnologyTypeNdef = 1 << 5
  };
  typedef uint32 TechnologyTypeMask;

  virtual ~NfcTagTechnology();

  
  
  virtual bool IsSupportedByTag() const = 0;

  
  NfcTag* tag() const { return tag_; }

 protected:
  
  
  
  
  explicit NfcTagTechnology(NfcTag* tag);

 private:
  NfcTagTechnology();

  
  
  NfcTag* tag_;

  DISALLOW_COPY_AND_ASSIGN(NfcTagTechnology);
};

class NfcNdefTagTechnology : public NfcTagTechnology {
 public:
  
  typedef base::Closure ErrorCallback;

  virtual ~NfcNdefTagTechnology();

  
  virtual bool IsSupportedByTag() const OVERRIDE;

  
  
  
  
  
  virtual NfcNdefMessage GetNdefMessage() const = 0;

  
  
  
  
  virtual void WriteNdefMessage(const NfcNdefMessage& message,
                                const base::Closure& callback,
                                const ErrorCallback& error_callback) = 0;

  
  
  
  static NfcNdefTagTechnology* Create(NfcTag* tag);

 private:
  
  
  explicit NfcNdefTagTechnology(NfcTag* tag);

  DISALLOW_COPY_AND_ASSIGN(NfcNdefTagTechnology);
};

}  

#endif  
