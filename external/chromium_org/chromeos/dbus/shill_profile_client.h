// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_SHILL_PROFILE_CLIENT_H_
#define CHROMEOS_DBUS_SHILL_PROFILE_CLIENT_H_

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

class CHROMEOS_EXPORT ShillProfileClient : public DBusClient {
 public:
  typedef ShillClientHelper::PropertyChangedHandler PropertyChangedHandler;
  typedef ShillClientHelper::DictionaryValueCallbackWithoutStatus
      DictionaryValueCallbackWithoutStatus;
  typedef ShillClientHelper::ErrorCallback ErrorCallback;

  
  
  
  
  class TestInterface {
   public:
    virtual void AddProfile(const std::string& profile_path,
                            const std::string& userhash) = 0;

    
    
    
    
    virtual void AddEntry(const std::string& profile_path,
                          const std::string& entry_path,
                          const base::DictionaryValue& properties) = 0;

    
    
    
    virtual bool AddService(const std::string& profile_path,
                            const std::string& service_path) = 0;

    
    virtual void GetProfilePaths(std::vector<std::string>* profiles) = 0;

   protected:
    virtual ~TestInterface() {}
  };

  virtual ~ShillProfileClient();

  
  
  static ShillProfileClient* Create();

  
  virtual void AddPropertyChangedObserver(
      const dbus::ObjectPath& profile_path,
      ShillPropertyChangedObserver* observer) = 0;

  
  virtual void RemovePropertyChangedObserver(
      const dbus::ObjectPath& profile_path,
      ShillPropertyChangedObserver* observer) = 0;

  
  
  virtual void GetProperties(
      const dbus::ObjectPath& profile_path,
      const DictionaryValueCallbackWithoutStatus& callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void GetEntry(const dbus::ObjectPath& profile_path,
                        const std::string& entry_path,
                        const DictionaryValueCallbackWithoutStatus& callback,
                        const ErrorCallback& error_callback) = 0;

  
  
  virtual void DeleteEntry(const dbus::ObjectPath& profile_path,
                           const std::string& entry_path,
                           const base::Closure& callback,
                           const ErrorCallback& error_callback) = 0;

  
  virtual TestInterface* GetTestInterface() = 0;

 protected:
  friend class ShillProfileClientTest;

  
  ShillProfileClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(ShillProfileClient);
};

}  

#endif  
