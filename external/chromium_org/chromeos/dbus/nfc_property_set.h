// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_NFC_PROPERTY_SET_H_
#define CHROMEOS_DBUS_NFC_PROPERTY_SET_H_

#include <string>

#include "base/callback.h"
#include "dbus/message.h"
#include "dbus/object_proxy.h"
#include "dbus/property.h"

namespace chromeos {

class NfcPropertySet : public dbus::PropertySet {
 public:
  NfcPropertySet(dbus::ObjectProxy* object_proxy,
                 const std::string& interface,
                 const PropertyChangedCallback& callback);

  
  
  virtual ~NfcPropertySet();

  
  
  void SetAllPropertiesReceivedCallback(const base::Closure& callback);

  
  virtual void ConnectSignals() OVERRIDE;
  virtual void Get(dbus::PropertyBase* property,
                   GetCallback callback) OVERRIDE;
  virtual void GetAll() OVERRIDE;
  virtual void OnGetAll(dbus::Response* response) OVERRIDE;
  virtual void Set(dbus::PropertyBase* property,
                   SetCallback callback) OVERRIDE;
  virtual void ChangedReceived(dbus::Signal* signal) OVERRIDE;

 private:
  
  
  base::Closure on_get_all_callback_;

  DISALLOW_COPY_AND_ASSIGN(NfcPropertySet);
};

}  

#endif  
