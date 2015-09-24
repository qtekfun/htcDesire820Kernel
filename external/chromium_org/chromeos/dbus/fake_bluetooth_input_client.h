// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_BLUETOOTH_INPUT_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_BLUETOOTH_INPUT_CLIENT_H_

#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/bluetooth_input_client.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

namespace chromeos {

class CHROMEOS_EXPORT FakeBluetoothInputClient
    : public BluetoothInputClient {
 public:
  struct Properties : public BluetoothInputClient::Properties {
    explicit Properties(const PropertyChangedCallback & callback);
    virtual ~Properties();

    
    virtual void Get(dbus::PropertyBase* property,
                     dbus::PropertySet::GetCallback callback) OVERRIDE;
    virtual void GetAll() OVERRIDE;
    virtual void Set(dbus::PropertyBase* property,
                     dbus::PropertySet::SetCallback callback) OVERRIDE;
  };

  FakeBluetoothInputClient();
  virtual ~FakeBluetoothInputClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path)
      OVERRIDE;

  
  void AddInputDevice(const dbus::ObjectPath& object_path);
  void RemoveInputDevice(const dbus::ObjectPath& object_path);

 private:
  
  void OnPropertyChanged(const dbus::ObjectPath& object_path,
                         const std::string& property_name);

  
  typedef std::map<const dbus::ObjectPath, Properties *> PropertiesMap;
  PropertiesMap properties_map_;

  
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(FakeBluetoothInputClient);
};

}  

#endif  
