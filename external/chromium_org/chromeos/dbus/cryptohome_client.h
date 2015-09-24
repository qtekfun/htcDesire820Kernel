// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_CRYPTOHOME_CLIENT_H_
#define CHROMEOS_DBUS_CRYPTOHOME_CLIENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/attestation/attestation_constants.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/dbus_method_call_status.h"

namespace chromeos {

class CHROMEOS_EXPORT CryptohomeClient : public DBusClient {
 public:
  
  typedef base::Callback<void(int async_id,
                              bool return_status,
                              int return_code)>
      AsyncCallStatusHandler;
  
  typedef base::Callback<void(int async_id,
                              bool return_status,
                              const std::string& data)>
      AsyncCallStatusWithDataHandler;
  
  typedef base::Callback<void(int async_id)> AsyncMethodCallback;
  
  typedef base::Callback<void(
      DBusMethodCallStatus call_status,
      const std::vector<uint8>& system_salt)> GetSystemSaltCallback;
  
  typedef base::Callback<void(bool service_is_ready)>
      WaitForServiceToBeAvailableCallback;
  
  
  
  
  
  
  
  
  typedef base::Callback<void(
      DBusMethodCallStatus call_status,
      const std::string& label,
      const std::string& user_pin,
      int slot)> Pkcs11GetTpmTokenInfoCallback;
  
  typedef base::Callback<void(DBusMethodCallStatus call_status,
                              bool result,
                              const std::string& data)> DataMethodCallback;

  virtual ~CryptohomeClient();

  
  
  static CryptohomeClient* Create();

  
  static std::string GetStubSanitizedUsername(const std::string& username);

  
  
  
  
  virtual void SetAsyncCallStatusHandlers(
      const AsyncCallStatusHandler& handler,
      const AsyncCallStatusWithDataHandler& data_handler) = 0;

  
  virtual void ResetAsyncCallStatusHandlers() = 0;

  
  virtual void WaitForServiceToBeAvailable(
      const WaitForServiceToBeAvailableCallback& callback) = 0;

  
  virtual void IsMounted(const BoolDBusMethodCallback& callback) = 0;

  
  
  virtual bool Unmount(bool* success) = 0;

  
  
  virtual void AsyncCheckKey(const std::string& username,
                             const std::string& key,
                             const AsyncMethodCallback& callback) = 0;

  
  
  virtual void AsyncMigrateKey(const std::string& username,
                               const std::string& from_key,
                               const std::string& to_key,
                               const AsyncMethodCallback& callback) = 0;

  
  
  virtual void AsyncRemove(const std::string& username,
                           const AsyncMethodCallback& callback) = 0;

  
  
  virtual void GetSystemSalt(const GetSystemSaltCallback& callback) = 0;

  
  
  virtual void GetSanitizedUsername(
      const std::string& username,
      const StringDBusMethodCallback& callback) = 0;

  
  
  
  
  
  
  virtual std::string BlockingGetSanitizedUsername(
      const std::string& username) = 0;

  
  
  
  
  virtual void AsyncMount(const std::string& username,
                          const std::string& key,
                          int flags,
                          const AsyncMethodCallback& callback) = 0;

  
  
  
  virtual void AsyncAddKey(const std::string& username,
                           const std::string& key,
                           const std::string& new_key,
                           const AsyncMethodCallback& callback) = 0;

  
  
  virtual void AsyncMountGuest(const AsyncMethodCallback& callback) = 0;

  
  
  
  
  virtual void AsyncMountPublic(const std::string& public_mount_id,
                                int flags,
                                const AsyncMethodCallback& callback) = 0;

  
  virtual void TpmIsReady(const BoolDBusMethodCallback& callback) = 0;

  
  virtual void TpmIsEnabled(const BoolDBusMethodCallback& callback) = 0;

  
  
  
  virtual bool CallTpmIsEnabledAndBlock(bool* enabled) = 0;

  
  virtual void TpmGetPassword(const StringDBusMethodCallback& callback) = 0;

  
  virtual void TpmIsOwned(const BoolDBusMethodCallback& callback) = 0;

  
  
  
  virtual bool CallTpmIsOwnedAndBlock(bool* owned) = 0;

  
  virtual void TpmIsBeingOwned(const BoolDBusMethodCallback& callback) = 0;

  
  
  
  virtual bool CallTpmIsBeingOwnedAndBlock(bool* owning) = 0;

  
  
  virtual void TpmCanAttemptOwnership(
      const VoidDBusMethodCallback& callback) = 0;

  
  virtual void TpmClearStoredPassword(
      const VoidDBusMethodCallback& callback) = 0;

  
  
  
  virtual bool CallTpmClearStoredPasswordAndBlock() = 0;

  
  virtual void Pkcs11IsTpmTokenReady(
      const BoolDBusMethodCallback& callback) = 0;

  
  
  
  
  virtual void Pkcs11GetTpmTokenInfo(
      const Pkcs11GetTpmTokenInfoCallback& callback) = 0;

  
  
  
  
  virtual void Pkcs11GetTpmTokenInfoForUser(
      const std::string& user_email,
      const Pkcs11GetTpmTokenInfoCallback& callback) = 0;

  
  
  
  virtual bool InstallAttributesGet(const std::string& name,
                                    std::vector<uint8>* value,
                                    bool* successful) = 0;

  
  
  virtual bool InstallAttributesSet(const std::string& name,
                                    const std::vector<uint8>& value,
                                    bool* successful) = 0;

  
  
  virtual bool InstallAttributesFinalize(bool* successful) = 0;

  
  virtual void InstallAttributesIsReady(
      const BoolDBusMethodCallback& callback) = 0;

  
  
  virtual bool InstallAttributesIsInvalid(bool* is_invalid) = 0;

  
  
  virtual bool InstallAttributesIsFirstInstall(bool* is_first_install) = 0;

  
  
  virtual void TpmAttestationIsPrepared(
        const BoolDBusMethodCallback& callback) = 0;

  
  
  virtual void TpmAttestationIsEnrolled(
        const BoolDBusMethodCallback& callback) = 0;

  
  
  
  
  
  virtual void AsyncTpmAttestationCreateEnrollRequest(
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  virtual void AsyncTpmAttestationEnroll(
      const std::string& pca_response,
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void AsyncTpmAttestationCreateCertRequest(
      attestation::AttestationCertificateProfile certificate_profile,
      const std::string& user_id,
      const std::string& request_origin,
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void AsyncTpmAttestationFinishCertRequest(
      const std::string& pca_response,
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  
  virtual void TpmAttestationDoesKeyExist(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const BoolDBusMethodCallback& callback) = 0;

  
  
  
  
  
  
  virtual void TpmAttestationGetCertificate(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const DataMethodCallback& callback) = 0;

  
  
  
  
  
  virtual void TpmAttestationGetPublicKey(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const DataMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void TpmAttestationRegisterKey(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual void TpmAttestationSignEnterpriseChallenge(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const std::string& domain,
      const std::string& device_id,
      attestation::AttestationChallengeOptions options,
      const std::string& challenge,
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual void TpmAttestationSignSimpleChallenge(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const std::string& challenge,
      const AsyncMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void TpmAttestationGetKeyPayload(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const DataMethodCallback& callback) = 0;

  
  
  
  
  
  
  virtual void TpmAttestationSetKeyPayload(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_name,
      const std::string& payload,
      const BoolDBusMethodCallback& callback) = 0;

  
  
  
  
  
  
  
  
  virtual void TpmAttestationDeleteKeys(
      attestation::AttestationKeyType key_type,
      const std::string& user_id,
      const std::string& key_prefix,
      const BoolDBusMethodCallback& callback) = 0;

 protected:
  
  CryptohomeClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(CryptohomeClient);
};

}  

#endif  
