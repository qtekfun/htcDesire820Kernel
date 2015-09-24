// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SHILL_SERVICE_CLIENT_H_
#define CHROMEOS_DBUS_SHILL_SERVICE_CLIENT_H_

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

class CHROMEOS_EXPORT ShillServiceClient : public DBusClient {
 public:
  typedef ShillClientHelper::PropertyChangedHandler PropertyChangedHandler;
  typedef ShillClientHelper::DictionaryValueCallback DictionaryValueCallback;
  typedef ShillClientHelper::ListValueCallback ListValueCallback;
  typedef ShillClientHelper::ErrorCallback ErrorCallback;

  
  
  class TestInterface {
   public:
    
    virtual void AddService(const std::string& service_path,
                            const std::string& name,
                            const std::string& type,
                            const std::string& state,
                            bool add_to_visible_list,
                            bool add_to_watch_list) = 0;
    virtual void AddServiceWithIPConfig(const std::string& service_path,
                                        const std::string& name,
                                        const std::string& type,
                                        const std::string& state,
                                        const std::string& ipconfig_path,
                                        bool add_to_visible_list,
                                        bool add_to_watch_list) = 0;

    
    virtual void RemoveService(const std::string& service_path) = 0;

    
    virtual bool SetServiceProperty(const std::string& service_path,
                                    const std::string& property,
                                    const base::Value& value) = 0;

    
    virtual const base::DictionaryValue* GetServiceProperties(
        const std::string& service_path) const = 0;

    
    virtual void ClearServices() = 0;

    virtual void SetConnectBehavior(const std::string& service_path,
                                    const base::Closure& behavior) = 0;

   protected:
    virtual ~TestInterface() {}
  };
  virtual ~ShillServiceClient();

  
  
  static ShillServiceClient* Create();

  
  virtual void AddPropertyChangedObserver(
      const dbus::ObjectPath& service_path,
      ShillPropertyChangedObserver* observer) = 0;

  
  virtual void RemovePropertyChangedObserver(
      const dbus::ObjectPath& service_path,
      ShillPropertyChangedObserver* observer) = 0;

  
  
  virtual void GetProperties(const dbus::ObjectPath& service_path,
                             const DictionaryValueCallback& callback) = 0;

  
  
  virtual void SetProperty(const dbus::ObjectPath& service_path,
                           const std::string& name,
                           const base::Value& value,
                           const base::Closure& callback,
                           const ErrorCallback& error_callback) = 0;

  
  
  virtual void SetProperties(const dbus::ObjectPath& service_path,
                             const base::DictionaryValue& properties,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) = 0;

  
  
  virtual void ClearProperty(const dbus::ObjectPath& service_path,
                             const std::string& name,
                             const base::Closure& callback,
                             const ErrorCallback& error_callback) = 0;

  
  
  virtual void ClearProperties(const dbus::ObjectPath& service_path,
                               const std::vector<std::string>& names,
                               const ListValueCallback& callback,
                               const ErrorCallback& error_callback) = 0;

  
  
  virtual void Connect(const dbus::ObjectPath& service_path,
                       const base::Closure& callback,
                       const ErrorCallback& error_callback) = 0;

  
  
  virtual void Disconnect(const dbus::ObjectPath& service_path,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  virtual void Remove(const dbus::ObjectPath& service_path,
                      const base::Closure& callback,
                      const ErrorCallback& error_callback) = 0;

  
  
  virtual void ActivateCellularModem(
      const dbus::ObjectPath& service_path,
      const std::string& carrier,
      const base::Closure& callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void CompleteCellularActivation(
      const dbus::ObjectPath& service_path,
      const base::Closure& callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void GetLoadableProfileEntries(
      const dbus::ObjectPath& service_path,
      const DictionaryValueCallback& callback) = 0;

  
  virtual TestInterface* GetTestInterface() = 0;

 protected:
  friend class ShillServiceClientTest;

  
  ShillServiceClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(ShillServiceClient);
};

}  

#endif  
