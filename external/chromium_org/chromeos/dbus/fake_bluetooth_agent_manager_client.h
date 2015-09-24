// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_BLUETOOTH_AGENT_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_BLUETOOTH_AGENT_MANAGER_CLIENT_H_

#include "base/bind.h"
#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/bluetooth_agent_manager_client.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

namespace chromeos {

class FakeBluetoothAgentServiceProvider;

class CHROMEOS_EXPORT FakeBluetoothAgentManagerClient
    : public BluetoothAgentManagerClient {
 public:
  FakeBluetoothAgentManagerClient();
  virtual ~FakeBluetoothAgentManagerClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void RegisterAgent(const dbus::ObjectPath& agent_path,
                             const std::string& capability,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) OVERRIDE;
  virtual void UnregisterAgent(const dbus::ObjectPath& agent_path,
                               const base::Closure& callback,
                               const ErrorCallback& error_callback) OVERRIDE;
  virtual void RequestDefaultAgent(const dbus::ObjectPath& agent_path,
                                   const base::Closure& callback,
                                   const ErrorCallback& error_callback)
      OVERRIDE;

  
  void RegisterAgentServiceProvider(
      FakeBluetoothAgentServiceProvider* service_provider);
  void UnregisterAgentServiceProvider(
      FakeBluetoothAgentServiceProvider* service_provider);
  FakeBluetoothAgentServiceProvider* GetAgentServiceProvider();

 private:
  
  
  FakeBluetoothAgentServiceProvider* service_provider_;
};

}  

#endif  
