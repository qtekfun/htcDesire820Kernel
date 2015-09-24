// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_NFC_ADAPTER_CLIENT_H_
#define CHROMEOS_DBUS_NFC_ADAPTER_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/nfc_client_helpers.h"
#include "chromeos/dbus/nfc_property_set.h"
#include "dbus/object_path.h"
#include "dbus/object_proxy.h"
#include "dbus/property.h"

namespace chromeos {

class NfcManagerClient;

class CHROMEOS_EXPORT NfcAdapterClient : public DBusClient {
 public:
  
  struct Properties : public NfcPropertySet {
    
    
    
    
    dbus::Property<std::string> mode;

    
    dbus::Property<bool> powered;

    
    
    dbus::Property<bool> polling;

    
    
    dbus::Property<std::vector<std::string> > protocols;

    
    
    dbus::Property<std::vector<dbus::ObjectPath> > tags;

    
    
    
    dbus::Property<std::vector<dbus::ObjectPath> > devices;

    Properties(dbus::ObjectProxy* object_proxy,
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

  virtual ~NfcAdapterClient();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual std::vector<dbus::ObjectPath> GetAdapters() = 0;

  
  
  
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path) = 0;

  
  
  
  
  
  
  virtual void StartPollLoop(
      const dbus::ObjectPath& object_path,
      const std::string& mode,
      const base::Closure& callback,
      const nfc_client_helpers::ErrorCallback& error_callback) = 0;

  
  virtual void StopPollLoop(
      const dbus::ObjectPath& object_path,
      const base::Closure& callback,
      const nfc_client_helpers::ErrorCallback& error_callback) = 0;

  
  static NfcAdapterClient* Create(NfcManagerClient* manager_client);

 protected:
  friend class NfcClientTest;

  NfcAdapterClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcAdapterClient);
};

}  

#endif   
