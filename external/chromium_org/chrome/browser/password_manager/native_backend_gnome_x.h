// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_GNOME_X_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_GNOME_X_H_

#include <gnome-keyring.h>

#include <string>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "chrome/browser/password_manager/password_store_factory.h"
#include "chrome/browser/password_manager/password_store_x.h"
#include "chrome/browser/profiles/profile.h"

class PrefService;

namespace autofill {
struct PasswordForm;
}

class GnomeKeyringLoader {
 protected:
  static bool LoadGnomeKeyring();

#define GNOME_KEYRING_FOR_EACH_FUNC(F)          \
  F(is_available)                               \
  F(store_password)                             \
  F(delete_password)                            \
  F(find_itemsv)                                \
  F(result_to_message)

#define GNOME_KEYRING_DECLARE_POINTER(name) \
    static typeof(&::gnome_keyring_##name) gnome_keyring_##name;
  GNOME_KEYRING_FOR_EACH_FUNC(GNOME_KEYRING_DECLARE_POINTER)
#undef GNOME_KEYRING_DECLARE_POINTER

  
  static bool keyring_loaded;

 private:
#if defined(DLOPEN_GNOME_KEYRING)
  struct FunctionInfo {
    const char* name;
    void** pointer;
  };

  
  static const FunctionInfo functions[];
#endif  
};

class NativeBackendGnome : public PasswordStoreX::NativeBackend,
                           public GnomeKeyringLoader {
 public:
  NativeBackendGnome(LocalProfileId id, PrefService* prefs);

  virtual ~NativeBackendGnome();

  virtual bool Init() OVERRIDE;

  
  virtual bool AddLogin(const autofill::PasswordForm& form) OVERRIDE;
  virtual bool UpdateLogin(const autofill::PasswordForm& form) OVERRIDE;
  virtual bool RemoveLogin(const autofill::PasswordForm& form) OVERRIDE;
  virtual bool RemoveLoginsCreatedBetween(
      const base::Time& delete_begin, const base::Time& delete_end) OVERRIDE;
  virtual bool GetLogins(const autofill::PasswordForm& form,
                         PasswordFormList* forms) OVERRIDE;
  virtual bool GetLoginsCreatedBetween(const base::Time& get_begin,
                                       const base::Time& get_end,
                                       PasswordFormList* forms) OVERRIDE;
  virtual bool GetAutofillableLogins(PasswordFormList* forms) OVERRIDE;
  virtual bool GetBlacklistLogins(PasswordFormList* forms) OVERRIDE;

 private:
  
  bool RawAddLogin(const autofill::PasswordForm& form);

  
  bool GetLoginsList(PasswordFormList* forms, bool autofillable);

  
  bool GetAllLogins(PasswordFormList* forms);

  
  std::string GetProfileSpecificAppString() const;

  
  void MigrateToProfileSpecificLogins();

  
  const LocalProfileId profile_id_;

  
  PrefService* prefs_;

  
  std::string app_string_;

  
  bool migrate_tried_;

  DISALLOW_COPY_AND_ASSIGN(NativeBackendGnome);
};

#endif  
