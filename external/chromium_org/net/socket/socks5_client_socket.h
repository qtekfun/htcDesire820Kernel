// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKS5_CLIENT_SOCKET_H_
#define NET_SOCKET_SOCKS5_CLIENT_SOCKET_H_

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
#include "net/socket/stream_socket.h"
#include "url/gurl.h"

namespace net {

class ClientSocketHandle;
class BoundNetLog;

class NET_EXPORT_PRIVATE SOCKS5ClientSocket : public StreamSocket {
 public:
  
  
  
  
  
  
  SOCKS5ClientSocket(scoped_ptr<ClientSocketHandle> transport_socket,
                     const HostResolver::RequestInfo& req_info);

  
  virtual ~SOCKS5ClientSocket();

  

  
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
    STATE_GREET_WRITE,
    STATE_GREET_WRITE_COMPLETE,
    STATE_GREET_READ,
    STATE_GREET_READ_COMPLETE,
    STATE_HANDSHAKE_WRITE,
    STATE_HANDSHAKE_WRITE_COMPLETE,
    STATE_HANDSHAKE_READ,
    STATE_HANDSHAKE_READ_COMPLETE,
    STATE_NONE,
  };

  
  enum SocksEndPointAddressType {
    kEndPointDomain = 0x03,
    kEndPointResolvedIPv4 = 0x01,
    kEndPointResolvedIPv6 = 0x04,
  };

  static const unsigned int kGreetReadHeaderSize;
  static const unsigned int kWriteHeaderSize;
  static const unsigned int kReadHeaderSize;
  static const uint8 kSOCKS5Version;
  static const uint8 kTunnelCommand;
  static const uint8 kNullByte;

  void DoCallback(int result);
  void OnIOComplete(int result);

  int DoLoop(int last_io_result);
  int DoHandshakeRead();
  int DoHandshakeReadComplete(int result);
  int DoHandshakeWrite();
  int DoHandshakeWriteComplete(int result);
  int DoGreetRead();
  int DoGreetReadComplete(int result);
  int DoGreetWrite();
  int DoGreetWriteComplete(int result);

  
  
  int BuildHandshakeWriteBuffer(std::string* handshake) const;

  CompletionCallback io_callback_;

  
  scoped_ptr<ClientSocketHandle> transport_;

  State next_state_;

  
  CompletionCallback user_callback_;

  
  
  
  scoped_refptr<IOBuffer> handshake_buf_;

  
  
  std::string buffer_;

  
  
  bool completed_handshake_;

  
  size_t bytes_sent_;
  size_t bytes_received_;

  size_t read_header_size;

  HostResolver::RequestInfo host_request_info_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(SOCKS5ClientSocket);
};

}  

#endif  
