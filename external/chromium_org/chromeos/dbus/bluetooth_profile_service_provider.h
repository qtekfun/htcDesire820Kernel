// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLUETOOTH_PROFILE_SERVICE_PROVIDER_H_
#define CHROMEOS_DBUS_BLUETOOTH_PROFILE_SERVICE_PROVIDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "dbus/bus.h"
#include "dbus/file_descriptor.h"
#include "dbus/object_path.h"

namespace chromeos {

class CHROMEOS_EXPORT BluetoothProfileServiceProvider {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    enum Status {
      SUCCESS,
      REJECTED,
      CANCELLED
    };

    
    struct CHROMEOS_EXPORT Options {
      Options() {}
      ~Options() {}

      
      uint16 version;

      
      uint16 features;
    };

    
    
    
    typedef base::Callback<void(Status)> ConfirmationCallback;

    
    
    
    virtual void Release() = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void NewConnection(const dbus::ObjectPath& device_path,
                               scoped_ptr<dbus::FileDescriptor> fd,
                               const Options& options,
                               const ConfirmationCallback& callback) = 0;

    
    
    
    
    virtual void RequestDisconnection(const dbus::ObjectPath& device_path,
                                      const ConfirmationCallback& callback) = 0;

    
    
    virtual void Cancel() = 0;
  };

  virtual ~BluetoothProfileServiceProvider();

  
  
  
  
  static BluetoothProfileServiceProvider* Create(
      dbus::Bus* bus,
      const dbus::ObjectPath& object_path,
      Delegate* delegate);

 protected:
  BluetoothProfileServiceProvider();

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothProfileServiceProvider);
};

}  

#endif  
