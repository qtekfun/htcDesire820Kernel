// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLUETOOTH_AGENT_SERVICE_PROVIDER_H_
#define CHROMEOS_DBUS_BLUETOOTH_AGENT_SERVICE_PROVIDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "dbus/bus.h"
#include "dbus/object_path.h"

namespace chromeos {

class CHROMEOS_EXPORT BluetoothAgentServiceProvider {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    
    enum Status {
      SUCCESS,
      REJECTED,
      CANCELLED
    };

    
    
    
    typedef base::Callback<void(Status, const std::string&)> PinCodeCallback;

    
    
    
    
    typedef base::Callback<void(Status, uint32)> PasskeyCallback;

    
    
    
    typedef base::Callback<void(Status)> ConfirmationCallback;

    
    
    
    virtual void Release() = 0;

    
    
    
    
    
    
    
    virtual void RequestPinCode(const dbus::ObjectPath& device_path,
                                const PinCodeCallback& callback) = 0;

    
    
    
    
    
    
    
    
    
    virtual void DisplayPinCode(const dbus::ObjectPath& device_path,
                                const std::string& pincode) = 0;

    
    
    
    
    
    
    
    
    
    virtual void RequestPasskey(const dbus::ObjectPath& device_path,
                                const PasskeyCallback& callback) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void DisplayPasskey(const dbus::ObjectPath& device_path,
                                uint32 passkey, uint16 entered) = 0;

    
    
    
    
    
    
    
    
    
    
    
    virtual void RequestConfirmation(const dbus::ObjectPath& device_path,
                                     uint32 passkey,
                                     const ConfirmationCallback& callback) = 0;

    
    
    
    
    
    
    
    virtual void RequestAuthorization(const dbus::ObjectPath& device_path,
                                      const ConfirmationCallback& callback) = 0;

    
    
    
    
    
    virtual void AuthorizeService(const dbus::ObjectPath& device_path,
                                  const std::string& uuid,
                                  const ConfirmationCallback& callback) = 0;

    
    
    virtual void Cancel() = 0;
  };

  virtual ~BluetoothAgentServiceProvider();

  
  
  
  
  static BluetoothAgentServiceProvider* Create(
      dbus::Bus* bus,
      const dbus::ObjectPath& object_path,
      Delegate* delegate);

 protected:
  BluetoothAgentServiceProvider();

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothAgentServiceProvider);
};

}  

#endif  
