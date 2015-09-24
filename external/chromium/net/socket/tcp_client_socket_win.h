// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_CLIENT_SOCKET_WIN_H_
#define NET_SOCKET_TCP_CLIENT_SOCKET_WIN_H_
#pragma once

#include <winsock2.h>

#include "base/threading/non_thread_safe.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/socket/client_socket.h"

namespace net {

class BoundNetLog;

class TCPClientSocketWin : public ClientSocket, base::NonThreadSafe {
 public:
  
  
  
  TCPClientSocketWin(const AddressList& addresses,
                     net::NetLog* net_log,
                     const net::NetLog::Source& source);

  virtual ~TCPClientSocketWin();

  
  
  
  
  
  void AdoptSocket(SOCKET socket);

  
  virtual int Connect(CompletionCallback* callback
#ifdef ANDROID
                      , bool wait_for_connect
#endif
                     );
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool IsConnectedAndIdle() const;
  virtual int GetPeerAddress(AddressList* address) const;
  virtual int GetLocalAddress(IPEndPoint* address) const;
  virtual const BoundNetLog& NetLog() const { return net_log_; }
  virtual void SetSubresourceSpeculation();
  virtual void SetOmniboxSpeculation();
  virtual bool WasEverUsed() const;
  virtual bool UsingTCPFastOpen() const;

  
  
  
  virtual int Read(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual int Write(IOBuffer* buf, int buf_len, CompletionCallback* callback);

  virtual bool SetReceiveBufferSize(int32 size);
  virtual bool SetSendBufferSize(int32 size);

 private:
  
  enum ConnectState {
    CONNECT_STATE_CONNECT,
    CONNECT_STATE_CONNECT_COMPLETE,
    CONNECT_STATE_NONE,
  };

  class Core;

  
  int DoConnectLoop(int result);
  int DoConnect();
  int DoConnectComplete(int result);

  
  
  void DoDisconnect();

  
  bool waiting_connect() const {
    return next_connect_state_ != CONNECT_STATE_NONE;
  }

  
  int CreateSocket(const struct addrinfo* ai);

  
  int SetupSocket();

  
  void LogConnectCompletion(int net_error);

  void DoReadCallback(int rv);
  void DoWriteCallback(int rv);
  void DidCompleteConnect();
  void DidCompleteRead();
  void DidCompleteWrite();

  SOCKET socket_;

  
  AddressList addresses_;

  
  const struct addrinfo* current_ai_;

  
  bool waiting_read_;
  bool waiting_write_;

  
  
  
  scoped_refptr<Core> core_;

  
  CompletionCallback* read_callback_;

  
  CompletionCallback* write_callback_;

  
  ConnectState next_connect_state_;

  
  int connect_os_error_;

  BoundNetLog net_log_;

  
  bool previously_disconnected_;

  
  
  UseHistory use_history_;

  DISALLOW_COPY_AND_ASSIGN(TCPClientSocketWin);
};

}  

#endif  
