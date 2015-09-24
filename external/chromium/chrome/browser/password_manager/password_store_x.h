// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_X_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_X_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "chrome/browser/password_manager/password_store_default.h"

class LoginDatabase;
class Profile;
class WebDataService;

class PasswordStoreX : public PasswordStoreDefault {
 public:
  
  
  class NativeBackend {
   public:
    typedef std::vector<webkit_glue::PasswordForm*> PasswordFormList;

    virtual ~NativeBackend() {}

    virtual bool Init() = 0;

    virtual bool AddLogin(const webkit_glue::PasswordForm& form) = 0;
    virtual bool UpdateLogin(const webkit_glue::PasswordForm& form) = 0;
    virtual bool RemoveLogin(const webkit_glue::PasswordForm& form) = 0;
    virtual bool RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                            const base::Time& delete_end) = 0;
    virtual bool GetLogins(const webkit_glue::PasswordForm& form,
                           PasswordFormList* forms) = 0;
    virtual bool GetLoginsCreatedBetween(const base::Time& get_begin,
                                         const base::Time& get_end,
                                         PasswordFormList* forms) = 0;
    virtual bool GetAutofillableLogins(PasswordFormList* forms) = 0;
    virtual bool GetBlacklistLogins(PasswordFormList* forms) = 0;
  };

  
  
  PasswordStoreX(LoginDatabase* login_db,
                   Profile* profile,
                   WebDataService* web_data_service,
                   NativeBackend* backend);

 private:
  friend class PasswordStoreXTest;

  virtual ~PasswordStoreX();

  
  virtual void AddLoginImpl(const webkit_glue::PasswordForm& form);
  virtual void UpdateLoginImpl(const webkit_glue::PasswordForm& form);
  virtual void RemoveLoginImpl(const webkit_glue::PasswordForm& form);
  virtual void RemoveLoginsCreatedBetweenImpl(const base::Time& delete_begin,
                                              const base::Time& delete_end);
  virtual void GetLoginsImpl(GetLoginsRequest* request,
                             const webkit_glue::PasswordForm& form);
  virtual void GetAutofillableLoginsImpl(GetLoginsRequest* request);
  virtual void GetBlacklistLoginsImpl(GetLoginsRequest* request);
  virtual bool FillAutofillableLogins(
      std::vector<webkit_glue::PasswordForm*>* forms);
  virtual bool FillBlacklistLogins(
      std::vector<webkit_glue::PasswordForm*>* forms);

  
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
