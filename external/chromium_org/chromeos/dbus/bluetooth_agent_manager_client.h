// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLUETOOTH_AGENT_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_BLUETOOTH_AGENT_MANAGER_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/values.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "dbus/object_path.h"

namespace chromeos {

class CHROMEOS_EXPORT BluetoothAgentManagerClient : public DBusClient {
 public:
  virtual ~BluetoothAgentManagerClient();

  
  
  
  typedef base::Callback<void(const std::string& error_name,
                              const std::string& error_message)> ErrorCallback;

  
  
  
  
  
  virtual void RegisterAgent(const dbus::ObjectPath& agent_path,
                             const std::string& capability,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) = 0;

  
  
  virtual void UnregisterAgent(const dbus::ObjectPath& agent_path,
                               const base::Closure& callback,
                               const ErrorCallback& error_callback) = 0;

  
  
  virtual void RequestDefaultAgent(const dbus::ObjectPath& agent_path,
                                   const base::Closure& callback,
                                   const ErrorCallback& error_callback) = 0;

  
  static BluetoothAgentManagerClient* Create();

  
  static const char kNoResponseError[];

 protected:
  BluetoothAgentManagerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothAgentManagerClient);
};

}  

#endif  
