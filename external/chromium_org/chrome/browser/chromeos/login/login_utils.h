// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_UTILS_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_UTILS_H_

#include <string>

#include "base/memory/ref_counted.h"

class CommandLine;
class GURL;
class PrefRegistrySimple;
class PrefService;
class Profile;

namespace chromeos {

class Authenticator;
class LoginDisplayHost;
class LoginStatusConsumer;
struct UserContext;

class LoginUtils {
 public:
  class Delegate {
   public:
    
    virtual void OnProfilePrepared(Profile* profile) = 0;

#if defined(ENABLE_RLZ)
    
    virtual void OnRlzInitialized(Profile* profile) {}
#endif
   protected:
    virtual ~Delegate() {}
  };

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  static LoginUtils* Get();

  
  static void Set(LoginUtils* ptr);

  
  
  
  static bool IsWhitelisted(const std::string& username, bool* wildcard_match);

  virtual ~LoginUtils() {}

  
  
  
  virtual void DoBrowserLaunch(Profile* profile,
                               LoginDisplayHost* login_host) = 0;

  
  
  
  
  
  
  
  
  virtual void PrepareProfile(
      const UserContext& user_context,
      const std::string& display_email,
      bool has_cookies,
      bool has_active_session,
      Delegate* delegate) = 0;

  
  virtual void DelegateDeleted(Delegate* delegate) = 0;

  
  
  
  virtual void CompleteOffTheRecordLogin(const GURL& start_url) = 0;

  
  
  virtual void SetFirstLoginPrefs(PrefService* prefs) = 0;

  
  
  
  
  
  
  
  
  
  virtual scoped_refptr<Authenticator> CreateAuthenticator(
      LoginStatusConsumer* consumer) = 0;

  
  virtual void RestoreAuthenticationSession(Profile* profile) = 0;

  
  virtual void InitRlzDelayed(Profile* user_profile) = 0;
};

}  

#endif  
