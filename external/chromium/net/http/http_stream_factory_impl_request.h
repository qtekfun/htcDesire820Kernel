// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_IMPL_REQUEST_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_IMPL_REQUEST_H_

#include <set>
#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"
#include "net/base/net_log.h"
#include "net/http/http_stream_factory_impl.h"

namespace net {

class HttpStreamFactoryImpl::Request : public HttpStreamRequest {
 public:
  Request(const GURL& url,
          HttpStreamFactoryImpl* factory,
          HttpStreamRequest::Delegate* delegate,
          const BoundNetLog& net_log);
  virtual ~Request();

  
  const GURL& url() const { return url_; }

  
  
  
  
  void SetSpdySessionKey(const HostPortProxyPair& spdy_session_key);

  
  
  void AttachJob(HttpStreamFactoryImpl::Job* job);

  
  
  
  void Complete(bool was_npn_negotiated,
                bool using_spdy,
                const NetLog::Source& source);

  
  
  void RemoveRequestFromSpdySessionRequestMap();

  
  void OnSpdySessionReady(Job* job,
                          scoped_refptr<SpdySession> spdy_session,
                          bool direct);

  
  

  void OnStreamReady(Job* job,
                     const SSLConfig& used_ssl_config,
                     const ProxyInfo& used_proxy_info,
                     HttpStream* stream);
  void OnStreamFailed(Job* job, int status, const SSLConfig& used_ssl_config);
  void OnCertificateError(Job* job,
                          int status,
                          const SSLConfig& used_ssl_config,
                          const SSLInfo& ssl_info);
  void OnNeedsProxyAuth(Job* job,
                        const HttpResponseInfo& proxy_response,
                        const SSLConfig& used_ssl_config,
                        const ProxyInfo& used_proxy_info,
                        HttpAuthController* auth_controller);
  void OnNeedsClientAuth(Job* job,
                         const SSLConfig& used_ssl_config,
                         SSLCertRequestInfo* cert_info);
  void OnHttpsProxyTunnelResponse(
      Job *job,
      const HttpResponseInfo& response_info,
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      HttpStream* stream);

  

  virtual int RestartTunnelWithProxyAuth(const string16& username,
                                         const string16& password);
  virtual LoadState GetLoadState() const;
  virtual bool was_npn_negotiated() const;
  virtual bool using_spdy() const;

 private:
  
  
  void OrphanJobsExcept(Job* job);

  
  void OrphanJobs();

  const GURL url_;
  HttpStreamFactoryImpl* const factory_;
  HttpStreamRequest::Delegate* const delegate_;
  const BoundNetLog net_log_;

  
  scoped_ptr<Job> bound_job_;
  std::set<HttpStreamFactoryImpl::Job*> jobs_;
  scoped_ptr<const HostPortProxyPair> spdy_session_key_;

  bool completed_;
  bool was_npn_negotiated_;
  bool using_spdy_;

  DISALLOW_COPY_AND_ASSIGN(Request);
};

}  

#endif  
