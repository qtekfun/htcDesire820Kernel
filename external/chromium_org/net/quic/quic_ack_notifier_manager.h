// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_ACK_NOTIFIER_MANAGER_H_
#define NET_QUIC_QUIC_ACK_NOTIFIER_MANAGER_H_

#include <map>

#include "base/containers/hash_tables.h"
#include "net/quic/quic_protocol.h"

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template<>
struct hash<net::QuicAckNotifier*> {
  std::size_t operator()(const net::QuicAckNotifier* ptr) const {
    return hash<size_t>()(reinterpret_cast<size_t>(ptr));
  }
};
}
#endif

namespace net {

class QuicAckNotifier;


class NET_EXPORT_PRIVATE AckNotifierManager {
 public:
  AckNotifierManager();
  virtual ~AckNotifierManager();

  
  
  
  void OnPacketAcked(QuicPacketSequenceNumber sequence_number);

  
  
  
  void UpdateSequenceNumber(QuicPacketSequenceNumber old_sequence_number,
                            QuicPacketSequenceNumber new_sequence_number);

  
  
  
  
  
  void OnSerializedPacket(const SerializedPacket& serialized_packet);

 private:
  typedef base::hash_set<QuicAckNotifier*> AckNotifierSet;
  typedef std::map<QuicPacketSequenceNumber, AckNotifierSet> AckNotifierMap;

  
  
  
  
  
  AckNotifierSet ack_notifiers_;

  
  
  
  
  AckNotifierMap ack_notifier_map_;
};

}  

#endif  
