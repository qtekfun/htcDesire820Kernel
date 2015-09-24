// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_SHILL_PROPERTY_HANDLER_H_
#define CHROMEOS_NETWORK_SHILL_PROPERTY_HANDLER_H_

#include <list>
#include <map>
#include <set>
#include <string>

#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/dbus/shill_property_changed_observer.h"
#include "chromeos/network/managed_state.h"
#include "chromeos/network/network_handler_callbacks.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace chromeos {

class ShillManagerClient;

namespace internal {

class ShillPropertyObserver;

class CHROMEOS_EXPORT ShillPropertyHandler
    : public ShillPropertyChangedObserver,
      public base::SupportsWeakPtr<ShillPropertyHandler> {
 public:
  typedef std::map<std::string, ShillPropertyObserver*>
      ShillPropertyObserverMap;

  class CHROMEOS_EXPORT Listener {
   public:
    
    virtual void UpdateManagedList(ManagedState::ManagedType type,
                                   const base::ListValue& entries) = 0;

    
    virtual void UpdateManagedStateProperties(
        ManagedState::ManagedType type,
        const std::string& path,
        const base::DictionaryValue& properties) = 0;

    
    virtual void ProfileListChanged() = 0;

    
    virtual void UpdateNetworkServiceProperty(
        const std::string& service_path,
        const std::string& key,
        const base::Value& value) = 0;

    
    virtual void UpdateDeviceProperty(
        const std::string& device_path,
        const std::string& key,
        const base::Value& value) = 0;

    
    virtual void CheckPortalListChanged(
         const std::string& check_portal_list) = 0;

    
    virtual void TechnologyListChanged() = 0;

    
    
    
    virtual void ManagedStateListChanged(ManagedState::ManagedType type) = 0;

   protected:
    virtual ~Listener() {}
  };

  explicit ShillPropertyHandler(Listener* listener);
  virtual ~ShillPropertyHandler();

  
  void Init();

  
  
  
  void UpdateManagerProperties();

  
  bool IsTechnologyAvailable(const std::string& technology) const;
  bool IsTechnologyEnabled(const std::string& technology) const;
  bool IsTechnologyEnabling(const std::string& technology) const;
  bool IsTechnologyUninitialized(const std::string& technology) const;

  
  
  void SetTechnologyEnabled(
      const std::string& technology,
      bool enabled,
      const network_handler::ErrorCallback& error_callback);

  
  void SetCheckPortalList(const std::string& check_portal_list);

  
  void RequestScan() const;

  
  void ConnectToBestServices() const;

  
  void RequestProperties(ManagedState::ManagedType type,
                         const std::string& path);

  
  virtual void OnPropertyChanged(const std::string& key,
                                 const base::Value& value) OVERRIDE;

 private:
  typedef std::map<ManagedState::ManagedType, std::set<std::string> >
      TypeRequestMap;

  
  void ManagerPropertiesCallback(DBusMethodCallStatus call_status,
                                 const base::DictionaryValue& properties);

  
  
  
  void CheckPendingStateListUpdates(const std::string& key);

  
  void ManagerPropertyChanged(const std::string& key,
                              const base::Value& value);

  
  
  
  
  
  
  
  void UpdateProperties(ManagedState::ManagedType type,
                        const base::ListValue& entries);

  
  void UpdateObserved(ManagedState::ManagedType type,
                      const base::ListValue& entries);


  
  void UpdateAvailableTechnologies(const base::ListValue& technologies);
  void UpdateEnabledTechnologies(const base::ListValue& technologies);
  void UpdateUninitializedTechnologies(const base::ListValue& technologies);

  void EnableTechnologyFailed(
      const std::string& technology,
      const network_handler::ErrorCallback& error_callback,
      const std::string& dbus_error_name,
      const std::string& dbus_error_message);

  
  void GetPropertiesCallback(ManagedState::ManagedType type,
                             const std::string& path,
                             DBusMethodCallStatus call_status,
                             const base::DictionaryValue& properties);

  
  
  void PropertyChangedCallback(ManagedState::ManagedType type,
                               const std::string& path,
                               const std::string& key,
                               const base::Value& value);
  void NetworkServicePropertyChangedCallback(const std::string& path,
                                             const std::string& key,
                                             const base::Value& value);

  
  
  
  void GetIPConfigCallback(const std::string& service_path,
                           DBusMethodCallStatus call_status,
                           const base::DictionaryValue& properties);
  void UpdateIPConfigProperty(const std::string& service_path,
                              const base::DictionaryValue& properties,
                              const char* property);

  void NetworkDevicePropertyChangedCallback(const std::string& path,
                                            const std::string& key,
                                            const base::Value& value);

  
  Listener* listener_;

  
  ShillManagerClient* shill_manager_;

  
  TypeRequestMap pending_updates_;

  
  
  TypeRequestMap requested_updates_;

  
  ShillPropertyObserverMap observed_networks_;

  
  ShillPropertyObserverMap observed_devices_;

  
  std::set<std::string> available_technologies_;
  std::set<std::string> enabled_technologies_;
  std::set<std::string> enabling_technologies_;
  std::set<std::string> uninitialized_technologies_;

  DISALLOW_COPY_AND_ASSIGN(ShillPropertyHandler);
};

}  
}  

#endif  
