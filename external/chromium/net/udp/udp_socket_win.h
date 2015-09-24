// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_UDP_SOCKET_WIN_H_
#define NET_UDP_UDP_SOCKET_WIN_H_
#pragma once

#include <winsock2.h>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/win/object_watcher.h"
#include "net/base/completion_callback.h"
#include "net/base/ip_endpoint.h"
#include "net/base/io_buffer.h"
#include "net/base/net_log.h"

namespace net {

class BoundNetLog;

class UDPSocketWin : public base::NonThreadSafe {
 public:
  UDPSocketWin(net::NetLog* net_log,
               const net::NetLog::Source& source);
  virtual ~UDPSocketWin();

  
  
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

  
  bool is_connected() const { return socket_ != INVALID_SOCKET; }

 private:
  class ReadDelegate : public base::win::ObjectWatcher::Delegate {
   public:
    explicit ReadDelegate(UDPSocketWin* socket) : socket_(socket) {}
    virtual ~ReadDelegate() {}

    
    virtual void OnObjectSignaled(HANDLE object);

   private:
    UDPSocketWin* const socket_;
  };

  class WriteDelegate : public base::win::ObjectWatcher::Delegate {
   public:
    explicit WriteDelegate(UDPSocketWin* socket) : socket_(socket) {}
    virtual ~WriteDelegate() {}

    
    virtual void OnObjectSignaled(HANDLE object);

   private:
    UDPSocketWin* const socket_;
  };

  void DoReadCallback(int rv);
  void DoWriteCallback(int rv);
  void DidCompleteRead();
  void DidCompleteWrite();
  bool ProcessSuccessfulRead(int num_bytes, IPEndPoint* address);
  void ProcessSuccessfulWrite(int num_bytes);

  
  int CreateSocket(const IPEndPoint& address);

  
  
  
  int SendToOrWrite(IOBuffer* buf,
                    int buf_len,
                    const IPEndPoint* address,
                    CompletionCallback* callback);

  int InternalRecvFrom(IOBuffer* buf, int buf_len, IPEndPoint* address);
  int InternalSendTo(IOBuffer* buf, int buf_len, const IPEndPoint* address);

  SOCKET socket_;

  
  
  mutable scoped_ptr<IPEndPoint> local_address_;
  mutable scoped_ptr<IPEndPoint> remote_address_;

  
  ReadDelegate read_delegate_;
  WriteDelegate write_delegate_;

  
  base::win::ObjectWatcher read_watcher_;
  base::win::ObjectWatcher write_watcher_;

  
  OVERLAPPED read_overlapped_;
  OVERLAPPED write_overlapped_;

  
  scoped_refptr<IOBuffer> read_iobuffer_;
  struct sockaddr_storage recv_addr_storage_;
  socklen_t recv_addr_len_;
  IPEndPoint* recv_from_address_;

  
  scoped_refptr<IOBuffer> write_iobuffer_;

  
  CompletionCallback* read_callback_;

  
  CompletionCallback* write_callback_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(UDPSocketWin);
};

}  

#endif  
