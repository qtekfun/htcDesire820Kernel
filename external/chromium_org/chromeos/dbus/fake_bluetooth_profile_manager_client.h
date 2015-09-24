// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_BLUETOOTH_PROFILE_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_BLUETOOTH_PROFILE_MANAGER_CLIENT_H_

#include <map>
#include <string>

#include "base/bind.h"
#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/bluetooth_profile_manager_client.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

namespace chromeos {

class FakeBluetoothProfileServiceProvider;

class CHROMEOS_EXPORT FakeBluetoothProfileManagerClient
    : public BluetoothProfileManagerClient {
 public:
  FakeBluetoothProfileManagerClient();
  virtual ~FakeBluetoothProfileManagerClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void RegisterProfile(const dbus::ObjectPath& profile_path,
                               const std::string& uuid,
                               const Options& options,
                               const base::Closure& callback,
                               const ErrorCallback& error_callback) OVERRIDE;
  virtual void UnregisterProfile(const dbus::ObjectPath& profile_path,
                                 const base::Closure& callback,
                                 const ErrorCallback& error_callback) OVERRIDE;

  
  void RegisterProfileServiceProvider(
      FakeBluetoothProfileServiceProvider* service_provider);
  void UnregisterProfileServiceProvider(
      FakeBluetoothProfileServiceProvider* service_provider);
  FakeBluetoothProfileServiceProvider* GetProfileServiceProvider(
      const std::string& uuid);

  
  static const char kL2capUuid[];
  static const char kRfcommUuid[];

 private:
  
  
  
  
  typedef std::map<dbus::ObjectPath, FakeBluetoothProfileServiceProvider*>
      ServiceProviderMap;
  ServiceProviderMap service_provider_map_;

  
  
  
  typedef std::map<std::string, dbus::ObjectPath> ProfileMap;
  ProfileMap profile_map_;
};

}  

#endif  
