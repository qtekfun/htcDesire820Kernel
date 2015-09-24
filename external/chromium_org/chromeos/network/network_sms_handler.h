// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_SMS_HANDLER_H_
#define CHROMEOS_NETWORK_NETWORK_SMS_HANDLER_H_

#include <string>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/dbus/shill_property_changed_observer.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace chromeos {

class CHROMEOS_EXPORT NetworkSmsHandler : public ShillPropertyChangedObserver {
 public:
  static const char kNumberKey[];
  static const char kTextKey[];
  static const char kTimestampKey[];

  class Observer {
   public:
    virtual ~Observer() {}

    
    
    virtual void MessageReceived(const base::DictionaryValue& message) = 0;
  };

  virtual ~NetworkSmsHandler();

  
  
  void RequestUpdate(bool request_existing);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual void OnPropertyChanged(const std::string& name,
                                 const base::Value& value) OVERRIDE;

 private:
  friend class NetworkHandler;
  friend class NetworkSmsHandlerTest;

  class NetworkSmsDeviceHandler;
  class ModemManagerNetworkSmsDeviceHandler;
  class ModemManager1NetworkSmsDeviceHandler;

  NetworkSmsHandler();

  
  
  void Init();

  
  
  
  void AddReceivedMessage(const base::DictionaryValue& message);

  
  void NotifyMessageReceived(const base::DictionaryValue& message);

    
  void MessageReceived(const base::DictionaryValue& message);

  
  void ManagerPropertiesCallback(DBusMethodCallStatus call_status,
                                 const base::DictionaryValue& properties);

  
  void UpdateDevices(const base::ListValue* devices);

  
  
  void DevicePropertiesCallback(const std::string& device_path,
                                DBusMethodCallStatus call_status,
                                const base::DictionaryValue& properties);

  ObserverList<Observer> observers_;
  ScopedVector<NetworkSmsDeviceHandler> device_handlers_;
  ScopedVector<base::DictionaryValue> received_messages_;
  base::WeakPtrFactory<NetworkSmsHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkSmsHandler);
};

}  

#endif  
