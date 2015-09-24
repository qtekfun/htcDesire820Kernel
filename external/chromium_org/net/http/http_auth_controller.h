// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_CONTROLLER_H_
#define NET_HTTP_HTTP_AUTH_CONTROLLER_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/http/http_auth.h"
#include "url/gurl.h"

namespace net {

class AuthChallengeInfo;
class AuthCredentials;
class HttpAuthHandler;
class HttpAuthHandlerFactory;
class HttpAuthCache;
class HttpRequestHeaders;
struct HttpRequestInfo;

class NET_EXPORT_PRIVATE HttpAuthController
    : public base::RefCounted<HttpAuthController>,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  HttpAuthController(HttpAuth::Target target,
                     const GURL& auth_url,
                     HttpAuthCache* http_auth_cache,
                     HttpAuthHandlerFactory* http_auth_handler_factory);

  
  
  
  
  virtual int MaybeGenerateAuthToken(const HttpRequestInfo* request,
                                     const CompletionCallback& callback,
                                     const BoundNetLog& net_log);

  
  
  virtual void AddAuthorizationHeader(
      HttpRequestHeaders* authorization_headers);

  
  
  
  virtual int HandleAuthChallenge(scoped_refptr<HttpResponseHeaders> headers,
                                  bool do_not_send_server_auth,
                                  bool establishing_tunnel,
                                  const BoundNetLog& net_log);

  
  virtual void ResetAuth(const AuthCredentials& credentials);

  virtual bool HaveAuthHandler() const;

  virtual bool HaveAuth() const;

  virtual scoped_refptr<AuthChallengeInfo> auth_info();

  virtual bool IsAuthSchemeDisabled(HttpAuth::Scheme scheme) const;
  virtual void DisableAuthScheme(HttpAuth::Scheme scheme);
  virtual void DisableEmbeddedIdentity();

 private:
  
  enum InvalidateHandlerAction {
    INVALIDATE_HANDLER_AND_CACHED_CREDENTIALS,
    INVALIDATE_HANDLER_AND_DISABLE_SCHEME,
    INVALIDATE_HANDLER
  };

  
  friend class base::RefCounted<HttpAuthController>;

  virtual ~HttpAuthController();

  
  
  
  bool SelectPreemptiveAuth(const BoundNetLog& net_log);

  
  
  
  void InvalidateCurrentHandler(InvalidateHandlerAction action);

  
  
  void InvalidateRejectedAuthFromCache();

  
  
  
  bool SelectNextAuthIdentityToTry();

  
  
  void PopulateAuthChallenge();

  
  
  
  bool DisableOnAuthHandlerResult(int result);

  void OnIOComplete(int result);

  
  HttpAuth::Target target_;

  
  const GURL auth_url_;

  
  const GURL auth_origin_;

  
  
  const std::string auth_path_;

  
  
  
  scoped_ptr<HttpAuthHandler> handler_;

  
  
  
  HttpAuth::Identity identity_;

  
  
  std::string auth_token_;

  
  scoped_refptr<AuthChallengeInfo> auth_info_;

  
  
  
  bool embedded_identity_used_;

  
  
  bool default_credentials_used_;

  
  
  
  HttpAuthCache* const http_auth_cache_;
  HttpAuthHandlerFactory* const http_auth_handler_factory_;

  std::set<HttpAuth::Scheme> disabled_schemes_;

  CompletionCallback callback_;
};

}  

#endif  
