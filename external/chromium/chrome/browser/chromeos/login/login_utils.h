// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_UTILS_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_UTILS_H_
#pragma once

#include <string>

#include "chrome/common/net/gaia/gaia_auth_consumer.h"

class CommandLine;
class GURL;
class Profile;
class PrefService;

namespace {
class BrowserGuestSessionNavigatorTest;
}  

namespace chromeos {

class Authenticator;
class BackgroundView;
class LoginStatusConsumer;

class LoginUtils {
 public:
  class Delegate {
   public:
    
    virtual void OnProfilePrepared(Profile* profile) = 0;
  };

  
  
  static LoginUtils* Get();

  
  static void Set(LoginUtils* ptr);

  
  
  static void DoBrowserLaunch(Profile* profile);

  virtual ~LoginUtils() {}

  
  
  virtual void PrepareProfile(
      const std::string& username,
      const std::string& password,
      const GaiaAuthConsumer::ClientLoginResult& credentials,
      bool pending_requests,
      Delegate* delegate) = 0;

  
  
  
  virtual void CompleteOffTheRecordLogin(const GURL& start_url) = 0;

  
  
  virtual void SetFirstLoginPrefs(PrefService* prefs) = 0;

  
  
  virtual Authenticator* CreateAuthenticator(LoginStatusConsumer* consumer) = 0;

  
  virtual void PrewarmAuthentication() = 0;

  
  
  virtual void FetchCookies(
      Profile* profile,
      const GaiaAuthConsumer::ClientLoginResult& credentials) = 0;

  
  virtual void FetchTokens(
      Profile* profile,
      const GaiaAuthConsumer::ClientLoginResult& credentials) = 0;

  
  virtual void SetBackgroundView(BackgroundView* background_view) = 0;

  
  virtual BackgroundView* GetBackgroundView() = 0;

 protected:
  friend class ::BrowserGuestSessionNavigatorTest;

  
  
  virtual std::string GetOffTheRecordCommandLine(
      const GURL& start_url,
      const CommandLine& base_command_line,
      CommandLine* command_line) = 0;
};

}  

#endif  
