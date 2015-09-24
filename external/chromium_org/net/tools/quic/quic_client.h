// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_CLIENT_H_
#define NET_TOOLS_QUIC_QUIC_CLIENT_H_

#include <string>

#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_framer.h"
#include "net/quic/quic_packet_creator.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_client_session.h"
#include "net/tools/quic/quic_spdy_client_stream.h"

namespace net {

class ProofVerifier;

namespace tools {

class QuicEpollConnectionHelper;

namespace test {
class QuicClientPeer;
}  

class QuicClient : public EpollCallbackInterface,
                   public QuicDataStream::Visitor {
 public:
  QuicClient(IPEndPoint server_address,
             const string& server_hostname,
             const QuicVersionVector& supported_versions,
             bool print_response);
  QuicClient(IPEndPoint server_address,
             const std::string& server_hostname,
             const QuicConfig& config,
             const QuicVersionVector& supported_versions);

  virtual ~QuicClient();

  
  
  
  bool Initialize();

  
  
  bool Connect();

  
  
  
  bool StartConnect();

  
  
  
  bool EncryptionBeingEstablished();

  
  void Disconnect();

  
  
  void SendRequestsAndWaitForResponse(const CommandLine::StringVector& args);

  
  
  QuicSpdyClientStream* CreateReliableClientStream();

  
  void WaitForStreamToClose(QuicStreamId id);

  
  void WaitForCryptoHandshakeConfirmed();

  
  
  bool WaitForEvents();

  
  virtual void OnRegistration(
      EpollServer* eps, int fd, int event_mask) OVERRIDE {}
  virtual void OnModification(int fd, int event_mask) OVERRIDE {}
  virtual void OnEvent(int fd, EpollEvent* event) OVERRIDE;
  
  
  
  virtual void OnUnregistration(int fd, bool replaced) OVERRIDE {}
  virtual void OnShutdown(EpollServer* eps, int fd) OVERRIDE {}

  
  virtual void OnClose(QuicDataStream* stream) OVERRIDE;

  QuicPacketCreator::Options* options();

  QuicClientSession* session() { return session_.get(); }

  bool connected() const;

  void set_bind_to_address(IPAddressNumber address) {
    bind_to_address_ = address;
  }

  IPAddressNumber bind_to_address() const { return bind_to_address_; }

  void set_local_port(int local_port) { local_port_ = local_port; }

  const IPEndPoint& server_address() const { return server_address_; }

  const IPEndPoint& client_address() const { return client_address_; }

  EpollServer* epoll_server() { return &epoll_server_; }

  int fd() { return fd_; }

  
  void set_server_hostname(const string& hostname) {
    server_hostname_ = hostname;
  }

  
  
  void SetProofVerifier(ProofVerifier* verifier) {
    
    crypto_config_.SetProofVerifier(verifier);
  }

  
  
  
  void SetChannelIDSigner(ChannelIDSigner* signer) {
    crypto_config_.SetChannelIDSigner(signer);
  }

 protected:
  virtual QuicGuid GenerateGuid();
  virtual QuicEpollConnectionHelper* CreateQuicConnectionHelper();
  virtual QuicPacketWriter* CreateQuicPacketWriter();

 private:
  friend class net::tools::test::QuicClientPeer;

  
  bool ReadAndProcessPacket();

  
  const IPEndPoint server_address_;

  
  std::string server_hostname_;

  
  
  QuicConfig config_;
  QuicCryptoClientConfig crypto_config_;

  
  IPEndPoint client_address_;

  
  IPAddressNumber bind_to_address_;
  
  int local_port_;

  
  scoped_ptr<QuicClientSession> session_;
  
  EpollServer epoll_server_;
  
  int fd_;

  
  scoped_ptr<QuicEpollConnectionHelper> helper_;

  
  scoped_ptr<QuicPacketWriter> writer_;

  
  bool initialized_;

  
  
  
  int packets_dropped_;

  
  
  bool overflow_supported_;

  
  
  
  
  
  QuicVersionVector supported_versions_;

  
  
  bool print_response_;

  DISALLOW_COPY_AND_ASSIGN(QuicClient);
};

}  
}  

#endif  
