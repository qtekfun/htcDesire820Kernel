// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_PROXY_CLIENT_SOCKET_H_
#define NET_SPDY_SPDY_PROXY_CLIENT_SOCKET_H_

#include <list>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/load_timing_info.h"
#include "net/base/net_log.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_info.h"
#include "net/http/proxy_client_socket.h"
#include "net/spdy/spdy_http_stream.h"
#include "net/spdy/spdy_protocol.h"
#include "net/spdy/spdy_read_queue.h"
#include "net/spdy/spdy_session.h"
#include "net/spdy/spdy_stream.h"


class GURL;

namespace net {

class AddressList;
class HttpStream;
class IOBuffer;
class SpdyStream;

class NET_EXPORT_PRIVATE SpdyProxyClientSocket : public ProxyClientSocket,
                                                 public SpdyStream::Delegate {
 public:
  
  
  // any data read/written to the socket will be transferred in data
  
  SpdyProxyClientSocket(const base::WeakPtr<SpdyStream>& spdy_stream,
                        const std::string& user_agent,
                        const HostPortPair& endpoint,
                        const GURL& url,
                        const HostPortPair& proxy_server,
                        const BoundNetLog& source_net_log,
                        HttpAuthCache* auth_cache,
                        HttpAuthHandlerFactory* auth_handler_factory);


  
  virtual ~SpdyProxyClientSocket();

  
  virtual const HttpResponseInfo* GetConnectResponseInfo() const OVERRIDE;
  virtual HttpStream* CreateConnectResponseStream() OVERRIDE;
  virtual const scoped_refptr<HttpAuthController>& GetAuthController() const
      OVERRIDE;
  virtual int RestartWithAuth(const CompletionCallback& callback) OVERRIDE;
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

  
  virtual void OnRequestHeadersSent() OVERRIDE;
  virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
      const SpdyHeaderBlock& response_headers) OVERRIDE;
  virtual void OnDataReceived(scoped_ptr<SpdyBuffer> buffer) OVERRIDE;
  virtual void OnDataSent() OVERRIDE;
  virtual void OnClose(int status) OVERRIDE;

 private:
  enum State {
    STATE_DISCONNECTED,
    STATE_GENERATE_AUTH_TOKEN,
    STATE_GENERATE_AUTH_TOKEN_COMPLETE,
    STATE_SEND_REQUEST,
    STATE_SEND_REQUEST_COMPLETE,
    STATE_READ_REPLY_COMPLETE,
    STATE_OPEN,
    STATE_CLOSED
  };

  void LogBlockedTunnelResponse() const;

  void OnIOComplete(int result);

  int DoLoop(int last_io_result);
  int DoGenerateAuthToken();
  int DoGenerateAuthTokenComplete(int result);
  int DoSendRequest();
  int DoSendRequestComplete(int result);
  int DoReadReplyComplete(int result);

  
  
  size_t PopulateUserReadBuffer(char* out, size_t len);

  State next_state_;

  
  base::WeakPtr<SpdyStream> spdy_stream_;

  
  
  CompletionCallback read_callback_;
  
  CompletionCallback write_callback_;

  
  HttpRequestInfo request_;
  HttpResponseInfo response_;

  
  
  const HostPortPair endpoint_;
  scoped_refptr<HttpAuthController> auth_;

  
  SpdyReadQueue read_buffer_queue_;

  
  scoped_refptr<IOBuffer> user_buffer_;
  size_t user_buffer_len_;

  // User specified number of bytes to be written.
  int write_buffer_len_;

  
  bool was_ever_used_;

  
  bool redirect_has_load_timing_info_;
  LoadTimingInfo redirect_load_timing_info_;

  base::WeakPtrFactory<SpdyProxyClientSocket> weak_factory_;

  const BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(SpdyProxyClientSocket);
};

}  

#endif  
