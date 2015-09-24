// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_FEC_GROUP_H_
#define NET_QUIC_QUIC_FEC_GROUP_H_

#include <set>

#include "base/strings/string_piece.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicFecGroup {
 public:
  QuicFecGroup();
  ~QuicFecGroup();

  
  
  bool Update(const QuicPacketHeader& header,
              base::StringPiece decrypted_payload);

  
  
  
  
  bool UpdateFec(QuicPacketSequenceNumber fec_packet_sequence_number,
                 bool fec_packet_entropy,
                 const QuicFecData& fec);

  
  bool CanRevive() const;

  
  
  bool IsFinished() const;

  
  
  
  // written to |decrypted_payload|, or 0 if the packet could not be revived.
  size_t Revive(QuicPacketHeader* header,
                char* decrypted_payload,
                size_t decrypted_payload_len);

  
  
  bool ProtectsPacketsBefore(QuicPacketSequenceNumber num) const;

  const base::StringPiece payload_parity() const {
    return base::StringPiece(payload_parity_, payload_parity_len_);
  }

  bool entropy_parity() const {
    return entropy_parity_;
  }

  QuicPacketSequenceNumber min_protected_packet() const {
    return min_protected_packet_;
  }

  size_t NumReceivedPackets() const {
    return received_packets_.size();
  }

 private:
  bool UpdateParity(base::StringPiece payload, bool entropy);
  
  
  size_t NumMissingPackets() const;

  
  SequenceNumberSet received_packets_;
  
  
  
  QuicPacketSequenceNumber min_protected_packet_;
  
  
  
  QuicPacketSequenceNumber max_protected_packet_;
  
  char payload_parity_[kMaxPacketSize];
  size_t payload_parity_len_;
  bool entropy_parity_;

  DISALLOW_COPY_AND_ASSIGN(QuicFecGroup);
};

}  

#endif  
