// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_ADAPTER_FACTORY_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_ADAPTER_FACTORY_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "device/bluetooth/bluetooth_adapter.h"

namespace device {

class BluetoothAdapterFactory {
 public:
  typedef base::Callback<void(scoped_refptr<BluetoothAdapter> adapter)>
      AdapterCallback;

  
  
  static bool IsBluetoothAdapterAvailable();

  
  
  
  
  static void GetAdapter(const AdapterCallback& callback);

  
  
  
  static scoped_refptr<BluetoothAdapter> MaybeGetAdapter();
};

}  

#endif  
