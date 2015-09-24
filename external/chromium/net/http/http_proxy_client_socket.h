// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PROXY_CLIENT_SOCKET_H_
#define NET_HTTP_HTTP_PROXY_CLIENT_SOCKET_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_info.h"
#include "net/http/proxy_client_socket.h"

class GURL;

namespace net {

class AddressList;
class ClientSocketHandle;
class GrowableIOBuffer;
class HttpAuthCache;
class HttpAuthHandleFactory;
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
                        bool is_https_proxy);

  
  virtual ~HttpProxyClientSocket();

  
  
  
  int RestartWithAuth(CompletionCallback* callback);

  const scoped_refptr<HttpAuthController>& auth_controller() {
    return auth_;
  }

  bool using_spdy() {
    return using_spdy_;
  }

  
  virtual const HttpResponseInfo* GetConnectResponseInfo() const;
  virtual HttpStream* CreateConnectResponseStream();

  
  virtual int Connect(CompletionCallback* callback
#ifdef ANDROID
                      , bool wait_for_connect
                      , bool valid_uid
                      , uid_t calling_uid
#endif
                     );
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool IsConnectedAndIdle() const;
  virtual const BoundNetLog& NetLog() const;
  virtual void SetSubresourceSpeculation();
  virtual void SetOmniboxSpeculation();
  virtual bool WasEverUsed() const;
  virtual bool UsingTCPFastOpen() const;

  
  virtual int Read(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual int Write(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual bool SetReceiveBufferSize(int32 size);
  virtual bool SetSendBufferSize(int32 size);
  virtual int GetPeerAddress(AddressList* address) const;
  virtual int GetLocalAddress(IPEndPoint* address) const;

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

  int HandleAuthChallenge();

  void LogBlockedTunnelResponse(int response_code) const;

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

  CompletionCallbackImpl<HttpProxyClientSocket> io_callback_;
  State next_state_;

  
  CompletionCallback* user_callback_;

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
  
  const bool is_https_proxy_;

  std::string request_line_;
  HttpRequestHeaders request_headers_;

  const BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(HttpProxyClientSocket);
};

}  

#endif  
