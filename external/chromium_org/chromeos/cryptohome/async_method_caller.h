// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_CRYPTOHOME_ASYNC_METHOD_CALLER_H_
#define CHROMEOS_CRYPTOHOME_ASYNC_METHOD_CALLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "chromeos/attestation/attestation_constants.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/cryptohome_client.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

namespace cryptohome {


enum MountFlags {
    MOUNT_FLAGS_NONE = 0,       
                                
    CREATE_IF_MISSING = 1,      
    ENSURE_EPHEMERAL = 1 << 1,  
};

class CHROMEOS_EXPORT AsyncMethodCaller {
 public:
  
  
  typedef base::Callback<void(bool success, MountError return_code)> Callback;
  typedef base::Callback<void(bool success, const std::string& data)>
      DataCallback;

  virtual ~AsyncMethodCaller() {}

  
  
  
  virtual void AsyncCheckKey(const std::string& user_email,
                             const std::string& passhash,
                             Callback callback) = 0;

  
  
  
  
  virtual void AsyncMigrateKey(const std::string& user_email,
                               const std::string& old_hash,
                               const std::string& new_hash,
                               Callback callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void AsyncMount(const std::string& user_email,
                          const std::string& passhash,
                          int flags,
                          Callback callback) = 0;

  
  
  
  virtual void AsyncAddKey(const std::string& user_email,
                           const std::string& passhash,
                           const std::string& new_passhash,
                           Callback callback) = 0;

  
  
  virtual void AsyncMountGuest(Callback callback) = 0;

  
  
  
  
  virtual void AsyncMountPublic(const std::string& public_mount_id,
                                int flags,
                                Callback callback) = 0;

  
  
  virtual void AsyncRemove(const std::string& user_email,
                           Callback callback) = 0;

  
  
  
  virtual void AsyncTpmAttestationCreateEnrollRequest(
      const DataCallback& callback) = 0;

  
  
  
  virtual void AsyncTpmAttestationEnroll(const std::string& pca_response,
                                         const Callback& callback) = 0;

  
  
  
  
  
  
  virtual void AsyncTpmAttestationCreateCertRequest(
      chromeos::attestation::AttestationCertificateProfile certificate_profile,
      const std::string& user_id,
      const std::string& request_origin,
      const DataCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual void AsyncTpmAttestationFinishCertRequest(
      const std::string& pca_response,
      chromeos::attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const DataCallback& callback) = 0;

  
  
  
  
  virtual void TpmAttestationRegisterKey(
      chromeos::attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const Callback& callback) = 0;

  
  
  
  
  
  
  
  virtual void TpmAttestationSignEnterpriseChallenge(
      chromeos::attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const std::string& domain,
      const std::string& device_id,
      chromeos::attestation::AttestationChallengeOptions options,
      const std::string& challenge,
      const DataCallback& callback) = 0;

  
  
  
  
  
  
  virtual void TpmAttestationSignSimpleChallenge(
      chromeos::attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const std::string& challenge,
      const DataCallback& callback) = 0;

  
  
  
  virtual void AsyncGetSanitizedUsername(
      const std::string& user,
      const DataCallback& callback) = 0;

  
  static void Initialize();

  
  
  
  
  static void InitializeForTesting(AsyncMethodCaller* async_method_caller);

  
  static void Shutdown();

  
  
  static AsyncMethodCaller* GetInstance();
};

}  

#endif  
