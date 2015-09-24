// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_P2P_IPC_SOCKET_FACTORY_H_
#define CONTENT_RENDERER_P2P_IPC_SOCKET_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "third_party/libjingle/source/talk/p2p/base/packetsocketfactory.h"

namespace content {

class P2PSocketDispatcher;

class IpcPacketSocketFactory : public talk_base::PacketSocketFactory {
 public:
  CONTENT_EXPORT explicit IpcPacketSocketFactory(
      P2PSocketDispatcher* socket_dispatcher);
  virtual ~IpcPacketSocketFactory();

  virtual talk_base::AsyncPacketSocket* CreateUdpSocket(
      const talk_base::SocketAddress& local_address,
      int min_port, int max_port) OVERRIDE;
  virtual talk_base::AsyncPacketSocket* CreateServerTcpSocket(
      const talk_base::SocketAddress& local_address,
      int min_port,
      int max_port,
      int opts) OVERRIDE;
  virtual talk_base::AsyncPacketSocket* CreateClientTcpSocket(
      const talk_base::SocketAddress& local_address,
      const talk_base::SocketAddress& remote_address,
      const talk_base::ProxyInfo& proxy_info,
      const std::string& user_agent,
      int opts) OVERRIDE;
  virtual talk_base::AsyncResolverInterface* CreateAsyncResolver() OVERRIDE;

 private:
  P2PSocketDispatcher* socket_dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(IpcPacketSocketFactory);
};

}  

#endif  
