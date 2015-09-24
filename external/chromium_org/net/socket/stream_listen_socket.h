// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef NET_SOCKET_STREAM_LISTEN_SOCKET_H_
#define NET_SOCKET_STREAM_LISTEN_SOCKET_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <winsock2.h>
#endif
#include <string>
#if defined(OS_WIN)
#include "base/win/object_watcher.h"
#elif defined(OS_POSIX)
#include "base/message_loop/message_loop.h"
#endif

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/socket/socket_descriptor.h"

namespace net {

class IPEndPoint;

class NET_EXPORT StreamListenSocket
    :
#if defined(OS_WIN)
      public base::win::ObjectWatcher::Delegate {
#elif defined(OS_POSIX)
      public base::MessageLoopForIO::Watcher {
#endif

 public:
  virtual ~StreamListenSocket();

  
  
  
  class Delegate {
   public:
    
    
    virtual void DidAccept(StreamListenSocket* server,
                           scoped_ptr<StreamListenSocket> connection) = 0;
    virtual void DidRead(StreamListenSocket* connection,
                         const char* data,
                         int len) = 0;
    virtual void DidClose(StreamListenSocket* sock) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  void Send(const char* bytes, int len, bool append_linefeed = false);
  void Send(const std::string& str, bool append_linefeed = false);

  
  int GetLocalAddress(IPEndPoint* address);

  static const int kSocketError;

 protected:
  enum WaitState {
    NOT_WAITING      = 0,
    WAITING_ACCEPT   = 1,
    WAITING_READ     = 2
  };

  StreamListenSocket(SocketDescriptor s, Delegate* del);

  SocketDescriptor AcceptSocket();
  virtual void Accept() = 0;

  void Listen();
  void Read();
  void Close();
  void CloseSocket();

  
  
  void WatchSocket(WaitState state);
  void UnwatchSocket();

  Delegate* const socket_delegate_;

 private:
  friend class TransportClientSocketTest;

  void SendInternal(const char* bytes, int len);

#if defined(OS_WIN)
  
  virtual void OnObjectSignaled(HANDLE object);
  base::win::ObjectWatcher watcher_;
  HANDLE socket_event_;
#elif defined(OS_POSIX)
  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;
  WaitState wait_state_;
  
  base::MessageLoopForIO::FileDescriptorWatcher watcher_;
#endif

  
  
  
  void PauseReads();
  void ResumeReads();

  const SocketDescriptor socket_;
  bool reads_paused_;
  bool has_pending_reads_;

  DISALLOW_COPY_AND_ASSIGN(StreamListenSocket);
};

class NET_EXPORT StreamListenSocketFactory {
 public:
  virtual ~StreamListenSocketFactory() {}

  
  virtual scoped_ptr<StreamListenSocket> CreateAndListen(
      StreamListenSocket::Delegate* delegate) const = 0;
};

}  

#endif  
