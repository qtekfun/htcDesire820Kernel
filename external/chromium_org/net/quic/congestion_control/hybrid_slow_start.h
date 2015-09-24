// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_HYBRID_SLOW_START_H_
#define NET_QUIC_CONGESTION_CONTROL_HYBRID_SLOW_START_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE HybridSlowStart {
 public:
  explicit HybridSlowStart(const QuicClock* clock);

  
  void Restart();

  
  
  
  bool EndOfRound(QuicPacketSequenceNumber ack);

  
  void Reset(QuicPacketSequenceNumber end_sequence_number);

  
  
  void Update(QuicTime::Delta rtt, QuicTime::Delta delay_min);

  
  bool Exit();

  bool started() { return started_; }

 private:
  const QuicClock* clock_;
  bool started_;
  bool found_ack_train_;
  bool found_delay_;
  QuicTime round_start_;  
  QuicPacketSequenceNumber end_sequence_number_;  
  QuicTime last_time_;  
  uint8 sample_count_;  
  QuicTime::Delta current_rtt_;  

  DISALLOW_COPY_AND_ASSIGN(HybridSlowStart);
};

}  

#endif  
