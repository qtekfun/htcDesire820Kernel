// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_CLIENT_SOCKET_H_
#define NET_SOCKET_TCP_CLIENT_SOCKET_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/socket/stream_socket.h"
#include "net/socket/tcp_socket.h"

namespace net {

class NET_EXPORT TCPClientSocket : public StreamSocket {
 public:
  
  
  
  TCPClientSocket(const AddressList& addresses,
                  net::NetLog* net_log,
                  const net::NetLog::Source& source);

  
  
  TCPClientSocket(scoped_ptr<TCPSocket> connected_socket,
                  const IPEndPoint& peer_address);

  virtual ~TCPClientSocket();

  
  int Bind(const IPEndPoint& address);

  
  virtual int Connect(const CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(IPEndPoint* address) const OVERRIDE;
  virtual const BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual NextProto GetNegotiatedProtocol() const OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  
  
  virtual int Read(IOBuffer* buf, int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf, int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

  virtual bool SetKeepAlive(bool enable, int delay);
  virtual bool SetNoDelay(bool no_delay);

 private:
  
  enum ConnectState {
    CONNECT_STATE_CONNECT,
    CONNECT_STATE_CONNECT_COMPLETE,
    CONNECT_STATE_NONE,
  };

  
  int DoConnectLoop(int result);
  int DoConnect();
  int DoConnectComplete(int result);

  
  
  void DoDisconnect();

  void DidCompleteConnect(int result);
  void DidCompleteReadWrite(const CompletionCallback& callback, int result);

  int OpenSocket(AddressFamily family);

  scoped_ptr<TCPSocket> socket_;

  
  
  scoped_ptr<IPEndPoint> bind_address_;

  
  AddressList addresses_;

  
  int current_address_index_;

  
  CompletionCallback connect_callback_;

  
  ConnectState next_connect_state_;

  
  bool previously_disconnected_;

  
  
  UseHistory use_history_;

  DISALLOW_COPY_AND_ASSIGN(TCPClientSocket);
};

}  

#endif  
