// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_KWALLET_X_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_KWALLET_X_H_
#pragma once

#include <dbus/dbus-glib.h>
#include <glib.h>

#include <string>

#include "base/basictypes.h"
#include "base/time.h"
#include "chrome/browser/password_manager/password_store_x.h"
#include "webkit/glue/password_form.h"

class Pickle;

class NativeBackendKWallet : public PasswordStoreX::NativeBackend {
 public:
  NativeBackendKWallet();

  virtual ~NativeBackendKWallet();

  virtual bool Init();

  
  virtual bool AddLogin(const webkit_glue::PasswordForm& form);
  virtual bool UpdateLogin(const webkit_glue::PasswordForm& form);
  virtual bool RemoveLogin(const webkit_glue::PasswordForm& form);
  virtual bool RemoveLoginsCreatedBetween(const base::Time& delete_begin,
                                          const base::Time& delete_end);
  virtual bool GetLogins(const webkit_glue::PasswordForm& form,
                         PasswordFormList* forms);
  virtual bool GetLoginsCreatedBetween(const base::Time& delete_begin,
                                       const base::Time& delete_end,
                                       PasswordFormList* forms);
  virtual bool GetAutofillableLogins(PasswordFormList* forms);
  virtual bool GetBlacklistLogins(PasswordFormList* forms);

 private:
  
  bool StartKWalletd();
  bool InitWallet();

  
  bool GetLoginsList(PasswordFormList* forms,
                     const std::string& signon_realm,
                     int wallet_handle);

  
  bool GetLoginsList(PasswordFormList* forms,
                     bool autofillable,
                     int wallet_handle);

  
  bool GetLoginsList(PasswordFormList* forms,
                     const base::Time& begin,
                     const base::Time& end,
                     int wallet_handle);

  
  bool GetAllLogins(PasswordFormList* forms, int wallet_handle);

  
  
  
  bool SetLoginsList(const PasswordFormList& forms,
                     const std::string& signon_realm,
                     int wallet_handle);

  
  
  
  bool CheckError();

  
  
  int WalletHandle();

  
  
  
  
  static bool CompareForms(const webkit_glue::PasswordForm& a,
                           const webkit_glue::PasswordForm& b,
                           bool update_check);

  
  static void SerializeValue(const PasswordFormList& forms, Pickle* pickle);

  
  
  static bool CheckSerializedValue(const GArray* byte_array, const char* realm);

  
  static void DeserializeValue(const std::string& signon_realm,
                               const Pickle& pickle,
                               PasswordFormList* forms);

  
  // been written as a std::string. Returns true on success.
  static bool ReadGURL(const Pickle& pickle, void** iter, GURL* url);

  
  
  static const int kPickleVersion = 0;

  
  static const char* kAppId;
  
  static const char* kKWalletFolder;

  
  static const char* kKWalletServiceName;
  static const char* kKWalletPath;
  static const char* kKWalletInterface;
  static const char* kKLauncherServiceName;
  static const char* kKLauncherPath;
  static const char* kKLauncherInterface;

  
  static const int kInvalidKWalletHandle = -1;

  
  
  GError* error_;
  
  DBusGConnection* connection_;
  
  DBusGProxy* proxy_;

  
  std::string wallet_name_;

  DISALLOW_COPY_AND_ASSIGN(NativeBackendKWallet);
};

#endif  
