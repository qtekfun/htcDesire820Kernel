// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_UNIX_DOMAIN_SOCKET_POSIX_H_
#define NET_SOCKET_UNIX_DOMAIN_SOCKET_POSIX_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "build/build_config.h"
#include "net/base/net_export.h"
#include "net/socket/stream_listen_socket.h"

#if defined(OS_ANDROID) || defined(OS_LINUX)
#define SOCKET_ABSTRACT_NAMESPACE_SUPPORTED
#endif

namespace net {

class NET_EXPORT UnixDomainSocket : public StreamListenSocket {
 public:
  virtual ~UnixDomainSocket();

  
  
  
  
  typedef base::Callback<bool (uid_t user_id, gid_t group_id)> AuthCallback;

  
  
  static AuthCallback NoAuthentication();

  
  
  static scoped_ptr<UnixDomainSocket> CreateAndListen(
      const std::string& path,
      StreamListenSocket::Delegate* del,
      const AuthCallback& auth_callback);

#if defined(SOCKET_ABSTRACT_NAMESPACE_SUPPORTED)
  
  
  
  static scoped_ptr<UnixDomainSocket> CreateAndListenWithAbstractNamespace(
      const std::string& path,
      const std::string& fallback_path,
      StreamListenSocket::Delegate* del,
      const AuthCallback& auth_callback);
#endif

 private:
  UnixDomainSocket(SocketDescriptor s,
                   StreamListenSocket::Delegate* del,
                   const AuthCallback& auth_callback);

  static scoped_ptr<UnixDomainSocket> CreateAndListenInternal(
      const std::string& path,
      const std::string& fallback_path,
      StreamListenSocket::Delegate* del,
      const AuthCallback& auth_callback,
      bool use_abstract_namespace);

  static SocketDescriptor CreateAndBind(const std::string& path,
                                        bool use_abstract_namespace);

  
  virtual void Accept() OVERRIDE;

  AuthCallback auth_callback_;

  DISALLOW_COPY_AND_ASSIGN(UnixDomainSocket);
};

class NET_EXPORT UnixDomainSocketFactory : public StreamListenSocketFactory {
 public:
  
  UnixDomainSocketFactory(const std::string& path,
                          const UnixDomainSocket::AuthCallback& auth_callback);
  virtual ~UnixDomainSocketFactory();

  
  virtual scoped_ptr<StreamListenSocket> CreateAndListen(
      StreamListenSocket::Delegate* delegate) const OVERRIDE;

 protected:
  const std::string path_;
  const UnixDomainSocket::AuthCallback auth_callback_;

 private:
  DISALLOW_COPY_AND_ASSIGN(UnixDomainSocketFactory);
};

#if defined(SOCKET_ABSTRACT_NAMESPACE_SUPPORTED)
class NET_EXPORT UnixDomainSocketWithAbstractNamespaceFactory
    : public UnixDomainSocketFactory {
 public:
  UnixDomainSocketWithAbstractNamespaceFactory(
      const std::string& path,
      const std::string& fallback_path,
      const UnixDomainSocket::AuthCallback& auth_callback);
  virtual ~UnixDomainSocketWithAbstractNamespaceFactory();

  
  virtual scoped_ptr<StreamListenSocket> CreateAndListen(
      StreamListenSocket::Delegate* delegate) const OVERRIDE;

 private:
  std::string fallback_path_;

  DISALLOW_COPY_AND_ASSIGN(UnixDomainSocketWithAbstractNamespaceFactory);
};
#endif

}  

#endif  
