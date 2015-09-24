// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2012 The Linux Foundation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_NETWORK_TRANSACTION_H_
#define NET_HTTP_HTTP_NETWORK_TRANSACTION_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/base/ssl_config_service.h"
#include "net/http/http_auth.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_info.h"
#include "net/http/http_stream_factory.h"
#include "net/http/http_transaction.h"
#include "net/proxy/proxy_service.h"

namespace net {

class HttpAuthController;
class HttpNetworkSession;
class HttpStream;
class HttpStreamRequest;
class IOBuffer;
struct HttpRequestInfo;

class HttpNetworkTransaction : public HttpTransaction,
                               public HttpStreamRequest::Delegate {
 public:
  explicit HttpNetworkTransaction(HttpNetworkSession* session);

  virtual ~HttpNetworkTransaction();

  
  virtual int Start(const HttpRequestInfo* request_info,
                    CompletionCallback* callback,
                    const BoundNetLog& net_log);
  virtual int RestartIgnoringLastError(CompletionCallback* callback);
  virtual int RestartWithCertificate(X509Certificate* client_cert,
                                     CompletionCallback* callback);
  virtual int RestartWithAuth(const string16& username,
                              const string16& password,
                              CompletionCallback* callback);
  virtual bool IsReadyToRestartForAuth();

  virtual int Read(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual void StopCaching() {}
  virtual const HttpResponseInfo* GetResponseInfo() const;
  virtual LoadState GetLoadState() const;
  virtual uint64 GetUploadProgress() const;

  
  virtual void OnStreamReady(const SSLConfig& used_ssl_config,
                             const ProxyInfo& used_proxy_info,
                             HttpStream* stream);
  virtual void OnStreamFailed(int status,
                              const SSLConfig& used_ssl_config);
  virtual void OnCertificateError(int status,
                                  const SSLConfig& used_ssl_config,
                                  const SSLInfo& ssl_info);
  virtual void OnNeedsProxyAuth(
      const HttpResponseInfo& response_info,
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      HttpAuthController* auth_controller);
  virtual void OnNeedsClientAuth(const SSLConfig& used_ssl_config,
                                 SSLCertRequestInfo* cert_info);
  virtual void OnHttpsProxyTunnelResponse(const HttpResponseInfo& response_info,
                                          const SSLConfig& used_ssl_config,
                                          const ProxyInfo& used_proxy_info,
                                          HttpStream* stream);

 private:
  FRIEND_TEST_ALL_PREFIXES(HttpNetworkTransactionTest, ResetStateForRestart);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest, WindowUpdateReceived);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest, WindowUpdateSent);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest, WindowUpdateOverflow);
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest, FlowControlStallResume);

  enum State {
    STATE_CREATE_STREAM,
    STATE_CREATE_STREAM_COMPLETE,
    STATE_INIT_STREAM,
    STATE_INIT_STREAM_COMPLETE,
    STATE_GENERATE_PROXY_AUTH_TOKEN,
    STATE_GENERATE_PROXY_AUTH_TOKEN_COMPLETE,
    STATE_GENERATE_SERVER_AUTH_TOKEN,
    STATE_GENERATE_SERVER_AUTH_TOKEN_COMPLETE,
    STATE_BUILD_REQUEST,
    STATE_BUILD_REQUEST_COMPLETE,
    STATE_SEND_REQUEST,
    STATE_SEND_REQUEST_COMPLETE,
    STATE_READ_HEADERS,
    STATE_READ_HEADERS_COMPLETE,
    STATE_READ_BODY,
    STATE_READ_BODY_COMPLETE,
    STATE_DRAIN_BODY_FOR_GETZIP_RETRY,
    STATE_DRAIN_BODY_FOR_GETZIP_RETRY_COMPLETE,
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
  int DoDrainBodyForGetZipRetry();
  int DoDrainBodyForGetZipRetryComplete(int result);
  void DoDrainBodyForRetryComplete( int result, bool isForAuthentication );

  void BuildRequestHeaders(bool using_proxy);

  
  void LogTransactionConnectedMetrics();

  
  void LogTransactionMetrics() const;

  
  
  void LogBlockedTunnelResponse(int response_code) const;

  
  int HandleCertificateRequest(int error);

  
  
  
  int HandleSSLHandshakeError(int error);

  
  
  
  int HandleIOError(int error);

  
  HttpResponseHeaders* GetResponseHeaders() const;

  
  
  bool ShouldResendRequest(int error) const;

  
  
  void ResetConnectionAndRequestForResend();

  
  
  
  int HandleConnectionClosedBeforeEndOfHeaders();

  
  void PrepareForAuthRestart(HttpAuth::Target target);

  
  
  void PrepareForGetZipRetry();

  
  void PrepareForRetry(bool isForAuthentication);

  
  
  
  
  void DidDrainBodyForAuthRestart(bool keep_alive);

  
  
  
  
  void DidDrainBodyForGetZipRetry(bool keep_alive);

  
  
  void DidDrainBodyForRetry( bool keep_alive );

  
  void ResetStateForRestart();

  
  
  void ResetStateForAuthRestart();

  
  bool ShouldApplyProxyAuth() const;

  
  bool ShouldApplyServerAuth() const;

  
  
  
  int HandleAuthChallenge();

  
  bool HaveAuth(HttpAuth::Target target) const;

  
  GURL AuthURL(HttpAuth::Target target) const;

  
  static std::string DescribeState(State state);

  scoped_refptr<HttpAuthController>
      auth_controllers_[HttpAuth::AUTH_NUM_TARGETS];

  
  
  
  HttpAuth::Target pending_auth_target_;

  CompletionCallbackImpl<HttpNetworkTransaction> io_callback_;
  scoped_refptr<CancelableCompletionCallback<HttpNetworkTransaction> >
      delegate_callback_;
  CompletionCallback* user_callback_;
  scoped_ptr<UploadDataStream> request_body_;

  scoped_refptr<HttpNetworkSession> session_;

  BoundNetLog net_log_;
  const HttpRequestInfo* request_;
  HttpResponseInfo response_;

  
  ProxyInfo proxy_info_;

  scoped_ptr<HttpStreamRequest> stream_request_;
  scoped_ptr<HttpStream> stream_;

  
  bool headers_valid_;

  
  
  
  bool logged_response_time_;

  SSLConfig ssl_config_;

  HttpRequestHeaders request_headers_;

  
  
  
  static const int kDrainBodyBufferSize = 1024;

  
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;

  
  base::Time start_time_;

  
  State next_state_;

  
  
  bool establishing_tunnel_;

  bool report_to_stathub_;

  DISALLOW_COPY_AND_ASSIGN(HttpNetworkTransaction);
};

}  

#endif  
