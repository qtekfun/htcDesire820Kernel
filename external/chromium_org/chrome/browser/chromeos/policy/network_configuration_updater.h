// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_NETWORK_CONFIGURATION_UPDATER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_NETWORK_CONFIGURATION_UPDATER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "components/onc/onc_constants.h"
#include "components/policy/core/common/policy_service.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace chromeos {
class ManagedNetworkConfigurationHandler;

namespace onc {
class CertificateImporter;
}
}

namespace policy {

class PolicyMap;

class NetworkConfigurationUpdater : public PolicyService::Observer {
 public:
  virtual ~NetworkConfigurationUpdater();

  
  
  
  static scoped_ptr<NetworkConfigurationUpdater> CreateForDevicePolicy(
      scoped_ptr<chromeos::onc::CertificateImporter> certificate_importer,
      PolicyService* policy_service,
      chromeos::ManagedNetworkConfigurationHandler* network_config_handler);

  
  virtual void OnPolicyUpdated(const PolicyNamespace& ns,
                               const PolicyMap& previous,
                               const PolicyMap& current) OVERRIDE;
  virtual void OnPolicyServiceInitialized(PolicyDomain domain) OVERRIDE;

 protected:
  NetworkConfigurationUpdater(
      onc::ONCSource onc_source,
      std::string policy_key,
      scoped_ptr<chromeos::onc::CertificateImporter> certificate_importer,
      PolicyService* policy_service,
      chromeos::ManagedNetworkConfigurationHandler* network_config_handler);

  void Init();

  
  virtual void ImportCertificates(const base::ListValue& certificates_onc);

  
  
  
  virtual void ApplyNetworkPolicy(base::ListValue* network_configs_onc,
                                  base::DictionaryValue* global_network_config);

  onc::ONCSource onc_source_;

  
  chromeos::ManagedNetworkConfigurationHandler* network_config_handler_;

  scoped_ptr<chromeos::onc::CertificateImporter> certificate_importer_;

 private:
  
  void OnPolicyChanged(const base::Value* previous, const base::Value* current);

  
  void ApplyPolicy();

  std::string LogHeader() const;

  std::string policy_key_;

  
  PolicyChangeRegistrar policy_change_registrar_;

  
  PolicyService* policy_service_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConfigurationUpdater);
};

}  

#endif  
