// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_SOCKET_LIBEVENT_H_
#define NET_SOCKET_TCP_SOCKET_LIBEVENT_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/address_family.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/socket/socket_descriptor.h"

namespace net {

class AddressList;
class IOBuffer;
class IPEndPoint;

class NET_EXPORT TCPSocketLibevent : public base::NonThreadSafe {
 public:
  TCPSocketLibevent(NetLog* net_log, const NetLog::Source& source);
  virtual ~TCPSocketLibevent();

  int Open(AddressFamily family);
  
  int AdoptConnectedSocket(int socket, const IPEndPoint& peer_address);

  int Bind(const IPEndPoint& address);

  int Listen(int backlog);
  int Accept(scoped_ptr<TCPSocketLibevent>* socket,
             IPEndPoint* address,
             const CompletionCallback& callback);

  int Connect(const IPEndPoint& address, const CompletionCallback& callback);
  bool IsConnected() const;
  bool IsConnectedAndIdle() const;

  
  
  int Read(IOBuffer* buf, int buf_len, const CompletionCallback& callback);
  int Write(IOBuffer* buf, int buf_len, const CompletionCallback& callback);

  int GetLocalAddress(IPEndPoint* address) const;
  int GetPeerAddress(IPEndPoint* address) const;

  
  
  
  int SetDefaultOptionsForServer();
  
  
  
  void SetDefaultOptionsForClient();
  int SetAddressReuse(bool allow);
  bool SetReceiveBufferSize(int32 size);
  bool SetSendBufferSize(int32 size);
  bool SetKeepAlive(bool enable, int delay);
  bool SetNoDelay(bool no_delay);

  void Close();

  bool UsingTCPFastOpen() const;
  bool IsValid() const { return socket_ != kInvalidSocket; }

  
  
  
  
  
  
  
  
  
  
  void StartLoggingMultipleConnectAttempts(const AddressList& addresses);
  void EndLoggingMultipleConnectAttempts(int net_error);

  const BoundNetLog& net_log() const { return net_log_; }

 private:
  
  enum FastOpenStatus {
    FAST_OPEN_STATUS_UNKNOWN,

    
    
    FAST_OPEN_FAST_CONNECT_RETURN,

    
    
    FAST_OPEN_SLOW_CONNECT_RETURN,

    
    
    FAST_OPEN_ERROR,

    
    
    
    FAST_OPEN_SYN_DATA_ACK,

    
    
    
    FAST_OPEN_SYN_DATA_NACK,

    
    
    
    FAST_OPEN_SYN_DATA_FAILED,

    
    
    
    
    FAST_OPEN_NO_SYN_DATA_ACK,

    
    
    
    FAST_OPEN_NO_SYN_DATA_NACK,

    
    
    FAST_OPEN_NO_SYN_DATA_FAILED,

    FAST_OPEN_MAX_VALUE
  };

  
  
  class Watcher: public base::MessageLoopForIO::Watcher {
   public:
    Watcher(const base::Closure& read_ready_callback,
            const base::Closure& write_ready_callback);
    virtual ~Watcher();

    
    virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
    virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;

   private:
    base::Closure read_ready_callback_;
    base::Closure write_ready_callback_;

    DISALLOW_COPY_AND_ASSIGN(Watcher);
  };

  int AcceptInternal(scoped_ptr<TCPSocketLibevent>* socket,
                     IPEndPoint* address);

  int DoConnect();
  void DoConnectComplete(int result);

  void LogConnectBegin(const AddressList& addresses);
  void LogConnectEnd(int net_error);

  void DidCompleteRead();
  void DidCompleteWrite();
  void DidCompleteConnect();
  void DidCompleteConnectOrWrite();
  void DidCompleteAccept();

  
  int InternalWrite(IOBuffer* buf, int buf_len);

  
  void RecordFastOpenStatus();

  int socket_;

  base::MessageLoopForIO::FileDescriptorWatcher accept_socket_watcher_;
  Watcher accept_watcher_;

  scoped_ptr<TCPSocketLibevent>* accept_socket_;
  IPEndPoint* accept_address_;
  CompletionCallback accept_callback_;

  
  base::MessageLoopForIO::FileDescriptorWatcher read_socket_watcher_;
  base::MessageLoopForIO::FileDescriptorWatcher write_socket_watcher_;

  
  Watcher read_watcher_;
  Watcher write_watcher_;

  
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;

  
  scoped_refptr<IOBuffer> write_buf_;
  int write_buf_len_;

  
  CompletionCallback read_callback_;

  
  CompletionCallback write_callback_;

  
  const bool use_tcp_fastopen_;

  
  bool tcp_fastopen_connected_;

  FastOpenStatus fast_open_status_;

  
  
  bool waiting_connect_;

  scoped_ptr<IPEndPoint> peer_address_;
  
  int connect_os_error_;

  bool logging_multiple_connect_attempts_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(TCPSocketLibevent);
};

}  

#endif  
