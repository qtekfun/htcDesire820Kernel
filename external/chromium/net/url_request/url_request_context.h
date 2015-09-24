// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_
#define NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/ssl_config_service.h"
#include "net/base/transport_security_state.h"
#include "net/ftp/ftp_auth_cache.h"
#include "net/proxy/proxy_service.h"
#include "net/socket/dns_cert_provenance_checker.h"

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
class NetworkDelegate;
class SSLConfigService;
class URLRequest;

class NET_EXPORT URLRequestContext
    : public base::RefCountedThreadSafe<URLRequestContext>,
      public base::NonThreadSafe {
 public:
  URLRequestContext();

  
  void CopyFrom(URLRequestContext* other);

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

  DnsRRResolver* dnsrr_resolver() const {
    return dnsrr_resolver_;
  }

  void set_dnsrr_resolver(DnsRRResolver* dnsrr_resolver) {
    dnsrr_resolver_ = dnsrr_resolver;
  }

  DnsCertProvenanceChecker* dns_cert_checker() const {
    return dns_cert_checker_;
  }
  void set_dns_cert_checker(DnsCertProvenanceChecker* dns_cert_checker) {
    dns_cert_checker_ = dns_cert_checker;
  }

  
  ProxyService* proxy_service() const { return proxy_service_; }
  void set_proxy_service(ProxyService* proxy_service) {
    proxy_service_ = proxy_service;
  }

  
  SSLConfigService* ssl_config_service() const { return ssl_config_service_; }
  void set_ssl_config_service(SSLConfigService* service) {
    ssl_config_service_ = service;
  }

  
  
  HttpAuthHandlerFactory* http_auth_handler_factory() {
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

  
  FtpTransactionFactory* ftp_transaction_factory() {
    return ftp_transaction_factory_;
  }
  void set_ftp_transaction_factory(FtpTransactionFactory* factory) {
    ftp_transaction_factory_ = factory;
  }

  void set_network_delegate(NetworkDelegate* network_delegate) {
    network_delegate_ = network_delegate;
  }
  NetworkDelegate* network_delegate() const { return network_delegate_; }

  
  
  CookieStore* cookie_store() const { return cookie_store_.get(); }
  void set_cookie_store(CookieStore* cookie_store);

  
  
  CookiePolicy* cookie_policy() const { return cookie_policy_; }
  void set_cookie_policy(CookiePolicy* cookie_policy) {
    cookie_policy_ = cookie_policy;
  }

  TransportSecurityState* transport_security_state() const {
      return transport_security_state_;
  }
  void set_transport_security_state(
      TransportSecurityState* state) {
    transport_security_state_ = state;
  }

  
  FtpAuthCache* ftp_auth_cache() { return &ftp_auth_cache_; }

  
  const std::string& accept_charset() const { return accept_charset_; }
  void set_accept_charset(const std::string& accept_charset) {
    accept_charset_ = accept_charset;
  }

  
#ifdef ANDROID
  virtual
#endif
  const std::string& accept_language() const { return accept_language_; }

  void set_accept_language(const std::string& accept_language) {
    accept_language_ = accept_language;
  }

  
  
  
  virtual const std::string& GetUserAgent(const GURL& url) const;

  
  
  const std::string& referrer_charset() const { return referrer_charset_; }
  void set_referrer_charset(const std::string& charset) {
    referrer_charset_ = charset;
  }

  
  
  bool is_main() const { return is_main_; }
  void set_is_main(bool is_main) { is_main_ = is_main; }

  
  bool IsSNIAvailable() const;

#ifdef ANDROID
  
  bool getUID(uid_t *uid) const;
  void setUID(uid_t uid);
#endif

 protected:
  friend class base::RefCountedThreadSafe<URLRequestContext>;

  virtual ~URLRequestContext();

 private:
  
  
  
  

  
  bool is_main_;

  
  
  NetLog* net_log_;
  HostResolver* host_resolver_;
  CertVerifier* cert_verifier_;
  DnsRRResolver* dnsrr_resolver_;
  DnsCertProvenanceChecker* dns_cert_checker_;
  HttpAuthHandlerFactory* http_auth_handler_factory_;
  scoped_refptr<ProxyService> proxy_service_;
  scoped_refptr<SSLConfigService> ssl_config_service_;
  NetworkDelegate* network_delegate_;
  scoped_refptr<CookieStore> cookie_store_;
  CookiePolicy* cookie_policy_;
  scoped_refptr<TransportSecurityState> transport_security_state_;
  FtpAuthCache ftp_auth_cache_;
  std::string accept_language_;
  std::string accept_charset_;
  
  
  
  std::string referrer_charset_;

  HttpTransactionFactory* http_transaction_factory_;
  FtpTransactionFactory* ftp_transaction_factory_;

  
  
  
  

#ifdef ANDROID
  bool valid_uid_;
  uid_t calling_uid_;
#endif

  DISALLOW_COPY_AND_ASSIGN(URLRequestContext);
};

}  

#endif  
