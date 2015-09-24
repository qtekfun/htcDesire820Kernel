// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ASYNC_SOCKET_IO_HANDLER_H_
#define BASE_ASYNC_SOCKET_IO_HANDLER_H_

#include "base/message_loop/message_loop.h"
#include "base/sync_socket.h"
#include "base/threading/non_thread_safe.h"

namespace base {

class BASE_EXPORT AsyncSocketIoHandler
    : public NON_EXPORTED_BASE(base::NonThreadSafe),
#if defined(OS_WIN)
      public NON_EXPORTED_BASE(base::MessageLoopForIO::IOHandler) {
#else
      public NON_EXPORTED_BASE(base::MessageLoopForIO::Watcher) {
#endif
 public:
  AsyncSocketIoHandler();
  virtual ~AsyncSocketIoHandler();

  
  
  typedef base::Callback<void(int)> ReadCompleteCallback;

  
  
  
  
  bool Initialize(base::SyncSocket::Handle socket,
                  const ReadCompleteCallback& callback);

  
  
  
  
  bool Read(char* buffer, int buffer_len);

 private:
#if defined(OS_WIN)
  
  virtual void OnIOCompleted(base::MessageLoopForIO::IOContext* context,
                             DWORD bytes_transfered,
                             DWORD error) OVERRIDE;
#elif defined(OS_POSIX)
  
  virtual void OnFileCanWriteWithoutBlocking(int socket) OVERRIDE {}
  virtual void OnFileCanReadWithoutBlocking(int socket) OVERRIDE;

  void EnsureWatchingSocket();
#endif

  base::SyncSocket::Handle socket_;
#if defined(OS_WIN)
  base::MessageLoopForIO::IOContext* context_;
  bool is_pending_;
#elif defined(OS_POSIX)
  base::MessageLoopForIO::FileDescriptorWatcher socket_watcher_;
  
  
  char* pending_buffer_;
  int pending_buffer_len_;
  
  bool is_watching_;
#endif
  ReadCompleteCallback read_complete_;

  DISALLOW_COPY_AND_ASSIGN(AsyncSocketIoHandler);
};

}  

#endif  
