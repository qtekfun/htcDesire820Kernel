// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_NFC_DEVICE_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_NFC_DEVICE_CLIENT_H_

#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/nfc_client_helpers.h"
#include "chromeos/dbus/nfc_device_client.h"

namespace chromeos {

class CHROMEOS_EXPORT FakeNfcDeviceClient : public NfcDeviceClient {
 public:
  struct Properties : public NfcDeviceClient::Properties {
    explicit Properties(const PropertyChangedCallback& callback);
    virtual ~Properties();

    
    virtual void Get(dbus::PropertyBase* property,
                     dbus::PropertySet::GetCallback callback) OVERRIDE;
    virtual void GetAll() OVERRIDE;
    virtual void Set(dbus::PropertyBase* property,
                     dbus::PropertySet::SetCallback callback) OVERRIDE;
  };

  FakeNfcDeviceClient();
  virtual ~FakeNfcDeviceClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual Properties* GetProperties(
      const dbus::ObjectPath& object_path) OVERRIDE;
  virtual void Push(
      const dbus::ObjectPath& object_path,
      const base::DictionaryValue& attributes,
      const base::Closure& callback,
      const nfc_client_helpers::ErrorCallback& error_callback) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(FakeNfcDeviceClient);
};

}  

#endif  
