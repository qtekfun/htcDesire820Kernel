// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CONFIGURATION_POLICY_PROVIDER_TEST_H_
#define COMPONENTS_POLICY_CORE_COMMON_CONFIGURATION_POLICY_PROVIDER_TEST_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "components/policy/core/common/policy_types.h"
#include "components/policy/core/common/schema.h"
#include "components/policy/core/common/schema_registry.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
class DictionaryValue;
class ListValue;
class SequencedTaskRunner;
class Value;
}

namespace policy {

class ConfigurationPolicyProvider;

namespace test_keys {

extern const char kKeyString[];
extern const char kKeyBoolean[];
extern const char kKeyInteger[];
extern const char kKeyStringList[];
extern const char kKeyDictionary[];

}  

class PolicyTestBase : public testing::Test {
 public:
  PolicyTestBase();
  virtual ~PolicyTestBase();

  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

 protected:
  bool RegisterSchema(const PolicyNamespace& ns,
                      const std::string& schema);

  SchemaRegistry schema_registry_;

  
  base::MessageLoopForIO loop_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PolicyTestBase);
};

class PolicyProviderTestHarness {
 public:
  
  
  PolicyProviderTestHarness(PolicyLevel level, PolicyScope scope);
  virtual ~PolicyProviderTestHarness();

  
  virtual void SetUp() = 0;

  
  virtual ConfigurationPolicyProvider* CreateProvider(
      SchemaRegistry* registry,
      scoped_refptr<base::SequencedTaskRunner> task_runner) = 0;

  
  PolicyLevel policy_level() const;
  PolicyScope policy_scope() const;

  
  virtual void InstallEmptyPolicy() = 0;
  virtual void InstallStringPolicy(const std::string& policy_name,
                                   const std::string& policy_value) = 0;
  virtual void InstallIntegerPolicy(const std::string& policy_name,
                                    int policy_value) = 0;
  virtual void InstallBooleanPolicy(const std::string& policy_name,
                                    bool policy_value) = 0;
  virtual void InstallStringListPolicy(const std::string& policy_name,
                                       const base::ListValue* policy_value) = 0;
  virtual void InstallDictionaryPolicy(
      const std::string& policy_name,
      const base::DictionaryValue* policy_value) = 0;

  
  
  virtual void Install3rdPartyPolicy(const base::DictionaryValue* policies);

 private:
  PolicyLevel level_;
  PolicyScope scope_;

  DISALLOW_COPY_AND_ASSIGN(PolicyProviderTestHarness);
};

typedef PolicyProviderTestHarness* (*CreatePolicyProviderTestHarness)();

class ConfigurationPolicyProviderTest
    : public PolicyTestBase,
      public testing::WithParamInterface<CreatePolicyProviderTestHarness> {
 protected:
  ConfigurationPolicyProviderTest();
  virtual ~ConfigurationPolicyProviderTest();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  
  void CheckValue(const char* policy_name,
                  const base::Value& expected_value,
                  base::Closure install_value);

  scoped_ptr<PolicyProviderTestHarness> test_harness_;
  scoped_ptr<ConfigurationPolicyProvider> provider_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyProviderTest);
};

class Configuration3rdPartyPolicyProviderTest
    : public ConfigurationPolicyProviderTest {
 protected:
  Configuration3rdPartyPolicyProviderTest();
  virtual ~Configuration3rdPartyPolicyProviderTest();

 private:
  DISALLOW_COPY_AND_ASSIGN(Configuration3rdPartyPolicyProviderTest);
};

}  

#endif  
