// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_PERMISSION_BROKER_CLIENT_H_
#define CHROMEOS_DBUS_PERMISSION_BROKER_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

namespace chromeos {

class CHROMEOS_EXPORT PermissionBrokerClient : public DBusClient {
 public:
  
  
  
  typedef base::Callback<void(bool)> ResultCallback;

  virtual ~PermissionBrokerClient();

  static PermissionBrokerClient* Create();

  
  
  
  
  
  
  virtual void RequestPathAccess(const std::string& path,
                                 int interface_id,
                                 const ResultCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void RequestUsbAccess(uint16_t vendor_id,
                                uint16_t product_id,
                                int interface_id,
                                const ResultCallback& callback) = 0;

 protected:
  PermissionBrokerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(PermissionBrokerClient);
};

}  

#endif  
