// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_LISTEN_SOCKET_H_
#define NET_BASE_LISTEN_SOCKET_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include <winsock2.h>
#endif
#include <string>
#if defined(OS_WIN)
#include "base/win/object_watcher.h"
#elif defined(OS_POSIX)
#include "base/message_loop.h"
#endif

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

#if defined(OS_POSIX)
struct event;  
typedef int SOCKET;
#endif

class ListenSocket : public base::RefCountedThreadSafe<ListenSocket>,
#if defined(OS_WIN)
                     public base::win::ObjectWatcher::Delegate
#elif defined(OS_POSIX)
                     public MessageLoopForIO::Watcher
#endif
{
 public:
  
  
  
  class ListenSocketDelegate {
   public:
    virtual ~ListenSocketDelegate() {}

    
    
    
    virtual void DidAccept(ListenSocket *server, ListenSocket *connection) = 0;
    virtual void DidRead(ListenSocket *connection,
                         const char* data,
                         int len) = 0;
    virtual void DidClose(ListenSocket *sock) = 0;
  };

  
  
  static ListenSocket* Listen(std::string ip, int port,
                              ListenSocketDelegate* del);

  
  void Send(const char* bytes, int len, bool append_linefeed = false);
  void Send(const std::string& str, bool append_linefeed = false);

  
  
  
  void PauseReads();
  void ResumeReads();

 protected:
  friend class base::RefCountedThreadSafe<ListenSocket>;

  enum WaitState {
    NOT_WAITING      = 0,
    WAITING_ACCEPT   = 1,
    WAITING_READ     = 3,
    WAITING_CLOSE    = 4
  };

  static const SOCKET kInvalidSocket;
  static const int kSocketError;

  ListenSocket(SOCKET s, ListenSocketDelegate* del);
  virtual ~ListenSocket();
  static SOCKET Listen(std::string ip, int port);
  
  static SOCKET Accept(SOCKET s);

  virtual void SendInternal(const char* bytes, int len);

  virtual void Listen();
  virtual void Accept();
  virtual void Read();
  virtual void Close();
  virtual void CloseSocket(SOCKET s);

  
  
  void WatchSocket(WaitState state);
  void UnwatchSocket();

#if defined(OS_WIN)
  
  virtual void OnObjectSignaled(HANDLE object);
  base::win::ObjectWatcher watcher_;
  HANDLE socket_event_;
#elif defined(OS_POSIX)
  
  virtual void OnFileCanReadWithoutBlocking(int fd);
  virtual void OnFileCanWriteWithoutBlocking(int fd);
  WaitState wait_state_;
  
  MessageLoopForIO::FileDescriptorWatcher watcher_;
#endif

  SOCKET socket_;
  ListenSocketDelegate *socket_delegate_;

 private:
  bool reads_paused_;
  bool has_pending_reads_;

  DISALLOW_COPY_AND_ASSIGN(ListenSocket);
};

#endif  
