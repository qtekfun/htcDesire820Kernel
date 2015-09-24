// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_IMPL_JOB_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_IMPL_JOB_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_auth.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_pipelined_host.h"
#include "net/http/http_request_info.h"
#include "net/http/http_stream_factory_impl.h"
#include "net/proxy/proxy_service.h"
#include "net/quic/quic_stream_factory.h"
#include "net/socket/client_socket_handle.h"
#include "net/socket/ssl_client_socket.h"
#include "net/spdy/spdy_session_key.h"
#include "net/ssl/ssl_config_service.h"

namespace net {

class ClientSocketHandle;
class HttpAuthController;
class HttpNetworkSession;
class HttpStream;
class SpdySessionPool;
class QuicHttpStream;

class HttpStreamFactoryImpl::Job {
 public:
  Job(HttpStreamFactoryImpl* stream_factory,
      HttpNetworkSession* session,
      const HttpRequestInfo& request_info,
      RequestPriority priority,
      const SSLConfig& server_ssl_config,
      const SSLConfig& proxy_ssl_config,
      NetLog* net_log);
  ~Job();

  
  
  void Start(Request* request);

  
  
  int Preconnect(int num_streams);

  int RestartTunnelWithProxyAuth(const AuthCredentials& credentials);
  LoadState GetLoadState() const;

  
  
  
  
  void MarkAsAlternate(const GURL& original_url,
                       PortAlternateProtocolPair alternate);

  
  void WaitFor(Job* job);

  
  
  
  void Resume(Job* job);

  
  void Orphan(const Request* request);

  void SetPriority(RequestPriority priority);

  RequestPriority priority() const { return priority_; }
  bool was_npn_negotiated() const;
  NextProto protocol_negotiated() const;
  bool using_spdy() const;
  const BoundNetLog& net_log() const { return net_log_; }

  const SSLConfig& server_ssl_config() const;
  const SSLConfig& proxy_ssl_config() const;
  const ProxyInfo& proxy_info() const;

  
  bool IsPreconnecting() const;

  
  bool IsOrphaned() const;

 private:
  enum State {
    STATE_START,
    STATE_RESOLVE_PROXY,
    STATE_RESOLVE_PROXY_COMPLETE,

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    STATE_WAIT_FOR_JOB,
    STATE_WAIT_FOR_JOB_COMPLETE,

    STATE_INIT_CONNECTION,
    STATE_INIT_CONNECTION_COMPLETE,
    STATE_WAITING_USER_ACTION,
    STATE_RESTART_TUNNEL_AUTH,
    STATE_RESTART_TUNNEL_AUTH_COMPLETE,
    STATE_CREATE_STREAM,
    STATE_CREATE_STREAM_COMPLETE,
    STATE_DRAIN_BODY_FOR_AUTH_RESTART,
    STATE_DRAIN_BODY_FOR_AUTH_RESTART_COMPLETE,
    STATE_DONE,
    STATE_NONE
  };

  void OnStreamReadyCallback();
  void OnWebSocketHandshakeStreamReadyCallback();
  
  void OnNewSpdySessionReadyCallback();
  void OnStreamFailedCallback(int result);
  void OnCertificateErrorCallback(int result, const SSLInfo& ssl_info);
  void OnNeedsProxyAuthCallback(const HttpResponseInfo& response_info,
                                HttpAuthController* auth_controller);
  void OnNeedsClientAuthCallback(SSLCertRequestInfo* cert_info);
  void OnHttpsProxyTunnelResponseCallback(const HttpResponseInfo& response_info,
                                          HttpStream* stream);
  void OnPreconnectsComplete();

  void OnIOComplete(int result);
  int RunLoop(int result);
  int DoLoop(int result);
  int StartInternal();

  
  
  
  
  int DoStart();
  int DoResolveProxy();
  int DoResolveProxyComplete(int result);
  int DoWaitForJob();
  int DoWaitForJobComplete(int result);
  int DoInitConnection();
  int DoInitConnectionComplete(int result);
  int DoWaitingUserAction(int result);
  int DoCreateStream();
  int DoCreateStreamComplete(int result);
  int DoRestartTunnelAuth();
  int DoRestartTunnelAuthComplete(int result);

  
  void ReturnToStateInitConnection(bool close_connection);

  
  void SetSocketMotivation();

  bool IsHttpsProxyAndHttpUrl() const;

  
  
  void InitSSLConfig(const HostPortPair& origin_server,
                     SSLConfig* ssl_config,
                     bool is_proxy) const;

  
  
  
  void GetSSLInfo();

  SpdySessionKey GetSpdySessionKey() const;

  
  bool CanUseExistingSpdySession() const;

  
  
  
  
  
  
  int ReconsiderProxyAfterError(int error);

  
  
  
  int HandleCertificateError(int error);

  
  int HandleCertificateRequest(int error);

  
  void SwitchToSpdyMode();

  
  bool ShouldForceSpdySSL() const;

  
  bool ShouldForceSpdyWithoutSSL() const;

  
  bool ShouldForceQuic() const;

  bool IsRequestEligibleForPipelining();

  
  static void LogHttpConnectedMetrics(const ClientSocketHandle& handle);

  
  
  
  
  static int OnHostResolution(SpdySessionPool* spdy_session_pool,
                              const SpdySessionKey& spdy_session_key,
                              const AddressList& addresses,
                              const BoundNetLog& net_log);

  Request* request_;

  const HttpRequestInfo request_info_;
  RequestPriority priority_;
  ProxyInfo proxy_info_;
  SSLConfig server_ssl_config_;
  SSLConfig proxy_ssl_config_;
  const BoundNetLog net_log_;

  CompletionCallback io_callback_;
  scoped_ptr<ClientSocketHandle> connection_;
  HttpNetworkSession* const session_;
  HttpStreamFactoryImpl* const stream_factory_;
  State next_state_;
  ProxyService::PacRequest* pac_request_;
  SSLInfo ssl_info_;

  
  HostPortPair origin_;

  
  
  GURL origin_url_;

  
  
  scoped_ptr<GURL> original_url_;

  
  
  Job* blocking_job_;

  
  
  
  Job* waiting_job_;

  
  bool using_ssl_;

  
  bool using_spdy_;

  
  bool using_quic_;
  QuicStreamRequest quic_request_;

  
  bool force_spdy_always_;

  
  bool force_spdy_over_ssl_;

  
  int force_quic_port_;

  
  int spdy_certificate_error_;

  scoped_refptr<HttpAuthController>
      auth_controllers_[HttpAuth::AUTH_NUM_TARGETS];

  
  
  bool establishing_tunnel_;

  scoped_ptr<HttpStream> stream_;
  scoped_ptr<WebSocketHandshakeStreamBase> websocket_stream_;

  
  bool was_npn_negotiated_;

  
  NextProto protocol_negotiated_;

  
  
  int num_streams_;

  
  base::WeakPtr<SpdySession> new_spdy_session_;

  
  base::WeakPtr<SpdySession> existing_spdy_session_;

  
  bool spdy_session_direct_;

  
  scoped_ptr<HttpPipelinedHost::Key> http_pipelining_key_;

  
  bool existing_available_pipeline_;

  base::WeakPtrFactory<Job> ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(Job);
};

}  

#endif  
