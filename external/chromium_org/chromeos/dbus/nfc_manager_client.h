// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_NFC_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_NFC_MANAGER_CLIENT_H_

#include <vector>

#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/nfc_property_set.h"
#include "dbus/object_path.h"
#include "dbus/object_proxy.h"
#include "dbus/property.h"

namespace chromeos {

class CHROMEOS_EXPORT NfcManagerClient : public DBusClient {
 public:
  
  struct Properties : public NfcPropertySet {
    
    dbus::Property<std::vector<dbus::ObjectPath> > adapters;

    Properties(dbus::ObjectProxy* object_proxy,
               const PropertyChangedCallback& callback);
    virtual ~Properties();
  };

  
  
  
  
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void AdapterAdded(const dbus::ObjectPath& object_path) {}

    
    
    virtual void AdapterRemoved(const dbus::ObjectPath& object_path) {}

    
    
    virtual void ManagerPropertyChanged(const std::string& property_name) {}
  };

  virtual ~NfcManagerClient();

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual Properties* GetProperties() = 0;

  
  static NfcManagerClient* Create();

 protected:
  friend class NfcClientTest;

  NfcManagerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcManagerClient);
};

}  

#endif  
