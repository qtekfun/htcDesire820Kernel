// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLUETOOTH_ADAPTER_CLIENT_H_
#define CHROMEOS_DBUS_BLUETOOTH_ADAPTER_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/observer_list.h"
#include "base/values.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

namespace chromeos {

class CHROMEOS_EXPORT BluetoothAdapterClient : public DBusClient {
 public:
  
  struct Properties : public dbus::PropertySet {
    
    dbus::Property<std::string> address;

    
    dbus::Property<std::string> name;

    
    
    
    dbus::Property<std::string> alias;

    
    dbus::Property<uint32> bluetooth_class;

    
    dbus::Property<bool> powered;

    
    
    
    dbus::Property<bool> discoverable;

    
    
    
    dbus::Property<bool> pairable;

    
    
    
    dbus::Property<uint32> pairable_timeout;

    
    
    
    dbus::Property<uint32> discoverable_timeout;

    
    
    dbus::Property<bool> discovering;

    
    
    dbus::Property<std::vector<std::string> > uuids;

    
    dbus::Property<std::string> modalias;

    Properties(dbus::ObjectProxy* object_proxy,
               const std::string& interface_name,
               const PropertyChangedCallback& callback);
    virtual ~Properties();
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void AdapterAdded(const dbus::ObjectPath& object_path) {}

    
    
    virtual void AdapterRemoved(const dbus::ObjectPath& object_path) {}

    
    
    virtual void AdapterPropertyChanged(const dbus::ObjectPath& object_path,
                                        const std::string& property_name) {}
  };

  virtual ~BluetoothAdapterClient();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual std::vector<dbus::ObjectPath> GetAdapters() = 0;

  
  
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path) = 0;

  
  
  
  typedef base::Callback<void(const std::string& error_name,
                              const std::string& error_message)> ErrorCallback;

  
  virtual void StartDiscovery(const dbus::ObjectPath& object_path,
                              const base::Closure& callback,
                              const ErrorCallback& error_callback) = 0;

  
  
  virtual void StopDiscovery(const dbus::ObjectPath& object_path,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) = 0;

  
  
  
  virtual void RemoveDevice(const dbus::ObjectPath& object_path,
                            const dbus::ObjectPath& device_path,
                            const base::Closure& callback,
                            const ErrorCallback& error_callback) = 0;

  
  static BluetoothAdapterClient* Create();

  
  static const char kNoResponseError[];
  static const char kUnknownAdapterError[];

 protected:
  BluetoothAdapterClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothAdapterClient);
};

}  

#endif  
