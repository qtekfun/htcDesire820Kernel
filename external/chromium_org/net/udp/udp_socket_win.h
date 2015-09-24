// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_UDP_SOCKET_WIN_H_
#define NET_UDP_UDP_SOCKET_WIN_H_

#include <winsock2.h>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/win/object_watcher.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/rand_callback.h"
#include "net/base/ip_endpoint.h"
#include "net/base/io_buffer.h"
#include "net/base/net_log.h"
#include "net/udp/datagram_socket.h"

namespace net {

class NET_EXPORT UDPSocketWin : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  UDPSocketWin(DatagramSocket::BindType bind_type,
               const RandIntCallback& rand_int_cb,
               net::NetLog* net_log,
               const net::NetLog::Source& source);
  virtual ~UDPSocketWin();

  
  
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

  
  bool is_connected() const { return socket_ != INVALID_SOCKET; }

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

  class Core;

  void DoReadCallback(int rv);
  void DoWriteCallback(int rv);
  void DidCompleteRead();
  void DidCompleteWrite();

  
  
  
  void LogRead(int result, const char* bytes) const;
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

  
  
  bool ReceiveAddressToIPEndpoint(IPEndPoint* address) const;

  SOCKET socket_;
  int addr_family_;

  
  
  int socket_options_;

  
  uint32 multicast_interface_;

  
  
  int multicast_time_to_live_;

  
  
  DatagramSocket::BindType bind_type_;

  
  RandIntCallback rand_int_cb_;

  
  
  mutable scoped_ptr<IPEndPoint> local_address_;
  mutable scoped_ptr<IPEndPoint> remote_address_;

  
  
  
  scoped_refptr<Core> core_;

  IPEndPoint* recv_from_address_;

  
  
  scoped_ptr<IPEndPoint> send_to_address_;

  
  CompletionCallback read_callback_;

  
  CompletionCallback write_callback_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(UDPSocketWin);
};

}  

#endif  
