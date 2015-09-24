// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_SOCKET_H_
#define TOOLS_ANDROID_FORWARDER2_SOCKET_H_

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <string>
#include <vector>

#include "base/basictypes.h"

namespace forwarder2 {

class Socket {
 public:
  Socket();
  ~Socket();

  bool BindUnix(const std::string& path);
  bool BindTcp(const std::string& host, int port);
  bool ConnectUnix(const std::string& path);
  bool ConnectTcp(const std::string& host, int port);

  
  void Shutdown();

  
  void Close();
  bool IsClosed() const { return socket_ < 0; }

  bool Accept(Socket* new_socket);

  
  int GetPort();

  bool IsFdInSet(const fd_set& fds) const;
  bool AddFdToSet(fd_set* fds) const;

  
  
  
  int Read(void* buffer, size_t buffer_size);

  
  
  
  int NonBlockingRead(void* buffer, size_t buffer_size);

  
  int Write(const void* buffer, size_t count);

  
  int NonBlockingWrite(const void* buffer, size_t count);

  // Calls Read() multiple times until num_bytes is written to the provided
  
  
  
  int ReadNumBytes(void* buffer, size_t num_bytes);

  // Calls Write() multiple times until num_bytes is written. No bounds checking
  // is performed. Returns number of bytes written, which can be different from
  
  int WriteNumBytes(const void* buffer, size_t num_bytes);

  
  // terminator is not written to the socket.
  int WriteString(const std::string& buffer);

  bool has_error() const { return socket_error_; }

  
  
  
  void AddEventFd(int event_fd);

  
  
  bool DidReceiveEventOnFd(int fd) const;

  bool DidReceiveEvent() const;

  static int GetHighestFileDescriptor(const Socket& s1, const Socket& s2);

  static pid_t GetUnixDomainSocketProcessOwner(const std::string& path);

 private:
  enum EventType {
    READ,
    WRITE
  };

  union SockAddr {
    
    sockaddr_in addr4;
    
    sockaddr_in6 addr6;
    
    sockaddr_un addr_un;
  };

  struct Event {
    int fd;
    bool was_fired;
  };

  bool SetNonBlocking();

  
  bool InitTcpSocket(const std::string& host, int port);
  bool InitUnixSocket(const std::string& path);
  bool BindAndListen();
  bool Connect();

  bool Resolve(const std::string& host);
  bool InitSocketInternal();
  void SetSocketError();

  
  
  bool WaitForEvent(EventType type, int timeout_secs);

  int socket_;
  int port_;
  bool socket_error_;

  
  int family_;

  SockAddr addr_;

  
  sockaddr* addr_ptr_;
  
  socklen_t addr_len_;

  
  
  std::vector<Event> events_;

  DISALLOW_COPY_AND_ASSIGN(Socket);
};

}  

#endif  
