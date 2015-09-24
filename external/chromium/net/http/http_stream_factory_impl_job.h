// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_IMPL_JOB_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_IMPL_JOB_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/base/ssl_config_service.h"
#include "net/http/http_alternate_protocols.h"
#include "net/http/http_auth.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_request_info.h"
#include "net/http/http_stream_factory_impl.h"
#include "net/proxy/proxy_service.h"
#include "net/socket/client_socket_handle.h"

namespace net {

class ClientSocketHandle;
class HttpAuthController;
class HttpNetworkSession;
class HttpProxySocketParams;
class HttpStream;
class SOCKSSocketParams;
class SSLSocketParams;
class TransportSocketParams;

class HttpStreamFactoryImpl::Job {
 public:
  Job(HttpStreamFactoryImpl* stream_factory,
      HttpNetworkSession* session,
      const HttpRequestInfo& request_info,
      const SSLConfig& ssl_config,
      const BoundNetLog& net_log);
  ~Job();

  
  
  void Start(Request* request);

  
  
  int Preconnect(int num_streams);

  int RestartTunnelWithProxyAuth(const string16& username,
                                 const string16& password);
  LoadState GetLoadState() const;

  
  
  
  void MarkAsAlternate(const GURL& original_url);

  
  void WaitFor(Job* job);

  
  
  
  void Resume(Job* job);

  
  void Orphan(const Request* request);

  bool was_npn_negotiated() const;
  bool using_spdy() const;
  const BoundNetLog& net_log() const { return net_log_; }

  const SSLConfig& ssl_config() const;
  const ProxyInfo& proxy_info() const;

  
  bool IsPreconnecting() const;

  
  bool IsOrphaned() const;

 private:
  enum State {
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
  void OnSpdySessionReadyCallback();
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

  bool IsHttpsProxyAndHttpUrl();

  void InitSSLConfig(const HostPortPair& origin_server,
                     SSLConfig* ssl_config) const;

  
  void MarkBrokenAlternateProtocolAndFallback();

  
  
  
  void GetSSLInfo();

  
  
  
  
  
  
  int ReconsiderProxyAfterError(int error);

  
  
  
  int HandleCertificateError(int error);

  
  int HandleCertificateRequest(int error);

  
  void SwitchToSpdyMode();

  
  bool ShouldForceSpdySSL() const;

  
  bool ShouldForceSpdyWithoutSSL() const;

  
  static void LogHttpConnectedMetrics(const ClientSocketHandle& handle);

  Request* request_;

  const HttpRequestInfo request_info_;
  ProxyInfo proxy_info_;
  SSLConfig ssl_config_;
  const BoundNetLog net_log_;

  CompletionCallbackImpl<Job> io_callback_;
  scoped_ptr<ClientSocketHandle> connection_;
  HttpNetworkSession* const session_;
  HttpStreamFactoryImpl* const stream_factory_;
  State next_state_;
  ProxyService::PacRequest* pac_request_;
  SSLInfo ssl_info_;

  
  HostPortPair origin_;

  
  
  scoped_ptr<GURL> original_url_;

  
  
  Job* blocking_job_;

  
  Job* dependent_job_;

  
  bool using_ssl_;

  
  bool using_spdy_;

  
  bool force_spdy_always_;

  
  bool force_spdy_over_ssl_;

  
  int spdy_certificate_error_;

  scoped_refptr<HttpAuthController>
      auth_controllers_[HttpAuth::AUTH_NUM_TARGETS];

  
  
  bool establishing_tunnel_;

  scoped_ptr<HttpStream> stream_;

  
  bool was_npn_negotiated_;

  
  
  int num_streams_;

  
  scoped_refptr<SpdySession> new_spdy_session_;

  
  bool spdy_session_direct_;

  ScopedRunnableMethodFactory<Job> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(Job);
};

}  

#endif  
