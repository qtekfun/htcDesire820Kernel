// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/http/http_auth.h"

namespace net {

struct HttpRequestInfo;

class HttpAuthHandler {
 public:
  HttpAuthHandler();
  virtual ~HttpAuthHandler();

  
  
  
  
  
  bool InitFromChallenge(HttpAuth::ChallengeTokenizer* challenge,
                         HttpAuth::Target target,
                         const GURL& origin,
                         const BoundNetLog& net_log);

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual HttpAuth::AuthorizationResult HandleAnotherChallenge(
      HttpAuth::ChallengeTokenizer* challenge) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int GenerateAuthToken(const string16* username,
                        const string16* password,
                        const HttpRequestInfo* request,
                        CompletionCallback* callback,
                        std::string* auth_token);

  
  HttpAuth::Scheme auth_scheme() const {
    return auth_scheme_;
  }

  
  const std::string& realm() const {
    return realm_;
  }

  
  const std::string challenge() const {
    return auth_challenge_;
  }

  
  
  int score() const {
    return score_;
  }

  HttpAuth::Target target() const {
    return target_;
  }

  
  
  
  const GURL& origin() const {
    return origin_;
  }

  
  
  bool encrypts_identity() const {
    return (properties_ & ENCRYPTS_IDENTITY) != 0;
  }

  
  
  
  
  bool is_connection_based() const {
    return (properties_ & IS_CONNECTION_BASED) != 0;
  }

  
  
  
  
  virtual bool NeedsIdentity();

  
  
  
  
  
  virtual bool AllowsDefaultCredentials();

 protected:
  enum Property {
    ENCRYPTS_IDENTITY = 1 << 0,
    IS_CONNECTION_BASED = 1 << 1,
  };

  
  
  
  
  
  
  virtual bool Init(HttpAuth::ChallengeTokenizer* challenge) = 0;

  
  
  
  virtual int GenerateAuthTokenImpl(const string16* username,
                                    const string16* password,
                                    const HttpRequestInfo* request,
                                    CompletionCallback* callback,
                                    std::string* auth_token) = 0;

  
  HttpAuth::Scheme auth_scheme_;

  
  std::string realm_;

  
  std::string auth_challenge_;

  
  
  GURL origin_;

  
  int score_;

  
  
  HttpAuth::Target target_;

  
  int properties_;

  BoundNetLog net_log_;

 private:
  void OnGenerateAuthTokenComplete(int rv);
  void FinishGenerateAuthToken();

  CompletionCallback* original_callback_;
  CompletionCallbackImpl<HttpAuthHandler> wrapper_callback_;
};

}  

#endif  
