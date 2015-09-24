// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_H_

#include <vector>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "chrome/browser/password_manager/login_database.h"
#include "chrome/browser/password_manager/password_store.h"

namespace content {
class NotificationService;
}

namespace crypto {
class AppleKeychain;
}

class PasswordStoreMac : public PasswordStore {
 public:
  
  
  PasswordStoreMac(crypto::AppleKeychain* keychain, LoginDatabase* login_db);

  
  virtual bool Init() OVERRIDE;

  virtual void ShutdownOnUIThread() OVERRIDE;

 protected:
  virtual ~PasswordStoreMac();

  virtual bool ScheduleTask(const base::Closure& task) OVERRIDE;

 private:
  virtual void ReportMetricsImpl() OVERRIDE;
  virtual void AddLoginImpl(const autofill::PasswordForm& form) OVERRIDE;
  virtual void UpdateLoginImpl(
      const autofill::PasswordForm& form) OVERRIDE;
  virtual void RemoveLoginImpl(
      const autofill::PasswordForm& form) OVERRIDE;
  virtual void RemoveLoginsCreatedBetweenImpl(
      const base::Time& delete_begin, const base::Time& delete_end) OVERRIDE;
  virtual void GetLoginsImpl(
      const autofill::PasswordForm& form,
      AuthorizationPromptPolicy prompt_policy,
      const ConsumerCallbackRunner& callback_runner) OVERRIDE;
  virtual void GetAutofillableLoginsImpl(GetLoginsRequest* request) OVERRIDE;
  virtual void GetBlacklistLoginsImpl(GetLoginsRequest* request) OVERRIDE;
  virtual bool FillAutofillableLogins(
      std::vector<autofill::PasswordForm*>* forms) OVERRIDE;
  virtual bool FillBlacklistLogins(
      std::vector<autofill::PasswordForm*>* forms) OVERRIDE;

  
  
  
  bool AddToKeychainIfNecessary(const autofill::PasswordForm& form);

  
  
  bool DatabaseHasFormMatchingKeychainForm(
      const autofill::PasswordForm& form);

  
  
  
  std::vector<autofill::PasswordForm*> GetUnusedKeychainForms();

  
  void RemoveDatabaseForms(
      const std::vector<autofill::PasswordForm*>& forms);

  
  void RemoveKeychainForms(
      const std::vector<autofill::PasswordForm*>& forms);

  
  
  void CreateNotificationService();

  scoped_ptr<crypto::AppleKeychain> keychain_;
  scoped_ptr<LoginDatabase> login_metadata_db_;

  
  scoped_ptr<base::Thread> thread_;

  
  
  scoped_ptr<content::NotificationService> notification_service_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreMac);
};

#endif  
