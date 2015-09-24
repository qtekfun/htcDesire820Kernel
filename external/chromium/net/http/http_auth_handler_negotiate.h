// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_NEGOTIATE_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_NEGOTIATE_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "build/build_config.h"
#include "net/base/address_list.h"
#include "net/http/http_auth_handler.h"
#include "net/http/http_auth_handler_factory.h"

#if defined(OS_WIN)
#include "net/http/http_auth_sspi_win.h"
#elif defined(OS_POSIX)
#include "net/http/http_auth_gssapi_posix.h"
#endif

namespace net {

class HostResolver;
class SingleRequestHostResolver;
class URLSecurityManager;


class HttpAuthHandlerNegotiate : public HttpAuthHandler {
 public:
#if defined(OS_WIN)
  typedef SSPILibrary AuthLibrary;
  typedef HttpAuthSSPI AuthSystem;
#elif defined(OS_POSIX)
  typedef GSSAPILibrary AuthLibrary;
  typedef HttpAuthGSSAPI AuthSystem;
#endif

  class Factory : public HttpAuthHandlerFactory {
   public:
    Factory();
    virtual ~Factory();

    
    
    
    
    bool disable_cname_lookup() const { return disable_cname_lookup_; }
    void set_disable_cname_lookup(bool disable_cname_lookup) {
      disable_cname_lookup_ = disable_cname_lookup;
    }

    
    
    
    
    bool use_port() const { return use_port_; }
    void set_use_port(bool use_port) { use_port_ = use_port; }

    void set_host_resolver(HostResolver* host_resolver);

    
    
    void set_library(AuthLibrary* auth_library) {
      auth_library_.reset(auth_library);
    }

    virtual int CreateAuthHandler(HttpAuth::ChallengeTokenizer* challenge,
                                  HttpAuth::Target target,
                                  const GURL& origin,
                                  CreateReason reason,
                                  int digest_nonce_count,
                                  const BoundNetLog& net_log,
                                  scoped_ptr<HttpAuthHandler>* handler);

   private:
    bool disable_cname_lookup_;
    bool use_port_;
    HostResolver* resolver_;
#if defined(OS_WIN)
    ULONG max_token_length_;
    bool first_creation_;
    bool is_unsupported_;
#endif
    scoped_ptr<AuthLibrary> auth_library_;
  };

  HttpAuthHandlerNegotiate(AuthLibrary* sspi_library,
#if defined(OS_WIN)
                           ULONG max_token_length,
#endif
                           URLSecurityManager* url_security_manager,
                           HostResolver* host_resolver,
                           bool disable_cname_lookup,
                           bool use_port);

  virtual ~HttpAuthHandlerNegotiate();

  
  std::wstring CreateSPN(const AddressList& address_list, const GURL& orign);
  const std::wstring& spn() const { return spn_; }

  
  virtual HttpAuth::AuthorizationResult HandleAnotherChallenge(
      HttpAuth::ChallengeTokenizer* challenge);
  virtual bool NeedsIdentity();
  virtual bool AllowsDefaultCredentials();

 protected:
  virtual bool Init(HttpAuth::ChallengeTokenizer* challenge);

  virtual int GenerateAuthTokenImpl(const string16* username,
                                    const string16* password,
                                    const HttpRequestInfo* request,
                                    CompletionCallback* callback,
                                    std::string* auth_token);

 private:
  enum State {
    STATE_RESOLVE_CANONICAL_NAME,
    STATE_RESOLVE_CANONICAL_NAME_COMPLETE,
    STATE_GENERATE_AUTH_TOKEN,
    STATE_GENERATE_AUTH_TOKEN_COMPLETE,
    STATE_NONE,
  };

  void OnIOComplete(int result);
  void DoCallback(int result);
  int DoLoop(int result);

  int DoResolveCanonicalName();
  int DoResolveCanonicalNameComplete(int rv);
  int DoGenerateAuthToken();
  int DoGenerateAuthTokenComplete(int rv);
  bool CanDelegate() const;

  AuthSystem auth_system_;
  bool disable_cname_lookup_;
  bool use_port_;
  CompletionCallbackImpl<HttpAuthHandlerNegotiate> io_callback_;
  HostResolver* const resolver_;

  
  AddressList address_list_;
  scoped_ptr<SingleRequestHostResolver> single_resolve_;

  
  bool already_called_;
  bool has_username_and_password_;
  string16 username_;
  string16 password_;
  std::wstring spn_;

  
  CompletionCallback* user_callback_;
  std::string* auth_token_;

  State next_state_;

  const URLSecurityManager* url_security_manager_;
};

}  

#endif  
