// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_USER_CLOUD_POLICY_STORE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_USER_CLOUD_POLICY_STORE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/core/common/cloud/user_cloud_policy_store_base.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class POLICY_EXPORT UserCloudPolicyStore : public UserCloudPolicyStoreBase {
 public:
  
  
  UserCloudPolicyStore(
      const base::FilePath& policy_file,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);
  virtual ~UserCloudPolicyStore();

  
  
  static scoped_ptr<UserCloudPolicyStore> Create(
      const base::FilePath& profile_path,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

  
  void SetSigninUsername(const std::string& username);

  
  virtual void LoadImmediately();

  
  
  virtual void Clear();

  
  virtual void Load() OVERRIDE;
  virtual void Store(
      const enterprise_management::PolicyFetchResponse& policy) OVERRIDE;

 protected:
  std::string signin_username_;

 private:
  
  
  
  void PolicyLoaded(bool validate_in_background,
                    struct PolicyLoadResult policy_load_result);

  
  
  
  
  void Validate(
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy,
      bool validate_in_background,
      const UserCloudPolicyValidator::CompletionCallback& callback);

  
  
  void InstallLoadedPolicyAfterValidation(UserCloudPolicyValidator* validator);

  
  void StorePolicyAfterValidation(UserCloudPolicyValidator* validator);

  
  base::WeakPtrFactory<UserCloudPolicyStore> weak_factory_;

  
  base::FilePath backing_file_path_;

  DISALLOW_COPY_AND_ASSIGN(UserCloudPolicyStore);
};

}  

#endif  
