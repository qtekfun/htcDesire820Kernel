// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_VALIDATOR_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_VALIDATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner.h"
#include "base/time/time.h"
#include "components/policy/policy_export.h"
#include "policy/proto/cloud_policy.pb.h"

#if !defined(OS_ANDROID)
#include "policy/proto/chrome_extension_policy.pb.h"
#endif

namespace base {
class MessageLoopProxy;
}

namespace google {
namespace protobuf {
class MessageLite;
}
}

namespace enterprise_management {
class PolicyData;
class PolicyFetchResponse;
}

namespace policy {

class POLICY_EXPORT CloudPolicyValidatorBase {
 public:
  
  
  
  enum Status {
    
    VALIDATION_OK,
    
    VALIDATION_BAD_INITIAL_SIGNATURE,
    
    VALIDATION_BAD_SIGNATURE,
    
    VALIDATION_ERROR_CODE_PRESENT,
    
    VALIDATION_PAYLOAD_PARSE_ERROR,
    
    VALIDATION_WRONG_POLICY_TYPE,
    
    VALIDATION_WRONG_SETTINGS_ENTITY_ID,
    
    VALIDATION_BAD_TIMESTAMP,
    
    VALIDATION_WRONG_TOKEN,
    
    VALIDATION_BAD_USERNAME,
    
    VALIDATION_POLICY_PARSE_ERROR,
  };

  enum ValidateDMTokenOption {
    
    DM_TOKEN_REQUIRED,

    
    
    DM_TOKEN_NOT_REQUIRED,
  };

  enum ValidateTimestampOption {
    
    
    TIMESTAMP_REQUIRED,

    
    
    
    
    TIMESTAMP_NOT_BEFORE,

    
    TIMESTAMP_NOT_REQUIRED,
  };

  virtual ~CloudPolicyValidatorBase();

  
  Status status() const { return status_; }
  bool success() const { return status_ == VALIDATION_OK; }

  
  
  scoped_ptr<enterprise_management::PolicyFetchResponse>& policy() {
    return policy_;
  }
  scoped_ptr<enterprise_management::PolicyData>& policy_data() {
    return policy_data_;
  }

  
  
  
  
  void ValidateTimestamp(base::Time not_before,
                         base::Time not_after,
                         ValidateTimestampOption timestamp_option);

  
  void ValidateUsername(const std::string& expected_user);

  
  
  void ValidateDomain(const std::string& expected_domain);

  
  
  
  void ValidateDMToken(const std::string& dm_token,
                       ValidateDMTokenOption dm_token_option);

  
  void ValidatePolicyType(const std::string& policy_type);

  
  void ValidateSettingsEntityId(const std::string& settings_entity_id);

  
  void ValidatePayload();

  
  
  
  
  void ValidateSignature(const std::vector<uint8>& key,
                         bool allow_key_rotation);

  
  
  
  
  
  void ValidateInitialKey();

  
  
  
  
  
  void ValidateAgainstCurrentPolicy(
      const enterprise_management::PolicyData* policy_data,
      ValidateTimestampOption timestamp_option,
      ValidateDMTokenOption dm_token_option);

  
  void RunValidation();

 protected:
  
  
  
  CloudPolicyValidatorBase(
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy_response,
      google::protobuf::MessageLite* payload,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

  
  
  void PostValidationTask(const base::Closure& completion_callback);

 private:
  
  enum ValidationFlags {
    VALIDATE_TIMESTAMP   = 1 << 0,
    VALIDATE_USERNAME    = 1 << 1,
    VALIDATE_DOMAIN      = 1 << 2,
    VALIDATE_TOKEN       = 1 << 3,
    VALIDATE_POLICY_TYPE = 1 << 4,
    VALIDATE_ENTITY_ID   = 1 << 5,
    VALIDATE_PAYLOAD     = 1 << 6,
    VALIDATE_SIGNATURE   = 1 << 7,
    VALIDATE_INITIAL_KEY = 1 << 8,
  };

  
  static void PerformValidation(
      scoped_ptr<CloudPolicyValidatorBase> self,
      scoped_refptr<base::MessageLoopProxy> message_loop,
      const base::Closure& completion_callback);

  
  static void ReportCompletion(scoped_ptr<CloudPolicyValidatorBase> self,
                               const base::Closure& completion_callback);

  
  void RunChecks();

  
  Status CheckTimestamp();
  Status CheckUsername();
  Status CheckDomain();
  Status CheckToken();
  Status CheckPolicyType();
  Status CheckEntityId();
  Status CheckPayload();
  Status CheckSignature();
  Status CheckInitialKey();

  
  static bool VerifySignature(const std::string& data,
                              const std::string& key,
                              const std::string& signature);

  Status status_;
  scoped_ptr<enterprise_management::PolicyFetchResponse> policy_;
  scoped_ptr<enterprise_management::PolicyData> policy_data_;
  google::protobuf::MessageLite* payload_;

  int validation_flags_;
  int64 timestamp_not_before_;
  int64 timestamp_not_after_;
  ValidateTimestampOption timestamp_option_;
  ValidateDMTokenOption dm_token_option_;
  std::string user_;
  std::string domain_;
  std::string token_;
  std::string policy_type_;
  std::string settings_entity_id_;
  std::string key_;
  bool allow_key_rotation_;
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyValidatorBase);
};

template<typename PayloadProto>
class POLICY_EXPORT CloudPolicyValidator : public CloudPolicyValidatorBase {
 public:
  typedef base::Callback<void(CloudPolicyValidator<PayloadProto>*)>
      CompletionCallback;

  virtual ~CloudPolicyValidator() {}

  
  
  
  static CloudPolicyValidator<PayloadProto>* Create(
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy_response,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner) {
    return new CloudPolicyValidator(
        policy_response.Pass(),
        scoped_ptr<PayloadProto>(new PayloadProto()),
        background_task_runner);
  }

  scoped_ptr<PayloadProto>& payload() {
    return payload_;
  }

  
  
  
  
  void StartValidation(const CompletionCallback& completion_callback) {
    PostValidationTask(base::Bind(completion_callback, this));
  }

 private:
  CloudPolicyValidator(
      scoped_ptr<enterprise_management::PolicyFetchResponse> policy_response,
      scoped_ptr<PayloadProto> payload,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner)
      : CloudPolicyValidatorBase(policy_response.Pass(),
                                 payload.get(),
                                 background_task_runner),
        payload_(payload.Pass()) {}

  scoped_ptr<PayloadProto> payload_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyValidator);
};

typedef CloudPolicyValidator<enterprise_management::CloudPolicySettings>
    UserCloudPolicyValidator;

#if !defined(OS_ANDROID)
typedef CloudPolicyValidator<enterprise_management::ExternalPolicyData>
    ComponentCloudPolicyValidator;
#endif

}  

#endif  
