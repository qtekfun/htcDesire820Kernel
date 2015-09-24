 // Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_MANAGED_NETWORK_CONFIGURATION_HANDLER_H_
#define CHROMEOS_NETWORK_MANAGED_NETWORK_CONFIGURATION_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/network/network_handler.h"
#include "chromeos/network/network_handler_callbacks.h"
#include "components/onc/onc_constants.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace chromeos {

class NetworkPolicyObserver;

class CHROMEOS_EXPORT ManagedNetworkConfigurationHandler {
 public:
  virtual ~ManagedNetworkConfigurationHandler();

  virtual void AddObserver(NetworkPolicyObserver* observer) = 0;
  virtual void RemoveObserver(NetworkPolicyObserver* observer) = 0;

  
  virtual void GetProperties(
      const std::string& service_path,
      const network_handler::DictionaryResultCallback& callback,
      const network_handler::ErrorCallback& error_callback) const = 0;

  
  
  
  virtual void GetManagedProperties(
      const std::string& userhash,
      const std::string& service_path,
      const network_handler::DictionaryResultCallback& callback,
      const network_handler::ErrorCallback& error_callback) = 0;

  
  
  
  
  
  virtual void SetProperties(
      const std::string& service_path,
      const base::DictionaryValue& user_settings,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback) const = 0;

  
  
  
  
  
  virtual void CreateConfiguration(
      const std::string& userhash,
      const base::DictionaryValue& properties,
      const network_handler::StringResultCallback& callback,
      const network_handler::ErrorCallback& error_callback) const = 0;

  
  
  
  virtual void RemoveConfiguration(
      const std::string& service_path,
      const base::Closure& callback,
      const network_handler::ErrorCallback& error_callback) const = 0;

  
  
  
  
  
  
  virtual void SetPolicy(
      ::onc::ONCSource onc_source,
      const std::string& userhash,
      const base::ListValue& network_configs_onc,
      const base::DictionaryValue& global_network_config) = 0;

  
  
  
  virtual const base::DictionaryValue* FindPolicyByGUID(
      const std::string userhash,
      const std::string& guid,
      ::onc::ONCSource* onc_source) const = 0;

  
  
  virtual const base::DictionaryValue* GetGlobalConfigFromPolicy(
      const std::string userhash) const = 0;

  
  
  virtual const base::DictionaryValue* FindPolicyByGuidAndProfile(
      const std::string& guid,
      const std::string& profile_path) const = 0;

 private:
  DISALLOW_ASSIGN(ManagedNetworkConfigurationHandler);
};

}  

#endif  
