// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef GOOGLE_APIS_GAIA_GOOGLE_SERVICE_AUTH_ERROR_H_
#define GOOGLE_APIS_GAIA_GOOGLE_SERVICE_AUTH_ERROR_H_

#include <string>

#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

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

    
    
    UNEXPECTED_SERVICE_RESPONSE = 11,

    
    
    SERVICE_ERROR = 12,

    
    NUM_STATES = 13,
  };

  
  struct Captcha {
    Captcha();
    Captcha(const std::string& token,
            const GURL& audio,
            const GURL& img,
            const GURL& unlock,
            int width,
            int height);
    ~Captcha();
    
    bool operator==(const Captcha &b) const;

    std::string token;  
    GURL audio_url;     
    GURL image_url;     
    GURL unlock_url;    
    int image_width;    
    int image_height;   
  };

  
  struct SecondFactor {
    SecondFactor();
    SecondFactor(const std::string& token,
                 const std::string& prompt,
                 const std::string& alternate,
                 int length);
    ~SecondFactor();
    
    bool operator==(const SecondFactor &b) const;

    
    std::string token;
    
    
    std::string prompt_text;
    
    
    std::string alternate_text;
    
    int field_length;
  };

  
  bool operator==(const GoogleServiceAuthError &b) const;

  
  explicit GoogleServiceAuthError(State s);

  
  
  static GoogleServiceAuthError FromConnectionError(int error);

  
  
  
  
  static GoogleServiceAuthError FromClientLoginCaptchaChallenge(
      const std::string& captcha_token,
      const GURL& captcha_image_url,
      const GURL& captcha_unlock_url);

  
  
  static GoogleServiceAuthError FromServiceError(
      const std::string& error_message);

  
  
  static GoogleServiceAuthError FromUnexpectedServiceResponse(
      const std::string& error_message);

  
  
  
  static GoogleServiceAuthError AuthErrorNone();

  
  State state() const;
  const Captcha& captcha() const;
  const SecondFactor& second_factor() const;
  int network_error() const;
  const std::string& token() const;
  const std::string& error_message() const;

  
  
  base::DictionaryValue* ToValue() const;

  
  std::string ToString() const;

 private:
  GoogleServiceAuthError(State s, int error);

  
  GoogleServiceAuthError(State state, const std::string& error_message);

  explicit GoogleServiceAuthError(const std::string& error_message);

  GoogleServiceAuthError(State s, const std::string& captcha_token,
                         const GURL& captcha_audio_url,
                         const GURL& captcha_image_url,
                         const GURL& captcha_unlock_url,
                         int image_width,
                         int image_height);

  GoogleServiceAuthError(State s, const std::string& captcha_token,
                         const std::string& prompt_text,
                         const std::string& alternate_text,
                         int field_length);

  State state_;
  Captcha captcha_;
  SecondFactor second_factor_;
  int network_error_;
  std::string error_message_;
};

#endif  
