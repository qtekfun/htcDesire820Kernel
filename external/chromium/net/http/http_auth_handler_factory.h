// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_FACTORY_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_FACTORY_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/http/http_auth.h"
#include "net/http/url_security_manager.h"

class GURL;

namespace net {

class BoundNetLog;
class HostResolver;
class HttpAuthHandler;
class HttpAuthHandlerRegistryFactory;

class NET_EXPORT HttpAuthHandlerFactory {
 public:
  enum CreateReason {
    CREATE_CHALLENGE,     
    CREATE_PREEMPTIVE,    
  };

  HttpAuthHandlerFactory() : url_security_manager_(NULL) {}
  virtual ~HttpAuthHandlerFactory() {}

  
  
  void set_url_security_manager(URLSecurityManager* url_security_manager) {
    url_security_manager_ = url_security_manager;
  }

  
  URLSecurityManager* url_security_manager() {
    return url_security_manager_;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int CreateAuthHandler(HttpAuth::ChallengeTokenizer* challenge,
                                HttpAuth::Target target,
                                const GURL& origin,
                                CreateReason create_reason,
                                int digest_nonce_count,
                                const BoundNetLog& net_log,
                                scoped_ptr<HttpAuthHandler>* handler) = 0;

  
  
  
  
  
  int CreateAuthHandlerFromString(const std::string& challenge,
                                  HttpAuth::Target target,
                                  const GURL& origin,
                                  const BoundNetLog& net_log,
                                  scoped_ptr<HttpAuthHandler>* handler);

  
  
  
  
  
  int CreatePreemptiveAuthHandlerFromString(
      const std::string& challenge,
      HttpAuth::Target target,
      const GURL& origin,
      int digest_nonce_count,
      const BoundNetLog& net_log,
      scoped_ptr<HttpAuthHandler>* handler);

  
  
  
  
  
  
  
  
  
  static HttpAuthHandlerRegistryFactory* CreateDefault(HostResolver* resolver);

 private:
  
  URLSecurityManager* url_security_manager_;

  DISALLOW_COPY_AND_ASSIGN(HttpAuthHandlerFactory);
};

class NET_EXPORT HttpAuthHandlerRegistryFactory 
    : public HttpAuthHandlerFactory {
 public:
  HttpAuthHandlerRegistryFactory();
  virtual ~HttpAuthHandlerRegistryFactory();

  
  void SetURLSecurityManager(const std::string& scheme,
                             URLSecurityManager* url_security_manager);

  
  
  
  
  
  
  
  
  void RegisterSchemeFactory(const std::string& scheme,
                             HttpAuthHandlerFactory* factory);

  
  
  
  
  
  HttpAuthHandlerFactory* GetSchemeFactory(const std::string& scheme) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static HttpAuthHandlerRegistryFactory* Create(
      const std::vector<std::string>& supported_schemes,
      URLSecurityManager* security_manager,
      HostResolver* host_resolver,
      const std::string& gssapi_library_name,
      bool negotiate_disable_cname_lookup,
      bool negotiate_enable_port);

  
  
  virtual int CreateAuthHandler(HttpAuth::ChallengeTokenizer* challenge,
                                HttpAuth::Target target,
                                const GURL& origin,
                                CreateReason reason,
                                int digest_nonce_count,
                                const BoundNetLog& net_log,
                                scoped_ptr<HttpAuthHandler>* handler);

 private:
  typedef std::map<std::string, HttpAuthHandlerFactory*> FactoryMap;

  FactoryMap factory_map_;
  DISALLOW_COPY_AND_ASSIGN(HttpAuthHandlerRegistryFactory);
};

}  

#endif  
