// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_BASE_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_BASE_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/signin/signin_internals_util.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class CookieSettings;
class ProfileIOData;
class PrefService;

struct GoogleServiceSigninSuccessDetails {
  GoogleServiceSigninSuccessDetails(const std::string& in_username,
                                    const std::string& in_password)
      : username(in_username),
        password(in_password) {}
  std::string username;
  std::string password;
};

struct GoogleServiceSignoutDetails {
  explicit GoogleServiceSignoutDetails(const std::string& in_username)
      : username(in_username) {}
  std::string username;
};

class SigninManagerBase : public BrowserContextKeyedService {
 public:
  SigninManagerBase();
  virtual ~SigninManagerBase();

  
  virtual void Initialize(Profile* profile, PrefService* local_state);
  bool IsInitialized() const;

  
  
  
  
  
  
  virtual bool IsSigninAllowed() const;

  
  
  
  const std::string& GetAuthenticatedUsername() const;

  
  
  
  
  void SetAuthenticatedUsername(const std::string& username);

  
  virtual bool AuthInProgress() const;

  
  virtual void Shutdown() OVERRIDE;

    
  void AddSigninDiagnosticsObserver(
      signin_internals_util::SigninDiagnosticsObserver* observer);
  void RemoveSigninDiagnosticsObserver(
      signin_internals_util::SigninDiagnosticsObserver* observer);

 protected:
  
  
  
  void clear_authenticated_username();

  
  
  Profile* profile_;

  
  void NotifyDiagnosticsObservers(
      const signin_internals_util::UntimedSigninStatusField& field,
      const std::string& value);
  void NotifyDiagnosticsObservers(
      const signin_internals_util::TimedSigninStatusField& field,
      const std::string& value);

 private:
  friend class FakeSigninManagerBase;
  friend class FakeSigninManager;

  
  std::string authenticated_username_;

  
  ObserverList<signin_internals_util::SigninDiagnosticsObserver, true>
      signin_diagnostics_observers_;

  base::WeakPtrFactory<SigninManagerBase> weak_pointer_factory_;

  DISALLOW_COPY_AND_ASSIGN(SigninManagerBase);
};

#endif  
