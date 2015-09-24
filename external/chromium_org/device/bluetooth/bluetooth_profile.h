// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_PROFILE_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_PROFILE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"

namespace device {

class BluetoothDevice;
class BluetoothProfileMac;
class BluetoothSocket;
class MockBluetoothProfile;

class BluetoothProfile {
 public:
  
  struct Options {
    Options();
    ~Options();

    
    
    std::string name;

    
    
    uint16 channel;

    
    
    uint16 psm;

    
    
    bool require_authentication;

    
    
    bool require_authorization;

    
    
    
    bool auto_connect;

    
    
    uint16 version;

    
    
    uint16 features;
  };

  
  
  
  
  
  
  
  typedef base::Callback<void(BluetoothProfile*)> ProfileCallback;
  static void Register(const std::string& uuid,
                       const Options& options,
                       const ProfileCallback& callback);

  
  
  virtual void Unregister() = 0;

  
  
  
  
  
  
  
  
  typedef base::Callback<void(
      const BluetoothDevice*,
      scoped_refptr<BluetoothSocket>)> ConnectionCallback;
  virtual void SetConnectionCallback(const ConnectionCallback& callback) = 0;

 protected:
  BluetoothProfile();
  virtual ~BluetoothProfile();
};

}  

#endif  
