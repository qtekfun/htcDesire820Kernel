// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/password_manager/password_store_default.h"

class LoginDatabase;
class Profile;
class WebDataService;

namespace autofill {
struct PasswordForm;
}

class PasswordStoreWin : public PasswordStoreDefault {
 public:
  
  PasswordStoreWin(LoginDatabase* login_database,
                   Profile* profile,
                   WebDataService* web_data_service);

  
  virtual void ShutdownOnUIThread() OVERRIDE;

 private:
  class DBHandler;

  virtual ~PasswordStoreWin();

  
  void ShutdownOnDBThread();

  virtual void GetLoginsImpl(
      const autofill::PasswordForm& form,
      AuthorizationPromptPolicy prompt_policy,
      const ConsumerCallbackRunner& callback_runner) OVERRIDE;

  void GetIE7LoginIfNecessary(
    const autofill::PasswordForm& form,
    const ConsumerCallbackRunner& callback_runner,
    const std::vector<autofill::PasswordForm*>& matched_forms);

  scoped_ptr<DBHandler> db_handler_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreWin);
};

#endif  