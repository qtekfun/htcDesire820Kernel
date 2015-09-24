// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_PREFERENCES_BROWSERTEST_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_PREFERENCES_BROWSERTEST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_service.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/policy/core/common/mock_configuration_policy_provider.h"
#include "components/policy/core/common/policy_types.h"
#include "content/public/browser/notification_observer.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace content {
class NotificationDetails;
class NotificationSource;
class RenderViewHost;
}

class PreferencesBrowserTest : public InProcessBrowserTest {
 public:
  PreferencesBrowserTest();
  ~PreferencesBrowserTest();

  
  virtual void SetUpOnMainThread() OVERRIDE;

  void OnPreferenceChanged(const std::string& pref_name);

 protected:
  MOCK_METHOD1(OnCommit, void(const PrefService::Preference*));

  void SetUpPrefs();

  
  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;
  virtual void TearDownInProcessBrowserTestFixture() OVERRIDE;

  
  void SetUserPolicies(const std::vector<std::string>& names,
                       const std::vector<base::Value*>& values,
                       policy::PolicyLevel level);
  
  void ClearUserPolicies();
  
  void SetUserValues(const std::vector<std::string>& names,
                     const std::vector<base::Value*>& values);

  
  
  void VerifyKeyValue(const base::DictionaryValue& dict,
                      const std::string& key,
                      const base::Value& expected);
  
  
  void VerifyPref(const base::DictionaryValue* prefs,
                  const std::string& name,
                  const base::Value* value,
                  const std::string& controlledBy,
                  bool disabled,
                  bool uncommitted);
  
  
  
  void VerifyObservedPref(const std::string& observed_json,
                          const std::string& name,
                          const base::Value* value,
                          const std::string& controlledBy,
                          bool disabled,
                          bool uncommitted);
  
  
  
  void VerifyObservedPrefs(const std::string& observed_json,
                           const std::vector<std::string>& names,
                           const std::vector<base::Value*>& values,
                           const std::string& controlledBy,
                           bool disabled,
                           bool uncommitted);

  
  
  void ExpectNoCommit(const std::string& name);
  
  
  void ExpectSetCommit(const std::string& name,
                       const base::Value* value);
  
  
  void ExpectClearCommit(const std::string& name);
  
  void VerifyAndClearExpectations();

  
  void SetupJavaScriptTestEnvironment(
      const std::vector<std::string>& pref_names,
      std::string* observed_json) const;

  
  
  
  void SetPref(const std::string& name,
               const std::string& type,
               const base::Value* value,
               bool commit,
               std::string* observed_json);

  
  
  
  void VerifySetPref(const std::string& name,
                     const std::string& type,
                     const base::Value* value,
                     bool commit);
  
  
  
  void VerifyClearPref(const std::string& name,
                       const base::Value* value,
                       bool commit);
  
  
  
  void VerifyCommit(const std::string& name,
                    const base::Value* value,
                    const std::string& controlledBy);
  
  
  
  void VerifySetCommit(const std::string& name,
                       const base::Value* value);
  
  
  
  void VerifyClearCommit(const std::string& name,
                         const base::Value* value);
  
  
  
  
  void VerifyRollback(const std::string& name,
                      const base::Value* value,
                      const std::string& controlledBy);
  
  
  void StartObserving();
  
  
  
  void FinishObserving(std::string* observed_json);

  
  
  void UseDefaultTestPrefs(bool includeListPref);

  
  
  content::RenderViewHost* render_view_host_;

  
  policy::MockConfigurationPolicyProvider policy_provider_;

  
  
  PrefChangeRegistrar pref_change_registrar_;

  
  PrefService* pref_service_;

  
  std::vector<std::string> types_;
  std::vector<std::string> pref_names_;
  std::vector<std::string> policy_names_;
  std::vector<base::Value*> default_values_;
  std::vector<base::Value*> non_default_values_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PreferencesBrowserTest);
};

#endif  
