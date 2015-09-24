// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_CONFIGURATION_HANDLER_H_
#define CHROMEOS_NETWORK_NETWORK_CONFIGURATION_HANDLER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "chromeos/network/network_handler.h"
#include "chromeos/network/network_handler_callbacks.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace dbus {
class ObjectPath;
}

namespace chromeos {


class CHROMEOS_EXPORT NetworkConfigurationHandler
    : public base::SupportsWeakPtr<NetworkConfigurationHandler> {
 public:
  ~NetworkConfigurationHandler();

  
  
  void GetProperties(
      const std::string& service_path,
      const network_handler::DictionaryResultCallback& callback,
      const network_handler::ErrorCallback& error_callback) const;

  
  
  
  
  void SetProperties(
      const std::string& service_path,
      const base::DictionaryValue& properties,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  void ClearProperties(const std::string& service_path,
                       const std::vector<std::string>& property_paths,
                       const base::Closure& callback,
                       const network_handler::ErrorCallback& error_callback);

  
  
  
  
  
  
  void CreateConfiguration(
      const base::DictionaryValue& properties,
      const network_handler::StringResultCallback& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  void RemoveConfiguration(
      const std::string& service_path,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback);

  
  
  void SetNetworkProfile(const std::string& service_path,
                         const std::string& profile_path,
                         const base::Closure& callback,
                         const network_handler::ErrorCallback& error_callback);

  
  static NetworkConfigurationHandler* InitializeForTest(
      NetworkStateHandler* network_state_handler);

 protected:
  friend class ClientCertResolverTest;
  friend class NetworkHandler;
  friend class NetworkConfigurationHandlerTest;
  friend class NetworkConfigurationHandlerStubTest;
  class ProfileEntryDeleter;

  NetworkConfigurationHandler();
  void Init(NetworkStateHandler* network_state_handler);

  void RunCreateNetworkCallback(
      const network_handler::StringResultCallback& callback,
      const dbus::ObjectPath& service_path);

  
  
  void ProfileEntryDeleterCompleted(const std::string& service_path);
  bool PendingProfileEntryDeleterForTest(const std::string& service_path) {
    return profile_entry_deleters_.count(service_path);
  }

  
  
  void SetPropertiesSuccessCallback(const std::string& service_path,
                                    const base::Closure& callback);
  void SetPropertiesErrorCallback(
      const std::string& service_path,
      const network_handler::ErrorCallback& error_callback,
      const std::string& dbus_error_name,
      const std::string& dbus_error_message);

  
  
  void ClearPropertiesSuccessCallback(
      const std::string& service_path,
      const std::vector<std::string>& names,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback,
      const base::ListValue& result);
  void ClearPropertiesErrorCallback(
      const std::string& service_path,
      const network_handler::ErrorCallback& error_callback,
      const std::string& dbus_error_name,
      const std::string& dbus_error_message);

  
  NetworkStateHandler* network_state_handler_;

  
  std::map<std::string, ProfileEntryDeleter*> profile_entry_deleters_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConfigurationHandler);
};

}  

#endif  
