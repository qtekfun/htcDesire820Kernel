// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_KWALLET_X_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_NATIVE_BACKEND_KWALLET_X_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "chrome/browser/password_manager/password_store_factory.h"
#include "chrome/browser/password_manager/password_store_x.h"
#include "chrome/browser/profiles/profile.h"

class Pickle;
class PickleIterator;
class PrefService;

namespace autofill {
struct PasswordForm;
}

namespace base {
class WaitableEvent;
}

namespace dbus {
class Bus;
class ObjectProxy;
}

class NativeBackendKWallet : public PasswordStoreX::NativeBackend {
 public:
  NativeBackendKWallet(LocalProfileId id, PrefService* prefs);

  virtual ~NativeBackendKWallet();

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

 protected:
  
  static const int kInvalidKWalletHandle = -1;

  
  bool InitWithBus(scoped_refptr<dbus::Bus> optional_bus);

  
  static void DeserializeValue(const std::string& signon_realm,
                               const Pickle& pickle,
                               PasswordFormList* forms);

 private:
  enum InitResult {
    INIT_SUCCESS,    
    TEMPORARY_FAIL,  
    PERMANENT_FAIL   
  };

  
  bool StartKWalletd();
  InitResult InitWallet();
  void InitOnDBThread(scoped_refptr<dbus::Bus> optional_bus,
                      base::WaitableEvent* event,
                      bool* success);

  
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

  
  
  int WalletHandle();

  
  static void SerializeValue(const PasswordFormList& forms, Pickle* pickle);

  
  
  
  
  
  
  static bool DeserializeValueSize(const std::string& signon_realm,
                                   const PickleIterator& iter,
                                   bool size_32, bool warn_only,
                                   PasswordFormList* forms);

  
  
  static const int kPickleVersion = 1;

  
  std::string GetProfileSpecificFolderName() const;

  
  void MigrateToProfileSpecificLogins();

  
  const LocalProfileId profile_id_;

  
  PrefService* prefs_;

  
  std::string folder_name_;

  
  bool migrate_tried_;

  
  scoped_refptr<dbus::Bus> session_bus_;
  
  dbus::ObjectProxy* kwallet_proxy_;

  
  std::string wallet_name_;
  
  const std::string app_name_;

  DISALLOW_COPY_AND_ASSIGN(NativeBackendKWallet);
};

#endif  
