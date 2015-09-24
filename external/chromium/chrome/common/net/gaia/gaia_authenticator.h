// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_GAIA_GAIA_AUTHENTICATOR_H_
#define CHROME_COMMON_NET_GAIA_GAIA_AUTHENTICATOR_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/message_loop.h"
#include "chrome/common/deprecated/event_sys.h"
#include "googleurl/src/gurl.h"

namespace gaia {

static const char kGaiaUrl[] =
    "https://www.google.com:443/accounts/ClientLogin";

enum AuthenticationError {
  None                      = 0,
  BadAuthentication         = 1,
  NotVerified               = 2,
  TermsNotAgreed            = 3,
  Unknown                   = 4,
  AccountDeleted            = 5,
  AccountDisabled           = 6,
  CaptchaRequired           = 7,
  ServiceUnavailable        = 8,
  
  CredentialsNotSet         = 9,
  ConnectionUnavailable     = 10
};

class GaiaAuthenticator;

struct GaiaAuthEvent {
  enum {
    GAIA_AUTH_FAILED,
    GAIA_AUTH_SUCCEEDED,
    GAIA_AUTHENTICATOR_DESTROYED
  }
  what_happened;
  AuthenticationError error;
  const GaiaAuthenticator* authenticator;

  
  typedef GaiaAuthEvent EventType;
  static inline bool IsChannelShutdownEvent(const GaiaAuthEvent& event) {
    return event.what_happened == GAIA_AUTHENTICATOR_DESTROYED;
  }
};

class GaiaAuthenticator {
  FRIEND_TEST_ALL_PREFIXES(GaiaAuthenticatorTest,
                           TestNewlineAtEndOfAuthTokenRemoved);
 public:

  
  
  
  
  
  GaiaAuthenticator(const std::string& user_agent,
                    const std::string& service_id,
                    const std::string& gaia_url);

  virtual ~GaiaAuthenticator();

  
  
  void set_message_loop(const MessageLoop* loop) {
    message_loop_ = loop;
  }

  
  
  
  
  bool Authenticate(const std::string& user_name, const std::string& password,
                    const std::string& captcha_token,
                    const std::string& captcha_value);

  bool Authenticate(const std::string& user_name, const std::string& password);

  
  
  
  
  bool AuthenticateWithLsid(const std::string& lsid);

  
  void ResetCredentials();

  void SetUsernamePassword(const std::string& username,
                           const std::string& password);

  void SetUsername(const std::string& username);

  
  virtual void RenewAuthToken(const std::string& auth_token);
  void SetAuthToken(const std::string& auth_token);

  struct AuthResults {
    AuthResults();
    ~AuthResults();

    std::string email;
    std::string password;

    
    std::string sid;
    std::string lsid;
    std::string auth_token;

    std::string primary_email;

    
    std::string error_msg;
    enum AuthenticationError auth_error;
    std::string auth_error_url;
    std::string captcha_token;
    std::string captcha_url;
  };

 protected:

  struct AuthParams {
    AuthParams();
    ~AuthParams();

    GaiaAuthenticator* authenticator;
    uint32 request_id;
    std::string email;
    std::string password;
    std::string captcha_token;
    std::string captcha_value;
  };

  
  AuthParams MakeParams(const std::string& user_name,
                        const std::string& password,
                        const std::string& captcha_token,
                        const std::string& captcha_value);

  
  bool AuthenticateImpl(const AuthParams& params);
  bool AuthenticateImpl(const AuthParams& params, AuthResults* results);

  
  virtual bool PerformGaiaRequest(const AuthParams& params,
                                  AuthResults* results);
  virtual bool Post(const GURL& url, const std::string& post_body,
                    unsigned long* response_code, std::string* response_body);

  
  
  virtual bool LookupEmail(AuthResults* results);

  
  
  
  
  
  virtual int GetBackoffDelaySeconds(int current_backoff_delay);

 public:
  
  inline std::string email() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.email;
  }

  
  inline std::string password() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.password;
  }

  
  inline std::string auth_token() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.auth_token;
  }

  
  inline std::string sid() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.sid;
  }

  
  inline std::string lsid() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.lsid;
  }

  
  inline enum AuthenticationError auth_error() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.auth_error;
  }

  inline std::string auth_error_url() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.auth_error_url;
  }

  inline std::string captcha_token() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.captcha_token;
  }

  inline std::string captcha_url() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_.captcha_url;
  }

  inline AuthResults results() const {
    DCHECK_EQ(MessageLoop::current(), message_loop_);
    return auth_results_;
  }

  typedef EventChannel<GaiaAuthEvent, base::Lock> Channel;

  inline Channel* channel() const {
    return channel_;
  }

 private:
  bool IssueAuthToken(AuthResults* results, const std::string& service_id);

  
  void ExtractTokensFrom(const std::string& response, AuthResults* results);
  
  void ExtractAuthErrorFrom(const std::string& response, AuthResults* results);

  
  const std::string user_agent_;
  const std::string service_id_;
  const std::string gaia_url_;

  AuthResults auth_results_;

  
  
  
  // Note that even though this code was written to handle multiple requests
  
  uint32 request_count_;

  Channel* channel_;

  
  int delay_;  
  
  
  
  
  
#if defined(OS_WIN)
  __time32_t next_allowed_auth_attempt_time_;
#else  
  time_t next_allowed_auth_attempt_time_;
#endif  
  int early_auth_attempt_count_;

  
  const MessageLoop* message_loop_;
};

}  
#endif  

