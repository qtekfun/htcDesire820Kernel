// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_POLICY_CERT_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_POLICY_CERT_SERVICE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/policy/user_network_configuration_updater.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace chromeos {
class UserManager;
}

namespace net {
class X509Certificate;
typedef std::vector<scoped_refptr<X509Certificate> > CertificateList;
}

namespace policy {

class PolicyCertVerifier;

class PolicyCertService
    : public BrowserContextKeyedService,
      public UserNetworkConfigurationUpdater::WebTrustedCertsObserver {
 public:
  PolicyCertService(const std::string& user_id,
                    UserNetworkConfigurationUpdater* net_conf_updater,
                    chromeos::UserManager* user_manager);
  virtual ~PolicyCertService();

  
  
  scoped_ptr<PolicyCertVerifier> CreatePolicyCertVerifier();

  
  
  
  bool UsedPolicyCertificates() const;

  bool has_policy_certificates() const { return has_trust_anchors_; }

  
  virtual void OnTrustAnchorsChanged(const net::CertificateList& trust_anchors)
      OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

  static scoped_ptr<PolicyCertService> CreateForTesting(
      const std::string& user_id,
      PolicyCertVerifier* verifier,
      chromeos::UserManager* user_manager);

 private:
  PolicyCertService(const std::string& user_id,
                    PolicyCertVerifier* verifier,
                    chromeos::UserManager* user_manager);

  PolicyCertVerifier* cert_verifier_;
  std::string user_id_;
  UserNetworkConfigurationUpdater* net_conf_updater_;
  chromeos::UserManager* user_manager_;
  bool has_trust_anchors_;

  
  
  base::WeakPtrFactory<PolicyCertService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PolicyCertService);
};

}  

#endif  
