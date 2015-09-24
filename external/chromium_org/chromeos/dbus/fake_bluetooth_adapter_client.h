// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_BLUETOOTH_ADAPTER_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_BLUETOOTH_ADAPTER_CLIENT_H_

#include <vector>

#include "base/bind.h"
#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/bluetooth_adapter_client.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

namespace chromeos {

class CHROMEOS_EXPORT FakeBluetoothAdapterClient
    : public BluetoothAdapterClient {
 public:
  struct Properties : public BluetoothAdapterClient::Properties {
    explicit Properties(const PropertyChangedCallback & callback);
    virtual ~Properties();

    
    virtual void Get(dbus::PropertyBase* property,
                     dbus::PropertySet::GetCallback callback) OVERRIDE;
    virtual void GetAll() OVERRIDE;
    virtual void Set(dbus::PropertyBase* property,
                     dbus::PropertySet::SetCallback callback) OVERRIDE;
  };

  FakeBluetoothAdapterClient();
  virtual ~FakeBluetoothAdapterClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual std::vector<dbus::ObjectPath> GetAdapters() OVERRIDE;
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path)
      OVERRIDE;
  virtual void StartDiscovery(const dbus::ObjectPath& object_path,
                              const base::Closure& callback,
                              const ErrorCallback& error_callback) OVERRIDE;
  virtual void StopDiscovery(const dbus::ObjectPath& object_path,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) OVERRIDE;
  virtual void RemoveDevice(const dbus::ObjectPath& object_path,
                            const dbus::ObjectPath& device_path,
                            const base::Closure& callback,
                            const ErrorCallback& error_callback) OVERRIDE;

  
  void SetVisible(bool visible);
  void SetSecondVisible(bool visible);

  
  static const char kAdapterPath[];
  static const char kAdapterName[];
  static const char kAdapterAddress[];

  static const char kSecondAdapterPath[];
  static const char kSecondAdapterName[];
  static const char kSecondAdapterAddress[];

 private:
  
  void OnPropertyChanged(const std::string& property_name);

  
  ObserverList<Observer> observers_;

  
  scoped_ptr<Properties> properties_;
  scoped_ptr<Properties> second_properties_;

  
  bool visible_;
  bool second_visible_;

  
  int discovering_count_;
};

}  

#endif  
