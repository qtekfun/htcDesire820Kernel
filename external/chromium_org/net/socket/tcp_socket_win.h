// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_SOCKET_WIN_H_
#define NET_SOCKET_TCP_SOCKET_WIN_H_

#include <winsock2.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/win/object_watcher.h"
#include "net/base/address_family.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"

namespace net {

class AddressList;
class IOBuffer;
class IPEndPoint;

class NET_EXPORT TCPSocketWin : NON_EXPORTED_BASE(public base::NonThreadSafe),
                                public base::win::ObjectWatcher::Delegate  {
 public:
  TCPSocketWin(NetLog* net_log, const NetLog::Source& source);
  virtual ~TCPSocketWin();

  int Open(AddressFamily family);
  
  int AdoptConnectedSocket(SOCKET socket, const IPEndPoint& peer_address);

  int Bind(const IPEndPoint& address);

  int Listen(int backlog);
  int Accept(scoped_ptr<TCPSocketWin>* socket,
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
  int SetExclusiveAddrUse();
  bool SetReceiveBufferSize(int32 size);
  bool SetSendBufferSize(int32 size);
  bool SetKeepAlive(bool enable, int delay);
  bool SetNoDelay(bool no_delay);

  void Close();

  bool UsingTCPFastOpen() const;
  bool IsValid() const { return socket_ != INVALID_SOCKET; }

  
  
  
  
  
  
  
  
  
  
  void StartLoggingMultipleConnectAttempts(const AddressList& addresses);
  void EndLoggingMultipleConnectAttempts(int net_error);

  const BoundNetLog& net_log() const { return net_log_; }

 private:
  class Core;

  
  virtual void OnObjectSignaled(HANDLE object) OVERRIDE;

  int AcceptInternal(scoped_ptr<TCPSocketWin>* socket,
                     IPEndPoint* address);

  int DoConnect();
  void DoConnectComplete(int result);

  void LogConnectBegin(const AddressList& addresses);
  void LogConnectEnd(int net_error);

  int DoRead(IOBuffer* buf, int buf_len, const CompletionCallback& callback);
  void DidCompleteConnect();
  void DidCompleteWrite();
  void DidSignalRead();

  SOCKET socket_;

  HANDLE accept_event_;
  base::win::ObjectWatcher accept_watcher_;

  scoped_ptr<TCPSocketWin>* accept_socket_;
  IPEndPoint* accept_address_;
  CompletionCallback accept_callback_;

  
  bool waiting_connect_;
  bool waiting_read_;
  bool waiting_write_;

  
  
  
  scoped_refptr<Core> core_;

  
  CompletionCallback read_callback_;

  
  CompletionCallback write_callback_;

  scoped_ptr<IPEndPoint> peer_address_;
  
  int connect_os_error_;

  bool logging_multiple_connect_attempts_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(TCPSocketWin);
};

}  

#endif  

