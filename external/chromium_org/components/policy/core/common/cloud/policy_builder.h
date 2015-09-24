// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_POLICY_BUILDER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_POLICY_BUILDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "crypto/rsa_private_key.h"
#include "policy/proto/cloud_policy.pb.h"
#include "policy/proto/device_management_backend.pb.h"

#if !defined(OS_ANDROID)
#include "policy/proto/chrome_extension_policy.pb.h"
#endif

namespace policy {

class PolicyBuilder {
 public:
  
  static const char kFakeDeviceId[];
  static const char kFakeDomain[];
  static const char kFakeMachineName[];
  static const char kFakePolicyType[];
  static const int kFakePublicKeyVersion;
  static const int64 kFakeTimestamp;
  static const char kFakeToken[];
  static const char kFakeUsername[];
  static const char kFakeServiceAccountIdentity[];

  
  
  PolicyBuilder();
  virtual ~PolicyBuilder();

  
  enterprise_management::PolicyData& policy_data() {
    if (!policy_data_.get())
      policy_data_.reset(new enterprise_management::PolicyData());
    return *policy_data_;
  }
  void clear_policy_data() {
    policy_data_.reset();
  }

  enterprise_management::PolicyFetchResponse& policy() {
    return policy_;
  }

  scoped_ptr<crypto::RSAPrivateKey> GetSigningKey();
  void SetSigningKey(const crypto::RSAPrivateKey& key);
  void SetDefaultSigningKey();
  void UnsetSigningKey();

  scoped_ptr<crypto::RSAPrivateKey> GetNewSigningKey();
  void SetDefaultNewSigningKey();
  void UnsetNewSigningKey();

  
  
  virtual void Build();

  
  scoped_ptr<enterprise_management::PolicyFetchResponse> GetCopy();

  
  std::string GetBlob();

  
  static scoped_ptr<crypto::RSAPrivateKey> CreateTestSigningKey();
  static scoped_ptr<crypto::RSAPrivateKey> CreateTestOtherSigningKey();

 private:
  
  void SignData(const std::string& data,
                crypto::RSAPrivateKey* key,
                std::string* signature);

  enterprise_management::PolicyFetchResponse policy_;
  scoped_ptr<enterprise_management::PolicyData> policy_data_;
  std::string payload_data_;

  
  
  
  
  
  
  std::vector<uint8> raw_signing_key_;
  std::vector<uint8> raw_new_signing_key_;

  DISALLOW_COPY_AND_ASSIGN(PolicyBuilder);
};

template<typename PayloadProto>
class TypedPolicyBuilder : public PolicyBuilder {
 public:
  TypedPolicyBuilder();
  virtual ~TypedPolicyBuilder() {}

  
  PayloadProto& payload() {
    if (!payload_.get())
      payload_.reset(new PayloadProto());
    return *payload_;
  }
  void clear_payload() {
    payload_.reset();
  }

  
  virtual void Build() OVERRIDE {
    if (payload_.get())
      CHECK(payload_->SerializeToString(policy_data().mutable_policy_value()));

    PolicyBuilder::Build();
  }

 private:
  scoped_ptr<PayloadProto> payload_;

  DISALLOW_COPY_AND_ASSIGN(TypedPolicyBuilder);
};

typedef TypedPolicyBuilder<enterprise_management::CloudPolicySettings>
    UserPolicyBuilder;

#if !defined(OS_ANDROID)
typedef TypedPolicyBuilder<enterprise_management::ExternalPolicyData>
    ComponentPolicyBuilder;
#endif

}  

#endif  
