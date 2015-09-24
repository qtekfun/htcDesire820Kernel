// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_
#define NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_

#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_network_session.h"
#include "net/http/http_server_properties.h"
#include "net/http/transport_security_state.h"
#include "net/ssl/ssl_config_service.h"
#include "net/url_request/url_request.h"

namespace net {
class CertVerifier;
class CookieStore;
class CTVerifier;
class FraudulentCertificateReporter;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpTransactionFactory;
class HttpUserAgentSettings;
class NetworkDelegate;
class ServerBoundCertService;
class ProxyService;
class URLRequest;
class URLRequestJobFactory;
class URLRequestThrottlerManager;

class NET_EXPORT URLRequestContext
    : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  URLRequestContext();
  virtual ~URLRequestContext();

  
  void CopyFrom(const URLRequestContext* other);

  
  const HttpNetworkSession::Params* GetNetworkSessionParams() const;

  scoped_ptr<URLRequest> CreateRequest(const GURL& url,
                                       RequestPriority priority,
                                       URLRequest::Delegate* delegate) const;

  NetLog* net_log() const {
    return net_log_;
  }

  void set_net_log(NetLog* net_log) {
    net_log_ = net_log;
  }

  HostResolver* host_resolver() const {
    return host_resolver_;
  }

  void set_host_resolver(HostResolver* host_resolver) {
    host_resolver_ = host_resolver;
  }

  CertVerifier* cert_verifier() const {
    return cert_verifier_;
  }

  void set_cert_verifier(CertVerifier* cert_verifier) {
    cert_verifier_ = cert_verifier;
  }

  ServerBoundCertService* server_bound_cert_service() const {
    return server_bound_cert_service_;
  }

  void set_server_bound_cert_service(
      ServerBoundCertService* server_bound_cert_service) {
    server_bound_cert_service_ = server_bound_cert_service;
  }

  FraudulentCertificateReporter* fraudulent_certificate_reporter() const {
    return fraudulent_certificate_reporter_;
  }
  void set_fraudulent_certificate_reporter(
      FraudulentCertificateReporter* fraudulent_certificate_reporter) {
    fraudulent_certificate_reporter_ = fraudulent_certificate_reporter;
  }

  
  ProxyService* proxy_service() const { return proxy_service_; }
  void set_proxy_service(ProxyService* proxy_service) {
    proxy_service_ = proxy_service;
  }

  
  SSLConfigService* ssl_config_service() const {
    return ssl_config_service_.get();
  }
  void set_ssl_config_service(SSLConfigService* service) {
    ssl_config_service_ = service;
  }

  
  
  HttpAuthHandlerFactory* http_auth_handler_factory() const {
    return http_auth_handler_factory_;
  }
  void set_http_auth_handler_factory(HttpAuthHandlerFactory* factory) {
    http_auth_handler_factory_ = factory;
  }

  
  HttpTransactionFactory* http_transaction_factory() const {
    return http_transaction_factory_;
  }
  void set_http_transaction_factory(HttpTransactionFactory* factory) {
    http_transaction_factory_ = factory;
  }

  void set_network_delegate(NetworkDelegate* network_delegate) {
    network_delegate_ = network_delegate;
  }
  NetworkDelegate* network_delegate() const { return network_delegate_; }

  void set_http_server_properties(
      const base::WeakPtr<HttpServerProperties>& http_server_properties) {
    http_server_properties_ = http_server_properties;
  }
  base::WeakPtr<HttpServerProperties> http_server_properties() const {
    return http_server_properties_;
  }

  
  
  CookieStore* cookie_store() const { return cookie_store_.get(); }
  void set_cookie_store(CookieStore* cookie_store);

  TransportSecurityState* transport_security_state() const {
    return transport_security_state_;
  }
  void set_transport_security_state(
      TransportSecurityState* state) {
    transport_security_state_ = state;
  }

  CTVerifier* cert_transparency_verifier() const {
    return cert_transparency_verifier_;
  }
  void set_cert_transparency_verifier(CTVerifier* verifier) {
    cert_transparency_verifier_ = verifier;
  }

  
  
  
  
  
  std::string GetAcceptLanguage() const;
  
  
  std::string GetUserAgent(const GURL& url) const;
  

  const URLRequestJobFactory* job_factory() const { return job_factory_; }
  void set_job_factory(const URLRequestJobFactory* job_factory) {
    job_factory_ = job_factory;
  }

  
  URLRequestThrottlerManager* throttler_manager() const {
    return throttler_manager_;
  }
  void set_throttler_manager(URLRequestThrottlerManager* throttler_manager) {
    throttler_manager_ = throttler_manager;
  }

  
  
  std::set<const URLRequest*>* url_requests() const {
    return url_requests_.get();
  }

  void AssertNoURLRequests() const;

  
  
  const HttpUserAgentSettings* http_user_agent_settings() const {
    return http_user_agent_settings_;
  }
  void set_http_user_agent_settings(
      HttpUserAgentSettings* http_user_agent_settings) {
    http_user_agent_settings_ = http_user_agent_settings;
  }

 private:
  
  
  
  

  
  
  NetLog* net_log_;
  HostResolver* host_resolver_;
  CertVerifier* cert_verifier_;
  ServerBoundCertService* server_bound_cert_service_;
  FraudulentCertificateReporter* fraudulent_certificate_reporter_;
  HttpAuthHandlerFactory* http_auth_handler_factory_;
  ProxyService* proxy_service_;
  scoped_refptr<SSLConfigService> ssl_config_service_;
  NetworkDelegate* network_delegate_;
  base::WeakPtr<HttpServerProperties> http_server_properties_;
  HttpUserAgentSettings* http_user_agent_settings_;
  scoped_refptr<CookieStore> cookie_store_;
  TransportSecurityState* transport_security_state_;
  CTVerifier* cert_transparency_verifier_;
  HttpTransactionFactory* http_transaction_factory_;
  const URLRequestJobFactory* job_factory_;
  URLRequestThrottlerManager* throttler_manager_;

  
  
  
  

  scoped_ptr<std::set<const URLRequest*> > url_requests_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestContext);
};

}  

#endif  
