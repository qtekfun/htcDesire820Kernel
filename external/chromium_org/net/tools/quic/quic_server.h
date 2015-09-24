// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_SERVER_H_
#define NET_TOOLS_QUIC_QUIC_SERVER_H_

#include "base/memory/scoped_ptr.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/crypto/quic_crypto_server_config.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_framer.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_dispatcher.h"

namespace net {

namespace tools {

namespace test {
class QuicServerPeer;
}  

class QuicDispatcher;

class QuicServer : public EpollCallbackInterface {
 public:
  QuicServer();
  QuicServer(const QuicConfig& config,
             const QuicVersionVector& supported_versions);

  virtual ~QuicServer();

  
  bool Listen(const IPEndPoint& address);

  
  void WaitForEvents();

  
  void Shutdown();

  
  virtual void OnRegistration(
      EpollServer* eps, int fd, int event_mask) OVERRIDE {}
  virtual void OnModification(int fd, int event_mask) OVERRIDE {}
  virtual void OnEvent(int fd, EpollEvent* event) OVERRIDE;
  virtual void OnUnregistration(int fd, bool replaced) OVERRIDE {}

  
  
  
  
  
  
  static bool ReadAndDispatchSinglePacket(int fd, int port,
                                          QuicDispatcher* dispatcher,
                                          int* packets_dropped);

  virtual void OnShutdown(EpollServer* eps, int fd) OVERRIDE {}

  
  
  
  static void MaybeDispatchPacket(QuicDispatcher* dispatcher,
                                  const QuicEncryptedPacket& packet,
                                  const IPEndPoint& server_address,
                                  const IPEndPoint& client_address);

  void SetStrikeRegisterNoStartupPeriod() {
    crypto_config_.set_strike_register_no_startup_period();
  }

  bool overflow_supported() { return overflow_supported_; }

  int packets_dropped() { return packets_dropped_; }

  int port() { return port_; }

 private:
  friend class net::tools::test::QuicServerPeer;

  
  void Initialize();

  
  scoped_ptr<QuicDispatcher> dispatcher_;
  
  EpollServer epoll_server_;

  
  int port_;

  
  int fd_;

  
  
  
  int packets_dropped_;

  
  
  bool overflow_supported_;

  
  bool use_recvmmsg_;

  
  
  QuicConfig config_;
  
  QuicCryptoServerConfig crypto_config_;

  
  
  
  
  QuicVersionVector supported_versions_;

  DISALLOW_COPY_AND_ASSIGN(QuicServer);
};

}  
}  

#endif  
