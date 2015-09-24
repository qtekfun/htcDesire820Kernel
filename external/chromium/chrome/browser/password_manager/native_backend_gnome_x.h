// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_GNOME_X_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_GNOME_X_H_
#pragma once

#include "base/basictypes.h"
#include "base/time.h"
#include "chrome/browser/password_manager/password_store_x.h"

namespace webkit_glue {
struct PasswordForm;
}

class NativeBackendGnome : public PasswordStoreX::NativeBackend {
 public:
  NativeBackendGnome();

  virtual ~NativeBackendGnome();

  virtual bool Init();

  
  virtual bool AddLogin(const webkit_glue::PasswordForm& form);
  virtual bool UpdateLogin(const webkit_glue::PasswordForm& form);
  virtual bool RemoveLogin(const webkit_glue::PasswordForm& form);
  virtual bool RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                          const base::Time& delete_end);
  virtual bool GetLogins(const webkit_glue::PasswordForm& form,
                         PasswordFormList* forms);
  virtual bool GetLoginsCreatedBetween(const base::Time& get_begin,
                                       const base::Time& get_end,
                                       PasswordFormList* forms);
  virtual bool GetAutofillableLogins(PasswordFormList* forms);
  virtual bool GetBlacklistLogins(PasswordFormList* forms);

 private:
  
  bool RawAddLogin(const webkit_glue::PasswordForm& form);

  
  bool GetLoginsList(PasswordFormList* forms, bool autofillable);

  
  bool GetAllLogins(PasswordFormList* forms);

  DISALLOW_COPY_AND_ASSIGN(NativeBackendGnome);
};

#endif  
