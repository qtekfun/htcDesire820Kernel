// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_SOCKET_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_SOCKET_H_

#include <string>

#include "base/memory/ref_counted.h"

namespace net {

class DrainableIOBuffer;
class GrowableIOBuffer;

}  

namespace device {

class BluetoothSocket : public base::RefCounted<BluetoothSocket> {
 public:
  
  
  
  virtual bool Receive(net::GrowableIOBuffer* buffer) = 0;

  
  
  
  virtual bool Send(net::DrainableIOBuffer* buffer) = 0;

  virtual std::string GetLastErrorMessage() const = 0;

 protected:
  friend class base::RefCounted<BluetoothSocket>;
  virtual ~BluetoothSocket() {}
};

}  

#endif  
