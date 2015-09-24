// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_NETWORK_TRANSACTION_H_
#define NET_HTTP_HTTP_NETWORK_TRANSACTION_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_auth.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_info.h"
#include "net/http/http_stream_factory.h"
#include "net/http/http_transaction.h"
#include "net/proxy/proxy_service.h"
#include "net/ssl/ssl_config_service.h"
#include "net/websockets/websocket_handshake_stream_base.h"

namespace net {

class ClientSocketHandle;
class HttpAuthController;
class HttpNetworkSession;
class HttpStreamBase;
class HttpStreamRequest;
class IOBuffer;
class SpdySession;
struct HttpRequestInfo;

class NET_EXPORT_PRIVATE HttpNetworkTransaction
    : public HttpTransaction,
      public HttpStreamRequest::Delegate {
 public:
  HttpNetworkTransaction(RequestPriority priority,
                         HttpNetworkSession* session);

  virtual ~HttpNetworkTransaction();

  
  virtual int Start(const HttpRequestInfo* request_info,
                    const CompletionCallback& callback,
                    const BoundNetLog& net_log) OVERRIDE;
  virtual int RestartIgnoringLastError(
      const CompletionCallback& callback) OVERRIDE;
  virtual int RestartWithCertificate(
      X509Certificate* client_cert,
      const CompletionCallback& callback) OVERRIDE;
  virtual int RestartWithAuth(const AuthCredentials& credentials,
                              const CompletionCallback& callback) OVERRIDE;
  virtual bool IsReadyToRestartForAuth() OVERRIDE;

  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual void StopCaching() OVERRIDE {}
  virtual bool GetFullRequestHeaders(
      HttpRequestHeaders* headers) const OVERRIDE;
  virtual void DoneReading() OVERRIDE {}
  virtual const HttpResponseInfo* GetResponseInfo() const OVERRIDE;
  virtual LoadState GetLoadState() const OVERRIDE;
  virtual UploadProgress GetUploadProgress() const OVERRIDE;
  virtual bool GetLoadTimingInfo(
      LoadTimingInfo* load_timing_info) const OVERRIDE;
  virtual void SetPriority(RequestPriority priority) OVERRIDE;
  virtual void SetWebSocketHandshakeStreamCreateHelper(
      WebSocketHandshakeStreamBase::CreateHelper* create_helper) OVERRIDE;

  
  virtual void OnStreamReady(const SSLConfig& used_ssl_config,
                             const ProxyInfo& used_proxy_info,
                             HttpStreamBase* stream) OVERRIDE;
  virtual void OnWebSocketHandshakeStreamReady(
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      WebSocketHandshakeStreamBase* stream) OVERRIDE;
  virtual void OnStreamFailed(int status,
                              const SSLConfig& used_ssl_config) OVERRIDE;
  virtual void OnCertificateError(int status,
                                  const SSLConfig& used_ssl_config,
                                  const SSLInfo& ssl_info) OVERRIDE;
  virtual void OnNeedsProxyAuth(
      const HttpResponseInfo& response_info,
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      HttpAuthController* auth_controller) OVERRIDE;
  virtual void OnNeedsClientAuth(const SSLConfig& used_ssl_config,
                                 SSLCertRequestInfo* cert_info) OVERRIDE;
  virtual void OnHttpsProxyTunnelResponse(const HttpResponseInfo& response_info,
                                          const SSLConfig& used_ssl_config,
                                          const ProxyInfo& used_proxy_info,
                                          HttpStreamBase* stream) OVERRIDE;

 private:
  friend class HttpNetworkTransactionSSLTest;

  FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionTest,
                           ResetStateForRestart);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest,
                           WindowUpdateReceived);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest,
                           WindowUpdateSent);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest,
                           WindowUpdateOverflow);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest,
                           FlowControlStallResume);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest,
                           FlowControlStallResumeAfterSettings);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest,
                           FlowControlNegativeSendWindowSize);

  enum State {
    STATE_CREATE_STREAM,
    STATE_CREATE_STREAM_COMPLETE,
    STATE_INIT_STREAM,
    STATE_INIT_STREAM_COMPLETE,
    STATE_GENERATE_PROXY_AUTH_TOKEN,
    STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE,
    STATE_GENERATE_SERVER_AUTH_TOKEN,
    STATE_GENERATE_SERVER_AUTH_TOKEN_COMPLETE,
    STATE_INIT_REQUEST_BODY,
    STATE_INIT_REQUEST_BODY_COMPLETE,
    STATE_BUILD_REQUEST,
    STATE_BUILD_REQUEST_COMPLETE,
    STATE_SEND_REQUEST,
    STATE_SEND_REQUEST_COMPLETE,
    STATE_READ_HEADERS,
    STATE_READ_HEADERS_COMPLETE,
    STATE_READ_BODY,
    STATE_READ_BODY_COMPLETE,
    STATE_DRAIN_BODY_FOR_AUTH_RESTART,
    STATE_DRAIN_BODY_FOR_AUTH_RESTART_COMPLETE,
    STATE_NONE
  };

  bool is_https_request() const;

  void DoCallback(int result);
  void OnIOComplete(int result);

  
  int DoLoop(int result);

  
  
  
  
  int DoCreateStream();
  int DoCreateStreamComplete(int result);
  int DoInitStream();
  int DoInitStreamComplete(int result);
  int DoGenerateProxyAuthToken();
  int DoGenerateProxyAuthTokenComplete(int result);
  int DoGenerateServerAuthToken();
  int DoGenerateServerAuthTokenComplete(int result);
  int DoInitRequestBody();
  int DoInitRequestBodyComplete(int result);
  int DoBuildRequest();
  int DoBuildRequestComplete(int result);
  int DoSendRequest();
  int DoSendRequestComplete(int result);
  int DoReadHeaders();
  int DoReadHeadersComplete(int result);
  int DoReadBody();
  int DoReadBodyComplete(int result);
  int DoDrainBodyForAuthRestart();
  int DoDrainBodyForAuthRestartComplete(int result);

  void BuildRequestHeaders(bool using_proxy);

  
  void LogTransactionConnectedMetrics();

  
  void LogTransactionMetrics() const;

  
  
  void LogBlockedTunnelResponse(int response_code) const;

  
  int HandleCertificateRequest(int error);

  
  void HandleClientAuthError(int error);

  
  
  
  int HandleSSLHandshakeError(int error);

  
  
  
  int HandleIOError(int error);

  
  HttpResponseHeaders* GetResponseHeaders() const;

  
  
  bool ShouldResendRequest(int error) const;

  
  
  void ResetConnectionAndRequestForResend();

  
  
  
  int HandleConnectionClosedBeforeEndOfHeaders();

  
  void PrepareForAuthRestart(HttpAuth::Target target);

  
  
  
  void DidDrainBodyForAuthRestart(bool keep_alive);

  
  void ResetStateForRestart();

  
  
  void ResetStateForAuthRestart();

  
  bool ShouldApplyProxyAuth() const;

  
  bool ShouldApplyServerAuth() const;

  
  
  
  int HandleAuthChallenge();

  
  bool HaveAuth(HttpAuth::Target target) const;

  
  GURL AuthURL(HttpAuth::Target target) const;

  
  bool ForWebSocketHandshake() const;

  
  static std::string DescribeState(State state);

  scoped_refptr<HttpAuthController>
      auth_controllers_[HttpAuth::AUTH_NUM_TARGETS];

  
  
  
  HttpAuth::Target pending_auth_target_;

  CompletionCallback io_callback_;
  CompletionCallback callback_;

  HttpNetworkSession* session_;

  BoundNetLog net_log_;
  const HttpRequestInfo* request_;
  RequestPriority priority_;
  HttpResponseInfo response_;

  
  ProxyInfo proxy_info_;

  scoped_ptr<HttpStreamRequest> stream_request_;
  scoped_ptr<HttpStreamBase> stream_;

  
  bool headers_valid_;

  
  
  
  bool logged_response_time_;

  SSLConfig server_ssl_config_;
  SSLConfig proxy_ssl_config_;
  
  
  
  
  
  int fallback_error_code_;

  HttpRequestHeaders request_headers_;

  
  
  
  static const int kDrainBodyBufferSize = 1024;

  
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;

  
  base::Time start_time_;

  
  
  base::TimeTicks send_start_time_;
  base::TimeTicks send_end_time_;

  
  State next_state_;

  
  
  bool establishing_tunnel_;

  
  
  WebSocketHandshakeStreamBase::CreateHelper*
      websocket_handshake_stream_base_create_helper_;

  DISALLOW_COPY_AND_ASSIGN(HttpNetworkTransaction);
};

}  

#endif  
