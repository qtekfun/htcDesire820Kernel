// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_SERVICE_RECORD_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_SERVICE_RECORD_H_

#include <string>

#include "base/basictypes.h"

namespace device {

class BluetoothServiceRecord {
 public:
  virtual ~BluetoothServiceRecord();

  
  const std::string& name() const { return name_; }

  
  const std::string& address() const { return address_; }

  
  
  const std::string& uuid() const { return uuid_; }

  
  bool SupportsHid() const { return supports_hid_; }

  
  
  bool hid_reconnect_initiate() const { return hid_reconnect_initiate_; }

  
  
  bool hid_normally_connectable() const { return hid_normally_connectable_; }

  
  bool SupportsRfcomm() const { return supports_rfcomm_; }

  
  
  uint8 rfcomm_channel() const { return rfcomm_channel_; }

 protected:
  BluetoothServiceRecord();

  std::string address_;
  std::string name_;
  std::string uuid_;

  bool supports_hid_;
  bool hid_reconnect_initiate_;
  bool hid_normally_connectable_;

  bool supports_rfcomm_;
  uint8 rfcomm_channel_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothServiceRecord);
};

}  

#endif  
