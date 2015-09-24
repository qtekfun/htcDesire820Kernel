// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_FAKE_NFC_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_FAKE_NFC_MANAGER_CLIENT_H_

#include <set>
#include <string>

#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/nfc_manager_client.h"
#include "dbus/property.h"

namespace chromeos {

class CHROMEOS_EXPORT FakeNfcManagerClient : public NfcManagerClient {
 public:
  struct Properties : public NfcManagerClient::Properties {
    explicit Properties(const PropertyChangedCallback& callback);
    virtual ~Properties();

    
    virtual void Get(dbus::PropertyBase* property,
                     dbus::PropertySet::GetCallback callback) OVERRIDE;
    virtual void GetAll() OVERRIDE;
    virtual void Set(dbus::PropertyBase* property,
                     dbus::PropertySet::SetCallback callback) OVERRIDE;
  };

  FakeNfcManagerClient();
  virtual ~FakeNfcManagerClient();

  
  virtual void Init(dbus::Bus* bus) OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual Properties* GetProperties() OVERRIDE;

  
  void AddAdapter(const std::string& adapter_path);
  void RemoveAdapter(const std::string& adapter_path);

  
  static const char kDefaultAdapterPath[];

 private:
  
  void OnPropertyChanged(const std::string& property_name);

  
  ObserverList<Observer> observers_;

  
  std::set<dbus::ObjectPath> adapters_;

  
  
  scoped_ptr<Properties> properties_;

  DISALLOW_COPY_AND_ASSIGN(FakeNfcManagerClient);
};

}  

#endif  
