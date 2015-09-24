// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PROXY_CLIENT_SOCKET_H_
#define NET_HTTP_HTTP_PROXY_CLIENT_SOCKET_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/load_timing_info.h"
#include "net/base/net_log.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_info.h"
#include "net/http/proxy_client_socket.h"
#include "net/socket/ssl_client_socket.h"

class GURL;

namespace net {

class AddressList;
class ClientSocketHandle;
class GrowableIOBuffer;
class HttpAuthCache;
class HttpStream;
class HttpStreamParser;
class IOBuffer;

class HttpProxyClientSocket : public ProxyClientSocket {
 public:
  
  
  
  HttpProxyClientSocket(ClientSocketHandle* transport_socket,
                        const GURL& request_url,
                        const std::string& user_agent,
                        const HostPortPair& endpoint,
                        const HostPortPair& proxy_server,
                        HttpAuthCache* http_auth_cache,
                        HttpAuthHandlerFactory* http_auth_handler_factory,
                        bool tunnel,
                        bool using_spdy,
                        NextProto protocol_negotiated,
                        bool is_https_proxy);

  
  virtual ~HttpProxyClientSocket();

  
  virtual const HttpResponseInfo* GetConnectResponseInfo() const OVERRIDE;
  virtual HttpStream* CreateConnectResponseStream() OVERRIDE;
  virtual int RestartWithAuth(const CompletionCallback& callback) OVERRIDE;
  virtual const scoped_refptr<HttpAuthController>& GetAuthController() const
      OVERRIDE;
  virtual bool IsUsingSpdy() const OVERRIDE;
  virtual NextProto GetProtocolNegotiated() const OVERRIDE;

  
  virtual int Connect(const CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual const BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual NextProto GetNegotiatedProtocol() const OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(IPEndPoint* address) const OVERRIDE;

 private:
  enum State {
    STATE_NONE,
    STATE_GENERATE_AUTH_TOKEN,
    STATE_GENERATE_AUTH_TOKEN_COMPLETE,
    STATE_SEND_REQUEST,
    STATE_SEND_REQUEST_COMPLETE,
    STATE_READ_HEADERS,
    STATE_READ_HEADERS_COMPLETE,
    STATE_DRAIN_BODY,
    STATE_DRAIN_BODY_COMPLETE,
    STATE_TCP_RESTART,
    STATE_TCP_RESTART_COMPLETE,
    STATE_DONE,
  };

  
  
  
  static const int kDrainBodyBufferSize = 1024;

  int PrepareForAuthRestart();
  int DidDrainBodyForAuthRestart(bool keep_alive);

  void LogBlockedTunnelResponse() const;

  void DoCallback(int result);
  void OnIOComplete(int result);

  int DoLoop(int last_io_result);
  int DoGenerateAuthToken();
  int DoGenerateAuthTokenComplete(int result);
  int DoSendRequest();
  int DoSendRequestComplete(int result);
  int DoReadHeaders();
  int DoReadHeadersComplete(int result);
  int DoDrainBody();
  int DoDrainBodyComplete(int result);
  int DoTCPRestart();
  int DoTCPRestartComplete(int result);

  CompletionCallback io_callback_;
  State next_state_;

  
  CompletionCallback user_callback_;

  HttpRequestInfo request_;
  HttpResponseInfo response_;

  scoped_refptr<GrowableIOBuffer> parser_buf_;
  scoped_ptr<HttpStreamParser> http_stream_parser_;
  scoped_refptr<IOBuffer> drain_buf_;

  
  scoped_ptr<ClientSocketHandle> transport_;

  
  
  const HostPortPair endpoint_;
  scoped_refptr<HttpAuthController> auth_;
  const bool tunnel_;
  
  const bool using_spdy_;
  
  NextProto protocol_negotiated_;
  
  const bool is_https_proxy_;

  std::string request_line_;
  HttpRequestHeaders request_headers_;

  
  bool redirect_has_load_timing_info_;
  LoadTimingInfo redirect_load_timing_info_;

  const BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(HttpProxyClientSocket);
};

}  

#endif  
