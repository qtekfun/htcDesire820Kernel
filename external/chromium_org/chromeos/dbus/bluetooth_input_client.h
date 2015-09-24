// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLUETOOTH_INPUT_CLIENT_H_
#define CHROMEOS_DBUS_BLUETOOTH_INPUT_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

namespace chromeos {

class CHROMEOS_EXPORT BluetoothInputClient : public DBusClient {
 public:
  
  struct Properties : public dbus::PropertySet {
    
    dbus::Property<std::string> reconnect_mode;

    Properties(dbus::ObjectProxy* object_proxy,
               const std::string& interface_name,
               const PropertyChangedCallback& callback);
    virtual ~Properties();
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    virtual void InputAdded(const dbus::ObjectPath& object_path) {}

    
    
    
    virtual void InputRemoved(const dbus::ObjectPath& object_path) {}

    
    
    
    virtual void InputPropertyChanged(const dbus::ObjectPath& object_path,
                                      const std::string& property_name) {}
  };

  virtual ~BluetoothInputClient();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path) = 0;

  
  static BluetoothInputClient* Create();

 protected:
  BluetoothInputClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothInputClient);
};

}  

#endif  
