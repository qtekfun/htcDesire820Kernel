// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_BITRATE_RAMP_UP_H_
#define NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_BITRATE_RAMP_UP_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE InterArrivalBitrateRampUp {
 public:
  explicit InterArrivalBitrateRampUp(const QuicClock* clock);

  
  void Reset(QuicBandwidth current_rate,
             QuicBandwidth available_channel_estimate,
             QuicBandwidth channel_estimate);

  
  void UpdateChannelEstimate(QuicBandwidth channel_estimate);

  
  QuicBandwidth GetNewBitrate(QuicBandwidth sent_bitrate);

 private:
  uint32 CalcuateTimeToOriginPoint(QuicBandwidth rate_difference) const;

  static const QuicTime::Delta MaxCubicTimeInterval() {
    return QuicTime::Delta::FromMilliseconds(30);
  }

  const QuicClock* clock_;

  QuicBandwidth current_rate_;
  QuicBandwidth channel_estimate_;
  QuicBandwidth available_channel_estimate_;
  QuicBandwidth halfway_point_;

  
  QuicTime epoch_;

  
  QuicTime last_update_time_;

  
  uint32 time_to_origin_point_;

  DISALLOW_COPY_AND_ASSIGN(InterArrivalBitrateRampUp);
};

}  
#endif  
