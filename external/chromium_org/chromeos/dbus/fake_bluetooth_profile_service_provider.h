// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_BLUETOOTH_PROFILE_SERVICE_PROVIDER_H_
#define CHROMEOS_DBUS_FAKE_BLUETOOTH_PROFILE_SERVICE_PROVIDER_H_

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/bluetooth_profile_service_provider.h"
#include "dbus/file_descriptor.h"
#include "dbus/object_path.h"

namespace chromeos {

class CHROMEOS_EXPORT FakeBluetoothProfileServiceProvider
    : public BluetoothProfileServiceProvider {
 public:
  FakeBluetoothProfileServiceProvider(const dbus::ObjectPath& object_path,
                                      Delegate *delegate);
  virtual ~FakeBluetoothProfileServiceProvider();

  
  
  
  virtual void Release();
  virtual void NewConnection(
      const dbus::ObjectPath& device_path,
      scoped_ptr<dbus::FileDescriptor> fd,
      const Delegate::Options& options,
      const Delegate::ConfirmationCallback& callback);
  virtual void RequestDisconnection(
      const dbus::ObjectPath& device_path,
      const Delegate::ConfirmationCallback& callback);
  virtual void Cancel();

 private:
  friend class FakeBluetoothProfileManagerClient;

  
  dbus::ObjectPath object_path_;

  
  
  
  Delegate* delegate_;
};

}  

#endif  
