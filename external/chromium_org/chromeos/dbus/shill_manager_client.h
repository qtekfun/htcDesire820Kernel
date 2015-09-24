// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SHILL_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_SHILL_MANAGER_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/dbus/shill_client_helper.h"

namespace dbus {

class ObjectPath;

}  

namespace chromeos {

class ShillPropertyChangedObserver;

class CHROMEOS_EXPORT ShillManagerClient : public DBusClient {
 public:
  typedef ShillClientHelper::PropertyChangedHandler PropertyChangedHandler;
  typedef ShillClientHelper::DictionaryValueCallback DictionaryValueCallback;
  typedef ShillClientHelper::ErrorCallback ErrorCallback;
  typedef ShillClientHelper::StringCallback StringCallback;
  typedef ShillClientHelper::BooleanCallback BooleanCallback;

  
  
  class TestInterface {
   public:
    virtual void AddDevice(const std::string& device_path) = 0;
    virtual void RemoveDevice(const std::string& device_path) = 0;
    virtual void ClearDevices() = 0;
    virtual void AddTechnology(const std::string& type, bool enabled) = 0;
    virtual void RemoveTechnology(const std::string& type) = 0;
    virtual void SetTechnologyInitializing(const std::string& type,
                                           bool initializing) = 0;
    virtual void AddGeoNetwork(const std::string& technology,
                               const base::DictionaryValue& network) = 0;

    
    
    
    
    virtual void AddProfile(const std::string& profile_path) = 0;

    
    virtual void ClearProperties() = 0;

    
    virtual void AddManagerService(const std::string& service_path,
                                   bool add_to_visible_list,
                                   bool add_to_watch_list) = 0;
    virtual void RemoveManagerService(const std::string& service_path) = 0;
    virtual void ClearManagerServices() = 0;

    
    
    virtual void SortManagerServices() = 0;

   protected:
    virtual ~TestInterface() {}
  };

  
  struct VerificationProperties {
    VerificationProperties();
    ~VerificationProperties();

    
    
    std::string certificate;

    
    
    std::string public_key;

    
    
    std::string nonce;

    
    std::string signed_data;

    
    std::string device_serial;

    
    
    std::string device_ssid;

    
    
    std::string device_bssid;
  };

  virtual ~ShillManagerClient();

  
  
  static ShillManagerClient* Create();

  
  virtual void AddPropertyChangedObserver(
      ShillPropertyChangedObserver* observer) = 0;

  
  virtual void RemovePropertyChangedObserver(
      ShillPropertyChangedObserver* observer) = 0;

  
  
  virtual void GetProperties(const DictionaryValueCallback& callback) = 0;

  
  
  virtual void GetNetworksForGeolocation(
      const DictionaryValueCallback& callback) = 0;

  
  
  virtual void SetProperty(const std::string& name,
                           const base::Value& value,
                           const base::Closure& callback,
                           const ErrorCallback& error_callback) = 0;

  
  
  virtual void RequestScan(const std::string& type,
                           const base::Closure& callback,
                           const ErrorCallback& error_callback) = 0;

  
  
  virtual void EnableTechnology(const std::string& type,
                                const base::Closure& callback,
                                const ErrorCallback& error_callback) = 0;

  
  
  virtual void DisableTechnology(const std::string& type,
                                 const base::Closure& callback,
                                 const ErrorCallback& error_callback) = 0;

  
  
  virtual void ConfigureService(const base::DictionaryValue& properties,
                                const ObjectPathCallback& callback,
                                const ErrorCallback& error_callback) = 0;

  
  
  virtual void ConfigureServiceForProfile(
      const dbus::ObjectPath& profile_path,
      const base::DictionaryValue& properties,
      const ObjectPathCallback& callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void GetService(const base::DictionaryValue& properties,
                          const ObjectPathCallback& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  virtual void VerifyDestination(const VerificationProperties& properties,
                                 const BooleanCallback& callback,
                                 const ErrorCallback& error_callback) = 0;

  
  
  
  
  virtual void VerifyAndEncryptCredentials(
      const VerificationProperties& properties,
      const std::string& service_path,
      const StringCallback& callback,
      const ErrorCallback& error_callback) = 0;

  
  
  
  virtual void VerifyAndEncryptData(const VerificationProperties& properties,
                                    const std::string& data,
                                    const StringCallback& callback,
                                    const ErrorCallback& error_callback) = 0;

  
  
  virtual void ConnectToBestServices(const base::Closure& callback,
                                     const ErrorCallback& error_callback) = 0;

  
  virtual TestInterface* GetTestInterface() = 0;

 protected:
  friend class ShillManagerClientTest;

  
  ShillManagerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(ShillManagerClient);
};

}  

#endif  
