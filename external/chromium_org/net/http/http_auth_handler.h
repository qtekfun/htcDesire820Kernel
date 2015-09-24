// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_H_

#include <string>

#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/http/http_auth.h"

namespace net {

struct HttpRequestInfo;

class NET_EXPORT_PRIVATE HttpAuthHandler {
 public:
  HttpAuthHandler();
  virtual ~HttpAuthHandler();

  
  
  
  
  
  bool InitFromChallenge(HttpAuth::ChallengeTokenizer* challenge,
                         HttpAuth::Target target,
                         const GURL& origin,
                         const BoundNetLog& net_log);

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual HttpAuth::AuthorizationResult HandleAnotherChallenge(
      HttpAuth::ChallengeTokenizer* challenge) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int GenerateAuthToken(const AuthCredentials* credentials,
                        const HttpRequestInfo* request,
                        const CompletionCallback& callback,
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

  
  
  
  virtual bool AllowsExplicitCredentials();

 protected:
  enum Property {
    ENCRYPTS_IDENTITY = 1 << 0,
    IS_CONNECTION_BASED = 1 << 1,
  };

  
  
  
  
  
  
  virtual bool Init(HttpAuth::ChallengeTokenizer* challenge) = 0;

  
  
  
  virtual int GenerateAuthTokenImpl(const AuthCredentials* credentials,
                                    const HttpRequestInfo* request,
                                    const CompletionCallback& callback,
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

  CompletionCallback callback_;
};

}  

#endif  
