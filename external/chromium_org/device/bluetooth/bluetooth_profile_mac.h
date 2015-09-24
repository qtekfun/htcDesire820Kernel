// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_PROFILE_MAC_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_PROFILE_MAC_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "device/bluetooth/bluetooth_profile.h"

#ifdef __OBJC__
@class IOBluetoothDevice;
#else
class IOBluetoothDevice;
#endif

namespace device {

class BluetoothProfileMac : public BluetoothProfile {
 public:
  
  virtual void Unregister() OVERRIDE;
  virtual void SetConnectionCallback(
      const ConnectionCallback& callback) OVERRIDE;

  
  
  
  bool Connect(IOBluetoothDevice* device);

 private:
  friend BluetoothProfile;

  BluetoothProfileMac(const std::string& uuid, const std::string& name);
  virtual ~BluetoothProfileMac();

  const std::string uuid_;
  const std::string name_;
  ConnectionCallback connection_callback_;
};

}  

#endif  
