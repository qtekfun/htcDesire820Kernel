// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_UDP_SOCKET_LIBEVENT_H_
#define NET_UDP_UDP_SOCKET_LIBEVENT_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/rand_callback.h"
#include "net/socket/socket_descriptor.h"
#include "net/udp/datagram_socket.h"

namespace net {

class NET_EXPORT UDPSocketLibevent : public base::NonThreadSafe {
 public:
  UDPSocketLibevent(DatagramSocket::BindType bind_type,
                    const RandIntCallback& rand_int_cb,
                    net::NetLog* net_log,
                    const net::NetLog::Source& source);
  virtual ~UDPSocketLibevent();

  
  
  int Connect(const IPEndPoint& address);

  
  
  
  int Bind(const IPEndPoint& address);

  
  void Close();

  
  int GetPeerAddress(IPEndPoint* address) const;

  
  
  int GetLocalAddress(IPEndPoint* address) const;

  
  
  

  
  
  
  int Read(IOBuffer* buf, int buf_len, const CompletionCallback& callback);

  
  
  
  int Write(IOBuffer* buf, int buf_len, const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int RecvFrom(IOBuffer* buf,
               int buf_len,
               IPEndPoint* address,
               const CompletionCallback& callback);

  
  
  
  
  
  
  
  
  
  int SendTo(IOBuffer* buf,
             int buf_len,
             const IPEndPoint& address,
             const CompletionCallback& callback);

  
  bool SetReceiveBufferSize(int32 size);

  
  bool SetSendBufferSize(int32 size);

  
  bool is_connected() const { return socket_ != kInvalidSocket; }

  const BoundNetLog& NetLog() const { return net_log_; }

  
  
  
  void AllowAddressReuse();

  
  
  
  void AllowBroadcast();

  
  
  
  
  int JoinGroup(const IPAddressNumber& group_address) const;

  
  
  
  
  
  
  
  int LeaveGroup(const IPAddressNumber& group_address) const;

  
  
  
  
  int SetMulticastInterface(uint32 interface_index);

  
  
  
  
  
  int SetMulticastTimeToLive(int time_to_live);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int SetMulticastLoopbackMode(bool loopback);

  
  
  
  int SetDiffServCodePoint(DiffServCodePoint dscp);

 private:
  enum SocketOptions {
    SOCKET_OPTION_REUSE_ADDRESS  = 1 << 0,
    SOCKET_OPTION_BROADCAST      = 1 << 1,
    SOCKET_OPTION_MULTICAST_LOOP = 1 << 2
  };

  class ReadWatcher : public base::MessageLoopForIO::Watcher {
   public:
    explicit ReadWatcher(UDPSocketLibevent* socket) : socket_(socket) {}

    

    virtual void OnFileCanReadWithoutBlocking(int ) OVERRIDE;

    virtual void OnFileCanWriteWithoutBlocking(int ) OVERRIDE {}

   private:
    UDPSocketLibevent* const socket_;

    DISALLOW_COPY_AND_ASSIGN(ReadWatcher);
  };

  class WriteWatcher : public base::MessageLoopForIO::Watcher {
   public:
    explicit WriteWatcher(UDPSocketLibevent* socket) : socket_(socket) {}

    

    virtual void OnFileCanReadWithoutBlocking(int ) OVERRIDE {}

    virtual void OnFileCanWriteWithoutBlocking(int ) OVERRIDE;

   private:
    UDPSocketLibevent* const socket_;

    DISALLOW_COPY_AND_ASSIGN(WriteWatcher);
  };

  void DoReadCallback(int rv);
  void DoWriteCallback(int rv);
  void DidCompleteRead();
  void DidCompleteWrite();

  
  
  
  
  void LogRead(int result, const char* bytes, socklen_t addr_len,
               const sockaddr* addr) const;
  void LogWrite(int result, const char* bytes, const IPEndPoint* address) const;

  
  int CreateSocket(int addr_family);

  
  
  
  int SendToOrWrite(IOBuffer* buf,
                    int buf_len,
                    const IPEndPoint* address,
                    const CompletionCallback& callback);

  int InternalConnect(const IPEndPoint& address);
  int InternalRecvFrom(IOBuffer* buf, int buf_len, IPEndPoint* address);
  int InternalSendTo(IOBuffer* buf, int buf_len, const IPEndPoint* address);

  
  
  int SetSocketOptions();
  int DoBind(const IPEndPoint& address);
  
  int RandomBind(const IPAddressNumber& address);

  int socket_;
  int addr_family_;

  
  
  int socket_options_;

  
  uint32 multicast_interface_;

  
  
  int multicast_time_to_live_;

  
  
  DatagramSocket::BindType bind_type_;

  
  RandIntCallback rand_int_cb_;

  
  
  mutable scoped_ptr<IPEndPoint> local_address_;
  mutable scoped_ptr<IPEndPoint> remote_address_;

  
  base::MessageLoopForIO::FileDescriptorWatcher read_socket_watcher_;
  base::MessageLoopForIO::FileDescriptorWatcher write_socket_watcher_;

  
  ReadWatcher read_watcher_;
  WriteWatcher write_watcher_;

  
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;
  IPEndPoint* recv_from_address_;

  
  scoped_refptr<IOBuffer> write_buf_;
  int write_buf_len_;
  scoped_ptr<IPEndPoint> send_to_address_;

  
  CompletionCallback read_callback_;

  
  CompletionCallback write_callback_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(UDPSocketLibevent);
};

}  

#endif  
