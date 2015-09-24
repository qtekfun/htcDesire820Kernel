// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_CLIENT_SOCKET_LIBEVENT_H_
#define NET_SOCKET_TCP_CLIENT_SOCKET_LIBEVENT_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/socket/client_socket.h"

struct event;  

namespace net {

class BoundNetLog;

class TCPClientSocketLibevent : public ClientSocket, base::NonThreadSafe {
 public:
  
  
  
  TCPClientSocketLibevent(const AddressList& addresses,
                          net::NetLog* net_log,
                          const net::NetLog::Source& source);

  virtual ~TCPClientSocketLibevent();

  
  
  
  
  
  void AdoptSocket(int socket);

  
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
  virtual int GetPeerAddress(AddressList* address) const;
  virtual int GetLocalAddress(IPEndPoint* address) const;
  virtual const BoundNetLog& NetLog() const;
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

  class ReadWatcher : public MessageLoopForIO::Watcher {
   public:
    explicit ReadWatcher(TCPClientSocketLibevent* socket) : socket_(socket) {}

    

    virtual void OnFileCanReadWithoutBlocking(int ) {
      if (socket_->read_callback_)
        socket_->DidCompleteRead();
    }

    virtual void OnFileCanWriteWithoutBlocking(int ) {}

   private:
    TCPClientSocketLibevent* const socket_;

    DISALLOW_COPY_AND_ASSIGN(ReadWatcher);
  };

  class WriteWatcher : public MessageLoopForIO::Watcher {
   public:
    explicit WriteWatcher(TCPClientSocketLibevent* socket) : socket_(socket) {}

    

    virtual void OnFileCanReadWithoutBlocking(int ) {}

    virtual void OnFileCanWriteWithoutBlocking(int ) {
      if (socket_->waiting_connect()) {
        socket_->DidCompleteConnect();
      } else if (socket_->write_callback_) {
        socket_->DidCompleteWrite();
      }
    }

   private:
    TCPClientSocketLibevent* const socket_;

    DISALLOW_COPY_AND_ASSIGN(WriteWatcher);
  };

  
  int DoConnectLoop(int result);
  int DoConnect();
  int DoConnectComplete(int result);

  
  
  void DoDisconnect();

  void DoReadCallback(int rv);
  void DoWriteCallback(int rv);
  void DidCompleteRead();
  void DidCompleteWrite();
  void DidCompleteConnect();

  
  bool waiting_connect() const {
    return next_connect_state_ != CONNECT_STATE_NONE;
  }

  
  int CreateSocket(const struct addrinfo* ai);

  
  int SetupSocket();

  
  void LogConnectCompletion(int net_error);

  
  int InternalWrite(IOBuffer* buf, int buf_len);

  int socket_;

  
  AddressList addresses_;

  
  const struct addrinfo* current_ai_;

  
  MessageLoopForIO::FileDescriptorWatcher read_socket_watcher_;
  MessageLoopForIO::FileDescriptorWatcher write_socket_watcher_;

  
  ReadWatcher read_watcher_;
  WriteWatcher write_watcher_;

  
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;

  
  scoped_refptr<IOBuffer> write_buf_;
  int write_buf_len_;

  
  CompletionCallback* read_callback_;

  
  CompletionCallback* write_callback_;

  
  ConnectState next_connect_state_;

  
  int connect_os_error_;

  BoundNetLog net_log_;

  
  bool previously_disconnected_;

  
  
  UseHistory use_history_;

  
  bool use_tcp_fastopen_;

  
  bool tcp_fastopen_connected_;

#ifdef ANDROID
  
  bool wait_for_connect_;
  bool valid_uid_;
  uid_t calling_uid_;
#endif
  DISALLOW_COPY_AND_ASSIGN(TCPClientSocketLibevent);
};

}  

#endif  
