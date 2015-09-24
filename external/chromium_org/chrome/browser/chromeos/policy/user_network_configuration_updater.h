// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_NETWORK_CONFIGURATION_UPDATER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_NETWORK_CONFIGURATION_UPDATER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/policy/network_configuration_updater.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace chromeos {
class User;
}

namespace net {
class X509Certificate;
typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;
}

namespace policy {

class PolicyService;

class UserNetworkConfigurationUpdater : public NetworkConfigurationUpdater,
                                        public BrowserContextKeyedService {
 public:
  class WebTrustedCertsObserver {
   public:
    
    
    virtual void OnTrustAnchorsChanged(
        const net::CertificateList& trust_anchors) = 0;
  };

  virtual ~UserNetworkConfigurationUpdater();

  
  
  
  
  
  static scoped_ptr<UserNetworkConfigurationUpdater> CreateForUserPolicy(
      bool allow_trusted_certs_from_policy,
      const chromeos::User& user,
      scoped_ptr<chromeos::onc::CertificateImporter> certificate_importer,
      PolicyService* policy_service,
      chromeos::ManagedNetworkConfigurationHandler* network_config_handler);

  void AddTrustedCertsObserver(WebTrustedCertsObserver* observer);
  void RemoveTrustedCertsObserver(WebTrustedCertsObserver* observer);

  
  
  void GetWebTrustedCertificates(net::CertificateList* certs) const;

 private:
  class CrosTrustAnchorProvider;

  UserNetworkConfigurationUpdater(
      bool allow_trusted_certs_from_policy,
      const chromeos::User& user,
      scoped_ptr<chromeos::onc::CertificateImporter> certificate_importer,
      PolicyService* policy_service,
      chromeos::ManagedNetworkConfigurationHandler* network_config_handler);

  virtual void ImportCertificates(
      const base::ListValue& certificates_onc) OVERRIDE;

  virtual void ApplyNetworkPolicy(
      base::ListValue* network_configs_onc,
      base::DictionaryValue* global_network_config) OVERRIDE;

  void NotifyTrustAnchorsChanged();

  
  bool allow_trusted_certificates_from_policy_;

  
  
  const chromeos::User* user_;

  ObserverList<WebTrustedCertsObserver, true> observer_list_;

  
  
  net::CertificateList web_trust_certs_;

  DISALLOW_COPY_AND_ASSIGN(UserNetworkConfigurationUpdater);
};

}  

#endif  
