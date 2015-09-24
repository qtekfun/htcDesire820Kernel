// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_STATE_H_
#define CHROMEOS_NETWORK_NETWORK_STATE_H_

#include <string>
#include <vector>

#include "chromeos/network/managed_state.h"
#include "chromeos/network/network_ui_data.h"
#include "components/onc/onc_constants.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace chromeos {

class CHROMEOS_EXPORT NetworkState : public ManagedState {
 public:
  explicit NetworkState(const std::string& path);
  virtual ~NetworkState();

  
  
  virtual bool PropertyChanged(const std::string& key,
                               const base::Value& value) OVERRIDE;
  virtual bool InitialPropertiesReceived(
      const base::DictionaryValue& properties) OVERRIDE;

  
  
  
  void GetProperties(base::DictionaryValue* dictionary) const;

  
  bool RequiresActivation() const;

  
  const std::string& security() const { return security_; }
  const std::string& eap_method() const { return eap_method_; }
  const std::string& device_path() const { return device_path_; }
  const std::string& guid() const { return guid_; }
  const std::string& connection_state() const { return connection_state_; }
  const std::string& profile_path() const { return profile_path_; }
  const std::string& error() const { return error_; }
  bool connectable() const { return connectable_; }

  const NetworkUIData& ui_data() const { return ui_data_; }

  
  
  const std::string& ip_address() const { return ip_address_; }
  const std::string& gateway() const { return gateway_; }
  const std::vector<std::string>& dns_servers() const { return dns_servers_; }
  const GURL& web_proxy_auto_discovery_url() const {
    return web_proxy_auto_discovery_url_;
  }

  
  int signal_strength() const { return signal_strength_; }

  
  const std::string& network_technology() const {
    return network_technology_;
  }
  const std::string& activation_state() const { return activation_state_; }
  const std::string& roaming() const { return roaming_; }
  bool activate_over_non_cellular_networks() const {
    return activate_over_non_cellular_networks_;
  }
  bool cellular_out_of_credits() const { return cellular_out_of_credits_; }

  
  bool HasCACertNSS() const { return has_ca_cert_nss_; }

  
  bool IsConnectedState() const;
  bool IsConnectingState() const;

  
  bool IsPrivate() const;

  
  std::string GetDnsServersAsString() const;

  
  std::string GetNetmask() const;

  
  static bool StateIsConnected(const std::string& connection_state);
  static bool StateIsConnecting(const std::string& connection_state);

  
  static std::string IPConfigProperty(const char* key);


 private:
  friend class MobileActivatorTest;
  friend class NetworkStateHandler;
  friend class NetworkChangeNotifierChromeosUpdateTest;

  
  
  bool UpdateName(const base::DictionaryValue& properties);

  
  
  
  std::string security_;
  std::string eap_method_;  
  std::string device_path_;
  std::string guid_;
  std::string connection_state_;
  std::string profile_path_;
  std::string error_;
  bool connectable_;

  
  
  NetworkUIData ui_data_;

  
  
  
  std::string ip_address_;
  std::string gateway_;
  std::vector<std::string> dns_servers_;
  int prefix_length_;  
  GURL web_proxy_auto_discovery_url_;

  
  int signal_strength_;

  
  std::string network_technology_;
  std::string activation_state_;
  std::string roaming_;
  bool activate_over_non_cellular_networks_;
  bool cellular_out_of_credits_;

  
  
  bool has_ca_cert_nss_;

  DISALLOW_COPY_AND_ASSIGN(NetworkState);
};

}  

#endif  
