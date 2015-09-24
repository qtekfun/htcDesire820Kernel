// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKS_CLIENT_SOCKET_H_
#define NET_SOCKET_SOCKS_CLIENT_SOCKET_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/dns/host_resolver.h"
#include "net/dns/single_request_host_resolver.h"
#include "net/socket/stream_socket.h"

namespace net {

class ClientSocketHandle;
class BoundNetLog;

class NET_EXPORT_PRIVATE SOCKSClientSocket : public StreamSocket {
 public:
  
  
  SOCKSClientSocket(scoped_ptr<ClientSocketHandle> transport_socket,
                    const HostResolver::RequestInfo& req_info,
                    RequestPriority priority,
                    HostResolver* host_resolver);

  
  virtual ~SOCKSClientSocket();

  

  
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
  FRIEND_TEST_ALL_PREFIXES(SOCKSClientSocketTest, CompleteHandshake);
  FRIEND_TEST_ALL_PREFIXES(SOCKSClientSocketTest, SOCKS4AFailedDNS);
  FRIEND_TEST_ALL_PREFIXES(SOCKSClientSocketTest, SOCKS4AIfDomainInIPv6);

  enum State {
    STATE_RESOLVE_HOST,
    STATE_RESOLVE_HOST_COMPLETE,
    STATE_HANDSHAKE_WRITE,
    STATE_HANDSHAKE_WRITE_COMPLETE,
    STATE_HANDSHAKE_READ,
    STATE_HANDSHAKE_READ_COMPLETE,
    STATE_NONE,
  };

  void DoCallback(int result);
  void OnIOComplete(int result);

  int DoLoop(int last_io_result);
  int DoResolveHost();
  int DoResolveHostComplete(int result);
  int DoHandshakeRead();
  int DoHandshakeReadComplete(int result);
  int DoHandshakeWrite();
  int DoHandshakeWriteComplete(int result);

  const std::string BuildHandshakeWriteBuffer() const;

  
  scoped_ptr<ClientSocketHandle> transport_;

  State next_state_;

  
  CompletionCallback user_callback_;

  
  
  
  scoped_refptr<IOBuffer> handshake_buf_;

  
  
  std::string buffer_;

  
  
  bool completed_handshake_;

  
  size_t bytes_sent_;
  size_t bytes_received_;

  
  SingleRequestHostResolver host_resolver_;
  AddressList addresses_;
  HostResolver::RequestInfo host_request_info_;
  RequestPriority priority_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(SOCKSClientSocket);
};

}  

#endif  
