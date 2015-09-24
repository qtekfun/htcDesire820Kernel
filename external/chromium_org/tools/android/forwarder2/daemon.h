// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_DAEMON_H_
#define TOOLS_ANDROID_FORWARDER2_DAEMON_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace forwarder2 {

class Socket;

class Daemon {
 public:
  
  
  typedef int (*GetExitNotifierFDCallback)();

  class ClientDelegate {
   public:
    virtual ~ClientDelegate() {}

    
    virtual void OnDaemonReady(Socket* daemon_socket) = 0;
  };

  class ServerDelegate {
   public:
    virtual ~ServerDelegate() {}

    
    
    virtual void Init() = 0;

    virtual void OnClientConnected(scoped_ptr<Socket> client_socket) = 0;
  };

  
  
  
  
  Daemon(const std::string& log_file_path,
         const std::string& identifier,
         ClientDelegate* client_delegate,
         ServerDelegate* server_delegate,
         GetExitNotifierFDCallback get_exit_fd_callback);

  ~Daemon();

  
  
  
  bool SpawnIfNeeded();

  
  bool Kill();

 private:
  const std::string log_file_path_;
  const std::string identifier_;
  ClientDelegate* const client_delegate_;
  ServerDelegate* const server_delegate_;
  const GetExitNotifierFDCallback get_exit_fd_callback_;

  DISALLOW_COPY_AND_ASSIGN(Daemon);
};

}  

#endif  
