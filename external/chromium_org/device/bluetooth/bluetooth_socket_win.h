// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_SOCKET_WIN_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_SOCKET_WIN_H_

#include <WinSock2.h>

#include <string>

#include "base/memory/ref_counted.h"
#include "device/bluetooth/bluetooth_socket.h"

namespace net {

class DrainableIOBuffer;
class GrowableIOBuffer;

}  

namespace device {

class BluetoothServiceRecord;

class BluetoothSocketWin : public BluetoothSocket {
 public:
  static scoped_refptr<BluetoothSocket> CreateBluetoothSocket(
      const BluetoothServiceRecord& service_record);

  
  virtual bool Receive(net::GrowableIOBuffer* buffer) OVERRIDE;
  virtual bool Send(net::DrainableIOBuffer* buffer) OVERRIDE;
  virtual std::string GetLastErrorMessage() const OVERRIDE;

 protected:
  virtual ~BluetoothSocketWin();

 private:
  explicit BluetoothSocketWin(SOCKET fd);

  const SOCKET fd_;
  std::string error_message_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothSocketWin);
};

}  

#endif  
