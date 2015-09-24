// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SHILL_DEVICE_CLIENT_H_
#define CHROMEOS_DBUS_SHILL_DEVICE_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/shill_client_helper.h"

namespace base {

class Value;
class DictionaryValue;

}  

namespace dbus {

class ObjectPath;

}  

namespace chromeos {

class ShillPropertyChangedObserver;

class CHROMEOS_EXPORT ShillDeviceClient : public DBusClient {
 public:
  typedef ShillClientHelper::PropertyChangedHandler PropertyChangedHandler;
  typedef ShillClientHelper::DictionaryValueCallback DictionaryValueCallback;
  typedef ShillClientHelper::ErrorCallback ErrorCallback;

  
  
  class TestInterface {
   public:
    virtual void AddDevice(const std::string& device_path,
                           const std::string& type,
                           const std::string& object_path) = 0;
    virtual void RemoveDevice(const std::string& device_path) = 0;
    virtual void ClearDevices() = 0;
    virtual void SetDeviceProperty(const std::string& device_path,
                                   const std::string& name,
                                   const base::Value& value) = 0;
    virtual std::string GetDevicePathForType(const std::string& type) = 0;

   protected:
    virtual ~TestInterface() {}
  };

  virtual ~ShillDeviceClient();

  
  
  static ShillDeviceClient* Create();

  
  virtual void AddPropertyChangedObserver(
      const dbus::ObjectPath& device_path,
      ShillPropertyChangedObserver* observer) = 0;

  
  virtual void RemovePropertyChangedObserver(
      const dbus::ObjectPath& device_path,
      ShillPropertyChangedObserver* observer) = 0;

  
  
  virtual void GetProperties(const dbus::ObjectPath& device_path,
                             const DictionaryValueCallback& callback) = 0;

  
  
  virtual void ProposeScan(const dbus::ObjectPath& device_path,
                           const VoidDBusMethodCallback& callback) = 0;

  
  
  virtual void SetProperty(const dbus::ObjectPath& device_path,
                           const std::string& name,
                           const base::Value& value,
                           const base::Closure& callback,
                           const ErrorCallback& error_callback) = 0;

  
  
  virtual void ClearProperty(const dbus::ObjectPath& device_path,
                             const std::string& name,
                             const VoidDBusMethodCallback& callback) = 0;

  
  
  virtual void AddIPConfig(const dbus::ObjectPath& device_path,
                           const std::string& method,
                           const ObjectPathDBusMethodCallback& callback) = 0;

  
  
  virtual void RequirePin(const dbus::ObjectPath& device_path,
                          const std::string& pin,
                          bool require,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  virtual void EnterPin(const dbus::ObjectPath& device_path,
                        const std::string& pin,
                        const base::Closure& callback,
                        const ErrorCallback& error_callback) = 0;

  
  
  virtual void UnblockPin(const dbus::ObjectPath& device_path,
                          const std::string& puk,
                          const std::string& pin,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  virtual void ChangePin(const dbus::ObjectPath& device_path,
                         const std::string& old_pin,
                         const std::string& new_pin,
                         const base::Closure& callback,
                         const ErrorCallback& error_callback) = 0;

  
  
  virtual void Register(const dbus::ObjectPath& device_path,
                        const std::string& network_id,
                        const base::Closure& callback,
                        const ErrorCallback& error_callback) = 0;

  
  
  virtual void SetCarrier(const dbus::ObjectPath& device_path,
                          const std::string& carrier,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  virtual void Reset(const dbus::ObjectPath& device_path,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  virtual TestInterface* GetTestInterface() = 0;

 protected:
  friend class ShillDeviceClientTest;

  
  ShillDeviceClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(ShillDeviceClient);
};

}  

#endif  
