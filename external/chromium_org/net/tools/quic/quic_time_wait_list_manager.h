// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_TIME_WAIT_LIST_MANAGER_H_
#define NET_TOOLS_QUIC_QUIC_TIME_WAIT_LIST_MANAGER_H_

#include <deque>

#include "base/containers/hash_tables.h"
#include "base/strings/string_piece.h"
#include "net/quic/quic_blocked_writer_interface.h"
#include "net/quic/quic_framer.h"
#include "net/quic/quic_packet_writer.h"
#include "net/quic/quic_protocol.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_epoll_clock.h"

namespace net {
namespace tools {

class GuidCleanUpAlarm;

namespace test {
class QuicTimeWaitListManagerPeer;
}  

class QuicTimeWaitListManager : public QuicBlockedWriterInterface,
                                public QuicFramerVisitorInterface {
 public:
  
  
  QuicTimeWaitListManager(QuicPacketWriter* writer,
                          EpollServer* epoll_server,
                          const QuicVersionVector& supported_versions);
  virtual ~QuicTimeWaitListManager();

  
  
  
  
  
  
  void AddGuidToTimeWait(QuicGuid guid,
                         QuicVersion version,
                         QuicEncryptedPacket* close_packet);  

  
  
  bool IsGuidInTimeWait(QuicGuid guid) const;

  
  
  
  
  
  virtual void ProcessPacket(const IPEndPoint& server_address,
                             const IPEndPoint& client_address,
                             QuicGuid guid,
                             const QuicEncryptedPacket& packet);

  
  
  
  virtual bool OnCanWrite() OVERRIDE;

  
  void CleanUpOldGuids();

  
  virtual void OnError(QuicFramer* framer) OVERRIDE;
  virtual bool OnProtocolVersionMismatch(QuicVersion received_version) OVERRIDE;
  virtual bool OnUnauthenticatedHeader(const QuicPacketHeader& header) OVERRIDE;
  virtual void OnPacket() OVERRIDE {}
  virtual void OnPublicResetPacket(
      const QuicPublicResetPacket& ) OVERRIDE {}
  virtual void OnVersionNegotiationPacket(
      const QuicVersionNegotiationPacket& ) OVERRIDE {}

  virtual void OnPacketComplete() OVERRIDE {}
  
  
  virtual bool OnPacketHeader(const QuicPacketHeader& header) OVERRIDE;
  virtual void OnRevivedPacket() OVERRIDE;
  virtual void OnFecProtectedPayload(base::StringPiece ) OVERRIDE;
  virtual bool OnStreamFrame(const QuicStreamFrame& ) OVERRIDE;
  virtual bool OnAckFrame(const QuicAckFrame& ) OVERRIDE;
  virtual bool OnCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& ) OVERRIDE;
  virtual bool OnRstStreamFrame(const QuicRstStreamFrame& ) OVERRIDE;
  virtual bool OnConnectionCloseFrame(
      const QuicConnectionCloseFrame & ) OVERRIDE;
  virtual bool OnGoAwayFrame(const QuicGoAwayFrame& ) OVERRIDE;
  virtual void OnFecData(const QuicFecData& ) OVERRIDE;

 private:
  friend class test::QuicTimeWaitListManagerPeer;

  
  struct GuidAddTime;
  
  class QueuedPacket;

  
  
  bool ShouldSendResponse(int received_packet_count);

  
  
  
  QuicVersion GetQuicVersionFromGuid(QuicGuid guid);

  
  void SendPublicReset(const IPEndPoint& server_address,
                       const IPEndPoint& client_address,
                       QuicGuid guid,
                       QuicPacketSequenceNumber rejected_sequence_number);

  
  
  void SendOrQueuePacket(QueuedPacket* packet);

  
  
  
  
  void WriteToWire(QueuedPacket* packet);

  
  void SetGuidCleanUpAlarm();

  
  
  struct GuidData {
    GuidData(int num_packets_,
             QuicVersion version_,
             QuicEncryptedPacket* close_packet)
        : num_packets(num_packets_),
          version(version_),
          close_packet(close_packet) {}
    int num_packets;
    QuicVersion version;
    QuicEncryptedPacket* close_packet;
  };
  base::hash_map<QuicGuid, GuidData> guid_map_;
  typedef base::hash_map<QuicGuid, GuidData>::iterator GuidMapIterator;

  
  
  std::deque<GuidAddTime*> time_ordered_guid_list_;

  
  
  std::deque<QueuedPacket*> pending_packets_queue_;

  
  QuicFramer framer_;

  
  IPEndPoint server_address_;
  IPEndPoint client_address_;

  
  
  EpollServer* epoll_server_;

  
  const QuicTime::Delta kTimeWaitPeriod_;

  
  
  scoped_ptr<GuidCleanUpAlarm> guid_clean_up_alarm_;

  
  QuicEpollClock clock_;

  
  QuicPacketWriter* writer_;

  
  
  bool is_write_blocked_;

  DISALLOW_COPY_AND_ASSIGN(QuicTimeWaitListManager);
};

}  
}  

#endif  
