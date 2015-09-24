// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_SENT_ENTROPY_MANAGER_H_
#define NET_QUIC_QUIC_SENT_ENTROPY_MANAGER_H_

#include "net/base/linked_hash_map.h"
#include "net/quic/quic_framer.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicSentEntropyManager {
 public:
  QuicSentEntropyManager();
  virtual ~QuicSentEntropyManager();

  
  void RecordPacketEntropyHash(QuicPacketSequenceNumber sequence_number,
                               QuicPacketEntropyHash entropy_hash);

  QuicPacketEntropyHash EntropyHash(
      QuicPacketSequenceNumber sequence_number) const;

  
  
  bool IsValidEntropy(QuicPacketSequenceNumber sequence_number,
                      const SequenceNumberSet& missing_packets,
                      QuicPacketEntropyHash entropy_hash) const;

  
  
  void ClearEntropyBefore(QuicPacketSequenceNumber sequence_number);

 private:
  typedef linked_hash_map<QuicPacketSequenceNumber,
                          std::pair<QuicPacketEntropyHash,
                               QuicPacketEntropyHash> > SentEntropyMap;

  
  
  SentEntropyMap packets_entropy_;

  
  QuicPacketEntropyHash packets_entropy_hash_;
};

}  

#endif  
