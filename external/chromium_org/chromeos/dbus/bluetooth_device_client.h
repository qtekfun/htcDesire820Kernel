// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_BLUETOOTH_DEVICE_CLIENT_H_
#define CHROMEOS_DBUS_BLUETOOTH_DEVICE_CLIENT_H_

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

class CHROMEOS_EXPORT BluetoothDeviceClient : public DBusClient {
 public:
  
  struct Properties : public dbus::PropertySet {
    
    dbus::Property<std::string> address;

    
    
    dbus::Property<std::string> name;

    
    
    dbus::Property<std::string> icon;

    
    dbus::Property<uint32> bluetooth_class;

    
    dbus::Property<uint16> appearance;

    
    dbus::Property<uint16> vendor;

    
    
    dbus::Property<std::vector<std::string> > uuids;

    
    dbus::Property<bool> paired;

    
    dbus::Property<bool> connected;

    
    
    dbus::Property<bool> trusted;

    
    
    dbus::Property<bool> blocked;

    
    dbus::Property<std::string> alias;

    
    dbus::Property<dbus::ObjectPath> adapter;

    
    
    
    dbus::Property<bool> legacy_pairing;

    
    dbus::Property<std::string> modalias;

    
    dbus::Property<int16> rssi;

    Properties(dbus::ObjectProxy* object_proxy,
               const std::string& interface_name,
               const PropertyChangedCallback& callback);
    virtual ~Properties();
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void DeviceAdded(const dbus::ObjectPath& object_path) {}

    
    
    virtual void DeviceRemoved(const dbus::ObjectPath& object_path) {}

    
    
    virtual void DevicePropertyChanged(const dbus::ObjectPath& object_path,
                                       const std::string& property_name) {}
  };

  virtual ~BluetoothDeviceClient();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  virtual std::vector<dbus::ObjectPath> GetDevicesForAdapter(
      const dbus::ObjectPath& adapter_path) = 0;

  
  
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path) = 0;

  
  
  
  typedef base::Callback<void(const std::string& error_name,
                              const std::string& error_message)> ErrorCallback;

  
  
  
  
  virtual void Connect(const dbus::ObjectPath& object_path,
                       const base::Closure& callback,
                       const ErrorCallback& error_callback) = 0;

  
  
  virtual void Disconnect(const dbus::ObjectPath& object_path,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  
  virtual void ConnectProfile(const dbus::ObjectPath& object_path,
                              const std::string& uuid,
                              const base::Closure& callback,
                              const ErrorCallback& error_callback) = 0;

  
  
  virtual void DisconnectProfile(const dbus::ObjectPath& object_path,
                                 const std::string& uuid,
                                 const base::Closure& callback,
                                 const ErrorCallback& error_callback) = 0;

  
  
  
  virtual void Pair(const dbus::ObjectPath& object_path,
                    const base::Closure& callback,
                    const ErrorCallback& error_callback) = 0;

  
  
  virtual void CancelPairing(const dbus::ObjectPath& object_path,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) = 0;

  
  static BluetoothDeviceClient* Create();

  
  static const char kNoResponseError[];
  static const char kUnknownDeviceError[];

 protected:
  BluetoothDeviceClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(BluetoothDeviceClient);
};

}  

#endif  
