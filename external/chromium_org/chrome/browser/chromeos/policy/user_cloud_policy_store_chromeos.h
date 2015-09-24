// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_STORE_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_CLOUD_POLICY_STORE_CHROMEOS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "components/policy/core/common/cloud/cloud_policy_validator.h"
#include "components/policy/core/common/cloud/user_cloud_policy_store_base.h"

namespace base {
class SequencedTaskRunner;
}

namespace chromeos {
class CryptohomeClient;
class SessionManagerClient;
}

namespace policy {

class LegacyPolicyCacheLoader;

class UserCloudPolicyStoreChromeOS : public UserCloudPolicyStoreBase {
 public:
  UserCloudPolicyStoreChromeOS(
      chromeos::CryptohomeClient* cryptohome_client,
      chromeos::SessionManagerClient* session_manager_client,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner,
      const std::string& username,
      const base::FilePath& user_policy_key_dir,
      const base::FilePath& legacy_token_cache_file,
      const base::FilePath& legacy_policy_cache_file);
  virtual ~UserCloudPolicyStoreChromeOS();

  
  virtual void Store(
      const enterprise_management::PolicyFetchResponse& policy) OVERRIDE;
  virtual void Load() OVERRIDE;

  
  void LoadImmediately();

 private:
  
  void ValidatePolicyForStore(
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy);

  
  
  void OnPolicyToStoreValidated(UserCloudPolicyValidator* validator);

  
  void OnPolicyStored(bool);

  
  void OnPolicyRetrieved(const std::string& policy_blob);

  
  void ValidateRetrievedPolicy(
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy);

  
  
  void OnRetrievedPolicyValidated(UserCloudPolicyValidator* validator);

  
  void OnLegacyLoadFinished(
      const std::string& dm_token,
      const std::string& device_id,
      Status status,
      scoped_ptr<enterprise_management::PolicyFetchResponse>);

  
  void OnLegacyPolicyValidated(const std::string& dm_token,
                               const std::string& device_id,
                               UserCloudPolicyValidator* validator);

  
  void InstallLegacyTokens(const std::string& dm_token,
                           const std::string& device_id);

  
  static void RemoveLegacyCacheDir(const base::FilePath& dir);

  
  void ReloadPolicyKey(const base::Closure& callback);

  
  static void LoadPolicyKey(const base::FilePath& path,
                            std::vector<uint8>* key);

  
  void OnPolicyKeyReloaded(std::vector<uint8>* key,
                           const base::Closure& callback);

  
  
  void EnsurePolicyKeyLoaded(const base::Closure& callback);

  
  void OnGetSanitizedUsername(const base::Closure& callback,
                              chromeos::DBusMethodCallStatus call_status,
                              const std::string& sanitized_username);

  chromeos::CryptohomeClient* cryptohome_client_;
  chromeos::SessionManagerClient* session_manager_client_;
  const std::string username_;
  base::FilePath user_policy_key_dir_;

  base::WeakPtrFactory<UserCloudPolicyStoreChromeOS> weak_factory_;

  
  
  base::FilePath legacy_cache_dir_;
  scoped_ptr<LegacyPolicyCacheLoader> legacy_loader_;
  bool legacy_caches_loaded_;

  bool policy_key_loaded_;
  base::FilePath policy_key_path_;
  std::vector<uint8> policy_key_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudPolicyStoreChromeOS);
};

}  

#endif  
