// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_CONTEXT_STORAGE_H_
#define NET_URL_REQUEST_URL_REQUEST_CONTEXT_STORAGE_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace net {

class CertVerifier;
class CookiePolicy;
class CookieStore;
class DnsCertProvenanceChecker;
class DnsRRResolver;
class FtpTransactionFactory;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpTransactionFactory;
class NetLog;
class NetworkDelegate;
class ProxyService;
class SSLConfigService;
class TransportSecurityState;
class URLRequestContext;

class URLRequestContextStorage {
 public:
  
  
  
  explicit URLRequestContextStorage(URLRequestContext* context);
  ~URLRequestContextStorage();

  
  

  void set_net_log(NetLog* net_log);
  void set_host_resolver(HostResolver* host_resolver);
  void set_cert_verifier(CertVerifier* cert_verifier);
  void set_dnsrr_resolver(DnsRRResolver* dnsrr_resolver);
  void set_dns_cert_checker(DnsCertProvenanceChecker* dns_cert_checker);
  void set_http_auth_handler_factory(
      HttpAuthHandlerFactory* http_auth_handler_factory);
  void set_proxy_service(ProxyService* proxy_service);
  void set_ssl_config_service(SSLConfigService* ssl_config_service);
  void set_network_delegate(NetworkDelegate* network_delegate);
  void set_cookie_store(CookieStore* cookie_store);
  void set_cookie_policy(CookiePolicy* cookie_policy);
  void set_transport_security_state(
      TransportSecurityState* transport_security_state);
  void set_http_transaction_factory(
      HttpTransactionFactory* http_transaction_factory);
  void set_ftp_transaction_factory(
      FtpTransactionFactory* ftp_transaction_factory);

 private:
  
  
  
  URLRequestContext* const context_;

  
  scoped_ptr<NetLog> net_log_;
  scoped_ptr<HostResolver> host_resolver_;
  scoped_ptr<CertVerifier> cert_verifier_;
  scoped_ptr<DnsRRResolver> dnsrr_resolver_;
  scoped_ptr<DnsCertProvenanceChecker> dns_cert_checker_;
  scoped_ptr<HttpAuthHandlerFactory> http_auth_handler_factory_;
  
  scoped_refptr<ProxyService> proxy_service_;
  scoped_refptr<SSLConfigService> ssl_config_service_;
  scoped_ptr<NetworkDelegate> network_delegate_;
  scoped_refptr<CookieStore> cookie_store_;
  scoped_ptr<CookiePolicy> cookie_policy_;
  scoped_refptr<TransportSecurityState> transport_security_state_;

  scoped_ptr<HttpTransactionFactory> http_transaction_factory_;
  scoped_ptr<FtpTransactionFactory> ftp_transaction_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestContextStorage);
};

}  

#endif  
