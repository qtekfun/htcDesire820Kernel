// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_ATTESTATION_ATTESTATION_POLICY_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_ATTESTATION_ATTESTATION_POLICY_OBSERVER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"

namespace policy {
class CloudPolicyClient;
}

namespace chromeos {

class CrosSettings;
class CryptohomeClient;

namespace attestation {

class AttestationFlow;

class AttestationPolicyObserver {
 public:
  
  
  
  
  
  explicit AttestationPolicyObserver(policy::CloudPolicyClient* policy_client);

  
  
  AttestationPolicyObserver(policy::CloudPolicyClient* policy_client,
                            CryptohomeClient* cryptohome_client,
                            AttestationFlow* attestation_flow);

  ~AttestationPolicyObserver();

  
  void set_retry_delay(int retry_delay) {
    retry_delay_ = retry_delay;
  }

 private:
  
  void AttestationSettingChanged();

  
  void Start();

  
  void GetNewCertificate();

  
  void GetExistingCertificate();

  
  void CheckCertificateExpiry(const std::string& certificate);

  
  void UploadCertificate(const std::string& certificate);

  
  void CheckIfUploaded(const std::string& certificate,
                       const std::string& key_payload);

  
  void GetKeyPayload(base::Callback<void(const std::string&)> callback);

  
  
  void OnUploadComplete(bool status);

  
  void MarkAsUploaded(const std::string& key_payload);

  
  
  
  void Reschedule();

  CrosSettings* cros_settings_;
  policy::CloudPolicyClient* policy_client_;
  CryptohomeClient* cryptohome_client_;
  AttestationFlow* attestation_flow_;
  scoped_ptr<AttestationFlow> default_attestation_flow_;
  int num_retries_;
  int retry_delay_;

  scoped_ptr<CrosSettings::ObserverSubscription> attestation_subscription_;

  
  
  base::WeakPtrFactory<AttestationPolicyObserver> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(AttestationPolicyObserver);
};

}  
}  

#endif  
