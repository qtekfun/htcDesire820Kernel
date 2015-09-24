// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_ATTESTATION_ATTESTATION_FLOW_H_
#define CHROMEOS_ATTESTATION_ATTESTATION_FLOW_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/attestation/attestation_constants.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

namespace cryptohome {

class AsyncMethodCaller;

}  

namespace chromeos {

class CryptohomeClient;

namespace attestation {

class CHROMEOS_EXPORT ServerProxy {
 public:
  typedef base::Callback<void(bool success,
                              const std::string& data)> DataCallback;
  virtual ~ServerProxy() {}
  virtual void SendEnrollRequest(const std::string& request,
                                 const DataCallback& on_response) = 0;
  virtual void SendCertificateRequest(const std::string& request,
                                      const DataCallback& on_response) = 0;
};

class CHROMEOS_EXPORT AttestationFlow {
 public:
  typedef base::Callback<void(bool success,
                              const std::string& pem_certificate_chain)>
      CertificateCallback;

  AttestationFlow(cryptohome::AsyncMethodCaller* async_caller,
                  CryptohomeClient* cryptohome_client,
                  scoped_ptr<ServerProxy> server_proxy);
  virtual ~AttestationFlow();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void GetCertificate(AttestationCertificateProfile certificate_profile,
                              const std::string& user_id,
                              const std::string& request_origin,
                              bool force_new_key,
                              const CertificateCallback& callback);

 private:
  
  
  
  
  
  void StartEnroll(const base::Closure& on_failure,
                   const base::Closure& next_task);

  
  
  
  
  
  
  
  
  
  void SendEnrollRequestToPCA(const base::Closure& on_failure,
                              const base::Closure& next_task,
                              bool success,
                              const std::string& data);

  
  
  
  
  
  
  
  
  
  void SendEnrollResponseToDaemon(const base::Closure& on_failure,
                                  const base::Closure& next_task,
                                  bool success,
                                  const std::string& data);

  
  
  
  
  
  
  
  
  void OnEnrollComplete(const base::Closure& on_failure,
                        const base::Closure& next_task,
                        bool success,
                        cryptohome::MountError not_used);

  
  
  
  
  
  
  
  
  
  
  void StartCertificateRequest(
      const AttestationCertificateProfile certificate_profile,
      const std::string& user_id,
      const std::string& request_origin,
      bool generate_new_key,
      const CertificateCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  void SendCertificateRequestToPCA(AttestationKeyType key_type,
                                   const std::string& user_id,
                                   const std::string& key_name,
                                   const CertificateCallback& callback,
                                   bool success,
                                   const std::string& data);

  
  
  
  
  
  
  
  
  
  
  
  void SendCertificateResponseToDaemon(AttestationKeyType key_type,
                                       const std::string& user_id,
                                       const std::string& key_name,
                                       const CertificateCallback& callback,
                                       bool success,
                                       const std::string& data);

  
  
  
  
  
  
  
  void GetExistingCertificate(AttestationKeyType key_type,
                              const std::string& user_id,
                              const std::string& key_name,
                              const CertificateCallback& callback);

  cryptohome::AsyncMethodCaller* async_caller_;
  CryptohomeClient* cryptohome_client_;
  scoped_ptr<ServerProxy> server_proxy_;

  base::WeakPtrFactory<AttestationFlow> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(AttestationFlow);
};

}  
}  

#endif  
