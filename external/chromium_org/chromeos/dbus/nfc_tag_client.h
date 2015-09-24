// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_NFC_TAG_CLIENT_H_
#define CHROMEOS_DBUS_NFC_TAG_CLIENT_H_

#include <map>
#include <string>
#include <vector>

#include "base/values.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/nfc_client_helpers.h"
#include "chromeos/dbus/nfc_property_set.h"
#include "chromeos/dbus/nfc_record_client.h"
#include "dbus/object_path.h"
#include "dbus/object_proxy.h"
#include "dbus/property.h"

namespace chromeos {

class NfcAdapterClient;

class CHROMEOS_EXPORT NfcTagClient : public DBusClient {
 public:
  
  struct Properties : public NfcPropertySet {
    
    
    dbus::Property<std::string> type;

    
    
    dbus::Property<std::string> protocol;

    
    
    dbus::Property<std::vector<dbus::ObjectPath> > records;

    
    dbus::Property<bool> read_only;

    Properties(dbus::ObjectProxy* object_proxy,
               const PropertyChangedCallback& callback);
    virtual ~Properties();
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void TagAdded(const dbus::ObjectPath& object_path) {}

    
    
    virtual void TagRemoved(const dbus::ObjectPath& object_path) {}

    
    
    virtual void TagPropertyChanged(const dbus::ObjectPath& object_path,
                                    const std::string& property_name) {}
  };

  virtual ~NfcTagClient();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path) = 0;

  
  
  
  // resulting record object and written to the tag. The properties are defined
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Write(
      const dbus::ObjectPath& object_path,
      const base::DictionaryValue& attributes,
      const base::Closure& callback,
      const nfc_client_helpers::ErrorCallback& error_callback) = 0;

  
  static NfcTagClient* Create(NfcAdapterClient* adapter_client);

 protected:
  friend class NfcClientTest;

  NfcTagClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcTagClient);
};

}  

#endif  
