// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_CUBIC_H_
#define NET_QUIC_CONGESTION_CONTROL_CUBIC_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_time.h"

namespace net {

typedef uint32 QuicTcpCongestionWindow;

class NET_EXPORT_PRIVATE Cubic {
 public:
  explicit Cubic(const QuicClock* clock);

  
  void Reset();

  
  
  
  QuicTcpCongestionWindow CongestionWindowAfterPacketLoss(
      QuicTcpCongestionWindow current);

  
  
  
  
  QuicTcpCongestionWindow CongestionWindowAfterAck(
      QuicTcpCongestionWindow current,
      QuicTime::Delta delay_min);

 protected:
  
  
  uint32 CubeRoot(uint64 a);

 private:
  static const QuicTime::Delta MaxCubicTimeInterval() {
    return QuicTime::Delta::FromMilliseconds(30);
  }

  const QuicClock* clock_;

  
  QuicTime epoch_;

  
  QuicTime last_update_time_;

  
  QuicTcpCongestionWindow last_congestion_window_;

  
  
  
  QuicTcpCongestionWindow last_max_congestion_window_;

  
  uint32 acked_packets_count_;

  
  QuicTcpCongestionWindow estimated_tcp_congestion_window_;

  
  QuicTcpCongestionWindow origin_point_congestion_window_;

  
  uint32 time_to_origin_point_;

  
  QuicTcpCongestionWindow last_target_congestion_window_;

  DISALLOW_COPY_AND_ASSIGN(Cubic);
};

}  
#endif  
