// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_SOCKET_CHROMEOS_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_SOCKET_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chromeos/chromeos_export.h"
#include "device/bluetooth/bluetooth_socket.h"

namespace dbus {

class FileDescriptor;

}  

namespace net {

class DrainableIOBuffer;
class GrowableIOBuffer;

}  

namespace chromeos {

class CHROMEOS_EXPORT BluetoothSocketChromeOS
    : public device::BluetoothSocket {
 public:
  
  virtual bool Receive(net::GrowableIOBuffer* buffer) OVERRIDE;
  virtual bool Send(net::DrainableIOBuffer* buffer) OVERRIDE;
  virtual std::string GetLastErrorMessage() const OVERRIDE;

  
  
  
  static scoped_refptr<device::BluetoothSocket> Create(
      dbus::FileDescriptor* fd);

 protected:
  virtual ~BluetoothSocketChromeOS();

 private:
  BluetoothSocketChromeOS(int fd);

  
  
  
  enum SocketType {
    L2CAP,
    RFCOMM
  };

  
  const int fd_;
  SocketType socket_type_;

  
  
  std::string error_message_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothSocketChromeOS);
};

}  

#endif  
