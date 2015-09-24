// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_TEST_PASSWORD_STORE_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_TEST_PASSWORD_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "chrome/browser/password_manager/password_store.h"

namespace content {
class BrowserContext;
}

scoped_refptr<RefcountedBrowserContextKeyedService> CreateTestPasswordStore(
    content::BrowserContext* profile);

class TestPasswordStore : public PasswordStore {
 public:
  TestPasswordStore();

  
  
  static scoped_refptr<RefcountedBrowserContextKeyedService> Create(
      content::BrowserContext* profile);

  typedef std::map<std::string ,
                   std::vector<autofill::PasswordForm> > PasswordMap;

  PasswordMap stored_passwords();
  void Clear();

 protected:
  virtual ~TestPasswordStore();

  
  bool FormsAreEquivalent(const autofill::PasswordForm& lhs,
                          const autofill::PasswordForm& rhs);

  
  virtual void AddLoginImpl(const autofill::PasswordForm& form) OVERRIDE;
  virtual void UpdateLoginImpl(const autofill::PasswordForm& form) OVERRIDE;
  virtual void RemoveLoginImpl(const autofill::PasswordForm& form) OVERRIDE;
  virtual void GetLoginsImpl(
      const autofill::PasswordForm& form,
      PasswordStore::AuthorizationPromptPolicy prompt_policy,
      const ConsumerCallbackRunner& runner) OVERRIDE;
  virtual bool ScheduleTask(const base::Closure& task) OVERRIDE;
  virtual void WrapModificationTask(base::Closure task) OVERRIDE;

  
  virtual void ReportMetricsImpl() OVERRIDE {}
  virtual void RemoveLoginsCreatedBetweenImpl(const base::Time& begin,
                                              const base::Time& end) OVERRIDE {}
  virtual void GetAutofillableLoginsImpl(
      PasswordStore::GetLoginsRequest* request) OVERRIDE {}
  virtual void GetBlacklistLoginsImpl(
      PasswordStore::GetLoginsRequest* request) OVERRIDE {}
  virtual bool FillAutofillableLogins(
      std::vector<autofill::PasswordForm*>* forms) OVERRIDE;
  virtual bool FillBlacklistLogins(
      std::vector<autofill::PasswordForm*>* forms) OVERRIDE;
  virtual void ShutdownOnUIThread() OVERRIDE {}

 private:
  PasswordMap stored_passwords_;

  DISALLOW_COPY_AND_ASSIGN(TestPasswordStore);
};

#endif  
