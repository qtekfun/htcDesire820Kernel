// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_X_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_X_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/password_manager/password_store_default.h"

class LoginDatabase;
class PrefService;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

class PasswordStoreX : public PasswordStoreDefault {
 public:
  
  
  class NativeBackend {
   public:
    typedef std::vector<autofill::PasswordForm*> PasswordFormList;

    virtual ~NativeBackend() {}

    virtual bool Init() = 0;

    virtual bool AddLogin(const autofill::PasswordForm& form) = 0;
    virtual bool UpdateLogin(const autofill::PasswordForm& form) = 0;
    virtual bool RemoveLogin(const autofill::PasswordForm& form) = 0;
    virtual bool RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                            const base::Time& delete_end) = 0;
    virtual bool GetLogins(const autofill::PasswordForm& form,
                           PasswordFormList* forms) = 0;
    virtual bool GetLoginsCreatedBetween(const base::Time& get_begin,
                                         const base::Time& get_end,
                                         PasswordFormList* forms) = 0;
    virtual bool GetAutofillableLogins(PasswordFormList* forms) = 0;
    virtual bool GetBlacklistLogins(PasswordFormList* forms) = 0;
  };

  
  
  PasswordStoreX(LoginDatabase* login_db,
                 Profile* profile,
                 NativeBackend* backend);

#if !defined(OS_MACOSX) && !defined(OS_CHROMEOS) && defined(OS_POSIX)
  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static bool PasswordsUseLocalProfileId(PrefService* prefs);

  
  
  
  static void SetPasswordsUseLocalProfileId(PrefService* prefs);
#endif  

 private:
  friend class PasswordStoreXTest;

  virtual ~PasswordStoreX();

  
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

  
  void SortLoginsByOrigin(NativeBackend::PasswordFormList* list);

  
  void CheckMigration();

  
  bool use_native_backend() { return !!backend_.get(); }

  
  
  bool allow_default_store();

  
  
  
  
  ssize_t MigrateLogins();

  
  scoped_ptr<NativeBackend> backend_;
  
  bool migration_checked_;
  
  
  
  
  bool allow_fallback_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreX);
};

#endif  
