// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_NFC_RECORD_CLIENT_H_
#define CHROMEOS_DBUS_NFC_RECORD_CLIENT_H_

#include <string>

#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/nfc_property_set.h"
#include "dbus/object_path.h"
#include "dbus/object_proxy.h"
#include "dbus/property.h"

namespace chromeos {

class NfcDeviceClient;
class NfcTagClient;

class CHROMEOS_EXPORT NfcRecordClient : public DBusClient {
 public:
  
  struct Properties : public NfcPropertySet {
    
    
    dbus::Property<std::string> type;

    
    
    
    dbus::Property<std::string> encoding;

    
    
    dbus::Property<std::string> language;

    
    
    
    dbus::Property<std::string> representation;

    
    
    
    
    dbus::Property<std::string> uri;

    
    
    
    
    dbus::Property<std::string> mime_type;

    
    
    
    
    
    dbus::Property<std::string> size;

    
    
    
    
    
    dbus::Property<std::string> action;

    Properties(dbus::ObjectProxy* object_proxy,
               const PropertyChangedCallback& callback);
    virtual ~Properties();
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void RecordAdded(const dbus::ObjectPath& object_path) {}

    
    
    virtual void RecordRemoved(const dbus::ObjectPath& object_path) {}

    
    
    virtual void RecordPropertyChanged(const dbus::ObjectPath& object_path,
                                       const std::string& property_name) {}

    
    
    
    
    
    
    virtual void RecordPropertiesReceived(
        const dbus::ObjectPath& object_path) {}
  };

  virtual ~NfcRecordClient();

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  virtual Properties* GetProperties(const dbus::ObjectPath& object_path) = 0;

  
  static NfcRecordClient* Create(NfcDeviceClient* device_client,
                                 NfcTagClient* tag_client);

 protected:
  friend class NfcClientTest;

  NfcRecordClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcRecordClient);
};

}  

#endif  
