// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_CHANGE_NOTIFIER_CHROMEOS_H_
#define CHROMEOS_NETWORK_NETWORK_CHANGE_NOTIFIER_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/power_manager_client.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "net/base/network_change_notifier.h"

namespace chromeos {

class CHROMEOS_EXPORT NetworkChangeNotifierChromeos
    : public net::NetworkChangeNotifier,
      public chromeos::PowerManagerClient::Observer,
      public chromeos::NetworkStateHandlerObserver {
 public:
  NetworkChangeNotifierChromeos();
  virtual ~NetworkChangeNotifierChromeos();

  
  void Initialize();

  
  void Shutdown();

  
  virtual net::NetworkChangeNotifier::ConnectionType
      GetCurrentConnectionType() const OVERRIDE;

  
  virtual void SystemResumed(const base::TimeDelta& sleep_duration) OVERRIDE;

  
  virtual void DefaultNetworkChanged(
      const chromeos::NetworkState* default_network) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(NetworkChangeNotifierChromeosTest,
                           ConnectionTypeFromShill);
  friend class NetworkChangeNotifierChromeosUpdateTest;

  class DnsConfigService;

  
  
  
  
  
  void UpdateState(const chromeos::NetworkState* default_network,
                   bool* connection_type_changed,
                   bool* ip_address_changed,
                   bool* dns_changed);

  
  
  static net::NetworkChangeNotifier::ConnectionType
      ConnectionTypeFromShill(const std::string& type,
                              const std::string& technology);

  
  
  static net::NetworkChangeNotifier::NetworkChangeCalculatorParams
      NetworkChangeCalculatorParamsChromeos();

  NetworkChangeNotifier::ConnectionType connection_type_;
  
  std::string ip_address_;
  
  std::vector<std::string> dns_servers_;
  
  std::string service_path_;

  scoped_ptr<DnsConfigService> dns_config_service_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierChromeos);
};

}  

#endif  
