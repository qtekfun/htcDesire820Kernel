// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_IMPL_REQUEST_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_IMPL_REQUEST_H_

#include <set>
#include "base/memory/scoped_ptr.h"
#include "net/base/net_log.h"
#include "net/http/http_stream_factory_impl.h"
#include "net/socket/ssl_client_socket.h"
#include "net/spdy/spdy_session_key.h"
#include "url/gurl.h"

namespace net {

class ClientSocketHandle;
class SpdySession;

class HttpStreamFactoryImpl::Request : public HttpStreamRequest {
 public:
  Request(const GURL& url,
          HttpStreamFactoryImpl* factory,
          HttpStreamRequest::Delegate* delegate,
          WebSocketHandshakeStreamBase::CreateHelper*
              websocket_handshake_stream_create_helper,
          const BoundNetLog& net_log);
  virtual ~Request();

  
  const GURL& url() const { return url_; }

  
  
  
  
  void SetSpdySessionKey(const SpdySessionKey& spdy_session_key);

  
  
  
  
  bool SetHttpPipeliningKey(const HttpPipelinedHost::Key& http_pipelining_key);

  
  
  void AttachJob(HttpStreamFactoryImpl::Job* job);

  
  
  void Complete(bool was_npn_negotiated,
                NextProto protocol_negotiated,
                bool using_spdy,
                const BoundNetLog& job_net_log);

  
  
  void RemoveRequestFromSpdySessionRequestMap();

  
  
  void RemoveRequestFromHttpPipeliningRequestMap();

  
  void OnNewSpdySessionReady(Job* job,
                             const base::WeakPtr<SpdySession>& spdy_session,
                             bool direct);

  WebSocketHandshakeStreamBase::CreateHelper*
  websocket_handshake_stream_create_helper() {
    return websocket_handshake_stream_create_helper_;
  }

  
  

  void OnStreamReady(Job* job,
                     const SSLConfig& used_ssl_config,
                     const ProxyInfo& used_proxy_info,
                     HttpStreamBase* stream);
  void OnWebSocketHandshakeStreamReady(Job* job,
                                       const SSLConfig& used_ssl_config,
                                       const ProxyInfo& used_proxy_info,
                                       WebSocketHandshakeStreamBase* stream);
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
      HttpStreamBase* stream);

  

  virtual int RestartTunnelWithProxyAuth(
      const AuthCredentials& credentials) OVERRIDE;
  virtual void SetPriority(RequestPriority priority) OVERRIDE;
  virtual LoadState GetLoadState() const OVERRIDE;
  virtual bool was_npn_negotiated() const OVERRIDE;
  virtual NextProto protocol_negotiated() const OVERRIDE;
  virtual bool using_spdy() const OVERRIDE;

 private:
  
  
  void OrphanJobsExcept(Job* job);

  
  void OrphanJobs();

  
  void OnJobSucceeded(Job* job);

  const GURL url_;
  HttpStreamFactoryImpl* const factory_;
  WebSocketHandshakeStreamBase::CreateHelper* const
      websocket_handshake_stream_create_helper_;
  HttpStreamRequest::Delegate* const delegate_;
  const BoundNetLog net_log_;

  
  scoped_ptr<Job> bound_job_;
  std::set<HttpStreamFactoryImpl::Job*> jobs_;
  scoped_ptr<const SpdySessionKey> spdy_session_key_;
  scoped_ptr<const HttpPipelinedHost::Key> http_pipelining_key_;

  bool completed_;
  bool was_npn_negotiated_;
  
  NextProto protocol_negotiated_;
  bool using_spdy_;

  DISALLOW_COPY_AND_ASSIGN(Request);
};

}  

#endif  
