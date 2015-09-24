// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_PROXY_RESOLVING_CLIENT_SOCKET_H_
#define JINGLE_GLUE_PROXY_RESOLVING_CLIENT_SOCKET_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/proxy/proxy_info.h"
#include "net/proxy/proxy_service.h"
#include "net/socket/stream_socket.h"
#include "net/ssl/ssl_config_service.h"
#include "url/gurl.h"

namespace net {
class ClientSocketFactory;
class ClientSocketHandle;
class HttpNetworkSession;
class URLRequestContextGetter;
}  

namespace jingle_glue {

class ProxyResolvingClientSocket : public net::StreamSocket {
 public:
  
  
  
  
  
  
  ProxyResolvingClientSocket(
      net::ClientSocketFactory* socket_factory,
      const scoped_refptr<net::URLRequestContextGetter>& request_context_getter,
      const net::SSLConfig& ssl_config,
      const net::HostPortPair& dest_host_port_pair);
  virtual ~ProxyResolvingClientSocket();

  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    const net::CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;
  virtual int Connect(const net::CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual const net::BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual net::NextProto GetNegotiatedProtocol() const OVERRIDE;
  virtual bool GetSSLInfo(net::SSLInfo* ssl_info) OVERRIDE;

 private:
  
  void ProcessProxyResolveDone(int status);
  void ProcessConnectDone(int status);

  void CloseTransportSocket();
  void RunUserConnectCallback(int status);
  int ReconsiderProxyAfterError(int error);
  void ReportSuccessfulProxyConnection();

  
  net::CompletionCallback proxy_resolve_callback_;
  net::CompletionCallback connect_callback_;

  scoped_refptr<net::HttpNetworkSession> network_session_;

  
  scoped_ptr<net::ClientSocketHandle> transport_;

  const net::SSLConfig ssl_config_;
  net::ProxyService::PacRequest* pac_request_;
  net::ProxyInfo proxy_info_;
  const net::HostPortPair dest_host_port_pair_;
  const GURL proxy_url_;
  bool tried_direct_connect_fallback_;
  net::BoundNetLog bound_net_log_;

  
  net::CompletionCallback user_connect_callback_;

  base::WeakPtrFactory<ProxyResolvingClientSocket> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolvingClientSocket);
};

}  

#endif  