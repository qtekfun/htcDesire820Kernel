// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_DISPATCHER_H_
#define NET_TOOLS_QUIC_QUIC_DISPATCHER_H_

#include <list>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/ip_endpoint.h"
#include "net/base/linked_hash_map.h"
#include "net/quic/quic_blocked_writer_interface.h"
#include "net/quic/quic_packet_writer.h"
#include "net/quic/quic_protocol.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_server_session.h"
#include "net/tools/quic/quic_time_wait_list_manager.h"

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template<>
struct hash<net::QuicBlockedWriterInterface*> {
  std::size_t operator()(
      const net::QuicBlockedWriterInterface* ptr) const {
    return hash<size_t>()(reinterpret_cast<size_t>(ptr));
  }
};
}
#endif

namespace net {

class EpollServer;
class QuicConfig;
class QuicCryptoServerConfig;
class QuicSession;

namespace tools {

namespace test {
class QuicDispatcherPeer;
}  

class DeleteSessionsAlarm;
class QuicEpollConnectionHelper;

class QuicDispatcher : public QuicPacketWriter, public QuicSessionOwner {
 public:
  
  typedef linked_hash_map<QuicBlockedWriterInterface*, bool> WriteBlockedList;

  
  
  
  QuicDispatcher(const QuicConfig& config,
                 const QuicCryptoServerConfig& crypto_config,
                 const QuicVersionVector& supported_versions,
                 int fd,
                 EpollServer* epoll_server);
  virtual ~QuicDispatcher();

  
  virtual WriteResult WritePacket(
      const char* buffer, size_t buf_len,
      const IPAddressNumber& self_address,
      const IPEndPoint& peer_address,
      QuicBlockedWriterInterface* writer) OVERRIDE;
  virtual bool IsWriteBlockedDataBuffered() const OVERRIDE;

  
  
  virtual void ProcessPacket(const IPEndPoint& server_address,
                             const IPEndPoint& client_address,
                             QuicGuid guid,
                             bool has_version_flag,
                             const QuicEncryptedPacket& packet);

  
  
  
  
  virtual bool OnCanWrite();

  
  void Shutdown();

  
  virtual void OnConnectionClosed(QuicGuid guid, QuicErrorCode error) OVERRIDE;

  
  void set_fd(int fd);

  typedef base::hash_map<QuicGuid, QuicSession*> SessionMap;

  virtual QuicSession* CreateQuicSession(
      QuicGuid guid,
      const IPEndPoint& server_address,
      const IPEndPoint& client_address);

  
  void DeleteSessions();

  const SessionMap& session_map() const { return session_map_; }

  
  
  void UseWriter(QuicPacketWriter* writer);

  WriteBlockedList* write_blocked_list() { return &write_blocked_list_; }

 protected:
  const QuicConfig& config_;
  const QuicCryptoServerConfig& crypto_config_;

  QuicTimeWaitListManager* time_wait_list_manager() {
    return time_wait_list_manager_.get();
  }

  QuicEpollConnectionHelper* helper() { return helper_.get(); }
  EpollServer* epoll_server() { return epoll_server_; }

  const QuicVersionVector& supported_versions() const {
    return supported_versions_;
  }

 private:
  friend class net::tools::test::QuicDispatcherPeer;

  
  
  void CleanUpSession(SessionMap::iterator it);

  
  WriteBlockedList write_blocked_list_;

  SessionMap session_map_;

  
  scoped_ptr<QuicTimeWaitListManager> time_wait_list_manager_;

  
  scoped_ptr<DeleteSessionsAlarm> delete_sessions_alarm_;

  
  std::list<QuicSession*> closed_session_list_;

  EpollServer* epoll_server_;  

  
  int fd_;

  
  
  bool write_blocked_;

  
  scoped_ptr<QuicEpollConnectionHelper> helper_;

  
  scoped_ptr<QuicPacketWriter> writer_;

  
  
  
  
  const QuicVersionVector supported_versions_;

  DISALLOW_COPY_AND_ASSIGN(QuicDispatcher);
};

}  
}  

#endif  
