// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef CHROME_COMMON_NET_GAIA_GOOGLE_SERVICE_AUTH_ERROR_H_
#define CHROME_COMMON_NET_GAIA_GOOGLE_SERVICE_AUTH_ERROR_H_
#pragma once

#include <string>

#include "googleurl/src/gurl.h"

class DictionaryValue;

class GoogleServiceAuthError {
 public:
  
  
  
  
  enum State {
    
    NONE = 0,

    
    
    INVALID_GAIA_CREDENTIALS = 1,

    
    USER_NOT_SIGNED_UP = 2,

    
    
    
    CONNECTION_FAILED = 3,

    
    
    
    
    CAPTCHA_REQUIRED = 4,

    
    ACCOUNT_DELETED = 5,

    
    ACCOUNT_DISABLED = 6,

    
    SERVICE_UNAVAILABLE = 7,

    
    TWO_FACTOR = 8,

    
    
    REQUEST_CANCELED = 9,

    
    
    HOSTED_NOT_ALLOWED = 10,
  };

  
  struct Captcha {
    Captcha(const std::string& t, const GURL& img, const GURL& unlock);
    std::string token;  
    GURL image_url;     
    GURL unlock_url;    
  };

  
  bool operator==(const GoogleServiceAuthError &b) const;

  
  explicit GoogleServiceAuthError(State s);

  
  
  static GoogleServiceAuthError FromConnectionError(int error);

  
  static GoogleServiceAuthError FromCaptchaChallenge(
      const std::string& captcha_token,
      const GURL& captcha_image_url,
      const GURL& captcha_unlock_url);

  
  
  
  static GoogleServiceAuthError None();

  
  const State& state() const;
  const Captcha& captcha() const;
  int network_error() const;

  
  
  DictionaryValue* ToValue() const;

 private:
  GoogleServiceAuthError(State s, int error);

  GoogleServiceAuthError(State s, const std::string& captcha_token,
                         const GURL& captcha_image_url,
                         const GURL& captcha_unlock_url);

  State state_;
  Captcha captcha_;
  int network_error_;
};

#endif  
