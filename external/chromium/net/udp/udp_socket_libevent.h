// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_UDP_SOCKET_LIBEVENT_H_
#define NET_UDP_UDP_SOCKET_LIBEVENT_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_log.h"
#include "net/socket/client_socket.h"

namespace net {

class BoundNetLog;

class UDPSocketLibevent : public base::NonThreadSafe {
 public:
  UDPSocketLibevent(net::NetLog* net_log,
                    const net::NetLog::Source& source);
  virtual ~UDPSocketLibevent();

  
  
  int Connect(const IPEndPoint& address);

  
  
  
  int Bind(const IPEndPoint& address);

  
  void Close();

  
  int GetPeerAddress(IPEndPoint* address) const;

  
  
  int GetLocalAddress(IPEndPoint* address) const;

  
  
  

  
  
  
  int Read(IOBuffer* buf, int buf_len, CompletionCallback* callback);

  
  
  
  int Write(IOBuffer* buf, int buf_len, CompletionCallback* callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int RecvFrom(IOBuffer* buf,
               int buf_len,
               IPEndPoint* address,
               CompletionCallback* callback);

  
  
  
  
  
  
  
  
  
  int SendTo(IOBuffer* buf,
             int buf_len,
             const IPEndPoint& address,
             CompletionCallback* callback);

  
  bool is_connected() const { return socket_ != kInvalidSocket; }

 private:
  static const int kInvalidSocket = -1;

  class ReadWatcher : public MessageLoopForIO::Watcher {
   public:
    explicit ReadWatcher(UDPSocketLibevent* socket) : socket_(socket) {}

    

    virtual void OnFileCanReadWithoutBlocking(int ) {
      if (socket_->read_callback_)
        socket_->DidCompleteRead();
    }

    virtual void OnFileCanWriteWithoutBlocking(int ) {}

   private:
    UDPSocketLibevent* const socket_;

    DISALLOW_COPY_AND_ASSIGN(ReadWatcher);
  };

  class WriteWatcher : public MessageLoopForIO::Watcher {
   public:
    explicit WriteWatcher(UDPSocketLibevent* socket) : socket_(socket) {}

    

    virtual void OnFileCanReadWithoutBlocking(int ) {}

    virtual void OnFileCanWriteWithoutBlocking(int ) {
      if (socket_->write_callback_)
        socket_->DidCompleteWrite();
    }

   private:
    UDPSocketLibevent* const socket_;

    DISALLOW_COPY_AND_ASSIGN(WriteWatcher);
  };

  void DoReadCallback(int rv);
  void DoWriteCallback(int rv);
  void DidCompleteRead();
  void DidCompleteWrite();

  
  int CreateSocket(const IPEndPoint& address);

  
  
  
  int SendToOrWrite(IOBuffer* buf,
                    int buf_len,
                    const IPEndPoint* address,
                    CompletionCallback* callback);

  int InternalRecvFrom(IOBuffer* buf, int buf_len, IPEndPoint* address);
  int InternalSendTo(IOBuffer* buf, int buf_len, const IPEndPoint* address);

  int socket_;

  
  
  mutable scoped_ptr<IPEndPoint> local_address_;
  mutable scoped_ptr<IPEndPoint> remote_address_;

  
  MessageLoopForIO::FileDescriptorWatcher read_socket_watcher_;
  MessageLoopForIO::FileDescriptorWatcher write_socket_watcher_;

  
  ReadWatcher read_watcher_;
  WriteWatcher write_watcher_;

  
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;
  IPEndPoint* recv_from_address_;

  
  scoped_refptr<IOBuffer> write_buf_;
  int write_buf_len_;
  scoped_ptr<IPEndPoint> send_to_address_;

  
  CompletionCallback* read_callback_;

  
  CompletionCallback* write_callback_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(UDPSocketLibevent);
};

}  

#endif  
