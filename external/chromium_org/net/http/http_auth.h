// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_H_
#define NET_HTTP_HTTP_AUTH_H_

#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/auth.h"
#include "net/base/net_export.h"
#include "net/http/http_util.h"

template <class T> class scoped_refptr;

namespace net {

class BoundNetLog;
class HttpAuthHandler;
class HttpAuthHandlerFactory;
class HttpResponseHeaders;

class NET_EXPORT_PRIVATE HttpAuth {
 public:
  
  
  enum Target {
    AUTH_NONE = -1,
    
    
    AUTH_PROXY = 0,
    AUTH_SERVER = 1,
    AUTH_NUM_TARGETS = 2,
  };

  
  
  enum AuthorizationResult {
    AUTHORIZATION_RESULT_ACCEPT,   
                                   
                                   

    AUTHORIZATION_RESULT_REJECT,   

    AUTHORIZATION_RESULT_STALE,    
                                   
                                   

    AUTHORIZATION_RESULT_INVALID,  
                                   
                                   

    AUTHORIZATION_RESULT_DIFFERENT_REALM,  
                                           
                                           
                                           
                                           
                                           
  };

  
  enum IdentitySource {
    
    IDENT_SRC_NONE,

    
    
    IDENT_SRC_PATH_LOOKUP,

    
    
    IDENT_SRC_URL,

    
    
    IDENT_SRC_REALM_LOOKUP,

    
    
    IDENT_SRC_EXTERNAL,

    
    
    IDENT_SRC_DEFAULT_CREDENTIALS,
  };

  enum Scheme {
    AUTH_SCHEME_BASIC = 0,
    AUTH_SCHEME_DIGEST,
    AUTH_SCHEME_NTLM,
    AUTH_SCHEME_NEGOTIATE,
    AUTH_SCHEME_SPDYPROXY,
    AUTH_SCHEME_MOCK,
    AUTH_SCHEME_MAX,
  };

  
  
  struct Identity {
    Identity();

    IdentitySource source;
    bool invalid;
    AuthCredentials credentials;
  };

  
  
  static std::string GetChallengeHeaderName(Target target);

  
  
  static std::string GetAuthorizationHeaderName(Target target);

  
  
  static std::string GetAuthTargetString(Target target);

  
  static const char* SchemeToString(Scheme scheme);

  
  
  
  
  
  
  
  
  
  static void ChooseBestChallenge(
      HttpAuthHandlerFactory* http_auth_handler_factory,
      const HttpResponseHeaders* headers,
      Target target,
      const GURL& origin,
      const std::set<Scheme>& disabled_schemes,
      const BoundNetLog& net_log,
      scoped_ptr<HttpAuthHandler>* handler);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static AuthorizationResult HandleChallengeResponse(
      HttpAuthHandler* handler,
      const HttpResponseHeaders* headers,
      Target target,
      const std::set<Scheme>& disabled_schemes,
      std::string* challenge_used);

  
  
  
  
  
  
  
  
  class NET_EXPORT_PRIVATE ChallengeTokenizer {
   public:
    ChallengeTokenizer(std::string::const_iterator begin,
                       std::string::const_iterator end);

    
    std::string challenge_text() const {
      return std::string(begin_, end_);
    }

    
    std::string::const_iterator scheme_begin() const { return scheme_begin_; }
    std::string::const_iterator scheme_end() const { return scheme_end_; }
    std::string scheme() const {
      return std::string(scheme_begin_, scheme_end_);
    }

    HttpUtil::NameValuePairsIterator param_pairs() const;
    std::string base64_param() const;

   private:
    void Init(std::string::const_iterator begin,
              std::string::const_iterator end);

    std::string::const_iterator begin_;
    std::string::const_iterator end_;

    std::string::const_iterator scheme_begin_;
    std::string::const_iterator scheme_end_;

    std::string::const_iterator params_begin_;
    std::string::const_iterator params_end_;
  };
};

}  

#endif  
