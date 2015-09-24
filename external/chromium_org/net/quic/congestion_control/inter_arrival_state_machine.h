// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_STATE_MACHINE_H_
#define NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_STATE_MACHINE_H_

#include "net/base/net_export.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_time.h"

namespace net {


enum NET_EXPORT_PRIVATE InterArrivalState {
  
  
  kInterArrivalStatePacketLoss = 1,
  
  kInterArrivalStateStable = 2,
  
  
  kInterArrivalStateDelay = 3,
  
  
  kInterArrivalStateCompetingFlow = 4,
  
  
  
  kInterArrivalStateCompetingTcpFLow = 5,
};

class NET_EXPORT_PRIVATE InterArrivalStateMachine {
 public:
  explicit InterArrivalStateMachine(const QuicClock* clock);

  InterArrivalState GetInterArrivalState();

  
  void IncreaseBitrateDecision();

  
  void DecreaseBitrateDecision();

  
  
  void set_rtt(QuicTime::Delta rtt);

  
  bool PacketLossEvent();

  
  
  bool IncreasingDelayEvent();

 private:
  const QuicClock* clock_;
  InterArrivalState current_state_;
  QuicTime::Delta smoothed_rtt_;

  int decrease_event_count_;
  QuicTime last_decrease_event_;

  int increase_event_count_;
  QuicTime last_increase_event_;

  int loss_event_count_;
  QuicTime last_loss_event_;

  int delay_event_count_;
  QuicTime last_delay_event_;

  DISALLOW_COPY_AND_ASSIGN(InterArrivalStateMachine);
};

}  
#endif  
