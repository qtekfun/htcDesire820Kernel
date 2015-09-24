// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2012, The Linux Foundation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_H_

#include <list>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"

class GURL;

namespace net {

class BoundNetLog;
class HostMappingRules;
class HostPortPair;
class HttpAlternateProtocols;
class HttpAuthController;
class HttpNetworkSession;
class HttpResponseInfo;
class HttpStream;
class ProxyInfo;
class SSLCertRequestInfo;
class SSLInfo;
class X509Certificate;
struct HttpRequestInfo;
struct SSLConfig;

class HttpStreamRequest {
 public:
  
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    
    
    
    virtual void OnStreamReady(
        const SSLConfig& used_ssl_config,
        const ProxyInfo& used_proxy_info,
        HttpStream* stream) = 0;

    
    
    
    
    virtual void OnStreamFailed(int status,
                                const SSLConfig& used_ssl_config) = 0;

    
    
    
    
    virtual void OnCertificateError(int status,
                                    const SSLConfig& used_ssl_config,
                                    const SSLInfo& ssl_info) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnNeedsProxyAuth(const HttpResponseInfo& proxy_response,
                                  const SSLConfig& used_ssl_config,
                                  const ProxyInfo& used_proxy_info,
                                  HttpAuthController* auth_controller) = 0;

    
    
    
    
    virtual void OnNeedsClientAuth(const SSLConfig& used_ssl_config,
                                   SSLCertRequestInfo* cert_info) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnHttpsProxyTunnelResponse(
        const HttpResponseInfo& response_info,
        const SSLConfig& used_ssl_config,
        const ProxyInfo& used_proxy_info,
        HttpStream* stream) = 0;
  };

  virtual ~HttpStreamRequest() {}

  
  
  
  
  
  virtual int RestartTunnelWithProxyAuth(const string16& username,
                                         const string16& password) = 0;

  
  virtual LoadState GetLoadState() const = 0;

  
  virtual bool was_npn_negotiated() const = 0;

  
  virtual bool using_spdy() const = 0;
};

class HttpStreamFactory {
 public:
  virtual ~HttpStreamFactory();

  void ProcessAlternateProtocol(
      HttpAlternateProtocols* alternate_protocols,
      const std::string& alternate_protocol_str,
      const HostPortPair& http_host_port_pair);

  

  
  
  virtual HttpStreamRequest* RequestStream(
      const HttpRequestInfo& info,
      const SSLConfig& ssl_config,
      HttpStreamRequest::Delegate* delegate,
      const BoundNetLog& net_log) = 0;

  
  virtual void PreconnectStreams(int num_streams,
                                 const HttpRequestInfo& info,
                                 const SSLConfig& ssl_config,
                                 const BoundNetLog& net_log) = 0;

  
  virtual int PreconnectStreams(int num_streams,
                                 const HttpRequestInfo& info,
                                 const SSLConfig& ssl_config,
                                 const BoundNetLog& net_log,
                                 CompletionCallback* callback) = 0;

  virtual void AddTLSIntolerantServer(const HostPortPair& server) = 0;
  virtual bool IsTLSIntolerantServer(const HostPortPair& server) const = 0;

  
  static GURL ApplyHostMappingRules(const GURL& url, HostPortPair* endpoint);

  
  static void set_spdy_enabled(bool value) {
    spdy_enabled_ = value;
    if (value == false)
      set_next_protos("");
  }
  static bool spdy_enabled() { return spdy_enabled_; }

  
  static void set_use_alternate_protocols(bool value) {
    use_alternate_protocols_ = value;
  }
  static bool use_alternate_protocols() { return use_alternate_protocols_; }

  
  static void set_force_spdy_over_ssl(bool value) {
    force_spdy_over_ssl_ = value;
  }
  static bool force_spdy_over_ssl() {
    return force_spdy_over_ssl_;
  }

  
  static void set_force_spdy_always(bool value) {
    force_spdy_always_ = value;
  }
  static bool force_spdy_always() { return force_spdy_always_; }

  
  static void add_forced_spdy_exclusion(const std::string& value);
  
  static bool HasSpdyExclusion(const HostPortPair& endpoint);

  
  static void set_next_protos(const std::string& value) {
    delete next_protos_;
    next_protos_ = new std::string(value);
  }
  static const std::string* next_protos() { return next_protos_; }

  
  
  static void set_ignore_certificate_errors(bool value) {
    ignore_certificate_errors_ = value;
  }
  static bool ignore_certificate_errors() {
    return ignore_certificate_errors_;
  }

  static void SetHostMappingRules(const std::string& rules);

 protected:
  HttpStreamFactory();

 private:
  static const HostMappingRules& host_mapping_rules();

  static const HostMappingRules* host_mapping_rules_;
  static const std::string* next_protos_;
  static bool spdy_enabled_;
  static bool use_alternate_protocols_;
  static bool force_spdy_over_ssl_;
  static bool force_spdy_always_;
  static std::list<HostPortPair>* forced_spdy_exclusions_;
  static bool ignore_certificate_errors_;

  DISALLOW_COPY_AND_ASSIGN(HttpStreamFactory);
};

}  

#endif  
