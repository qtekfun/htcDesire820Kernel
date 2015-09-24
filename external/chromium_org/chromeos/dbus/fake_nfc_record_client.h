// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_NFC_RECORD_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_NFC_RECORD_CLIENT_H_

#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/nfc_record_client.h"
#include "dbus/object_path.h"

namespace chromeos {

class CHROMEOS_EXPORT FakeNfcRecordClient : public NfcRecordClient {
 public:
  struct Properties : public NfcRecordClient::Properties {
    explicit Properties(const PropertyChangedCallback& callback);
    virtual ~Properties();

    
    virtual void Get(dbus::PropertyBase* property,
                     dbus::PropertySet::GetCallback callback) OVERRIDE;
    virtual void GetAll() OVERRIDE;
    virtual void Set(dbus::PropertyBase* property,
                     dbus::PropertySet::SetCallback callback) OVERRIDE;
  };

  FakeNfcRecordClient();
  virtual ~FakeNfcRecordClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual Properties* GetProperties(
      const dbus::ObjectPath& object_path) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(FakeNfcRecordClient);
};

}  

#endif  
