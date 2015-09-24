// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_ADAPTER_FACTORY_H_
#define DEVICE_NFC_NFC_ADAPTER_FACTORY_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "device/nfc/nfc_adapter.h"

namespace device {

class NfcAdapterFactory {
 public:
  typedef base::Callback<void(scoped_refptr<NfcAdapter>)> AdapterCallback;

  
  static bool IsNfcAvailable();

  
  
  
  
  static void GetAdapter(const AdapterCallback& callback);
};

}  

#endif  
