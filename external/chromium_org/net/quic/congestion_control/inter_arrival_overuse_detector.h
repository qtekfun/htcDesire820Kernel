// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_OVERUSE_DETECTOR_H_
#define NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_OVERUSE_DETECTOR_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

enum NET_EXPORT_PRIVATE RateControlRegion {
  kRateControlRegionUnknown = 0,
  kRateControlRegionUnderMax = 1,
  kRateControlRegionNearMax = 2
};

enum NET_EXPORT_PRIVATE BandwidthUsage {
  kBandwidthSteady = 0,
  kBandwidthUnderUsing = 1,
  kBandwidthDraining = 2,
  kBandwidthOverUsing = 3,
};


class NET_EXPORT_PRIVATE InterArrivalOveruseDetector {
 public:
  InterArrivalOveruseDetector();

  
  
  
  
  
  
  
  
  void OnAcknowledgedPacket(QuicPacketSequenceNumber sequence_number,
                            QuicTime send_time,
                            bool last_of_send_time,
                            QuicTime receive_time);

  
  
  
  
  
  
  
  
  
  
  
  BandwidthUsage GetState(QuicTime::Delta* estimated_congestion_delay);

 private:
  struct PacketGroup {
    PacketGroup()
        : send_time(QuicTime::Zero()),
          last_receive_time(QuicTime::Zero()) {
    }
    QuicTime send_time;
    QuicTime last_receive_time;
  };

  
  
  void UpdateSendReceiveTimeOffset(QuicTime::Delta offset);

  
  void UpdateFilter(QuicTime::Delta received_delta,
                    QuicTime::Delta sent_delta);

  
  void UpdateDeltaEstimate(QuicTime::Delta residual);

  
  void DetectSlope(int64 sigma_delta);

  
  void DetectDrift(int64 sigma_delta);

  
  PacketGroup current_packet_group_;
  
  
  PacketGroup previous_packet_group_;
  
  QuicPacketSequenceNumber last_sequence_number_;
  
  int num_of_deltas_;
  
  
  
  QuicTime::Delta accumulated_deltas_;
  
  int delta_mean_;
  
  int64 delta_variance_;
  
  
  int delta_overuse_counter_;
  
  BandwidthUsage delta_estimate_;
  
  
  int slope_overuse_counter_;
  
  BandwidthUsage slope_estimate_;
  
  QuicTime::Delta send_receive_offset_;
  
  QuicTime::Delta estimated_congestion_delay_;

  DISALLOW_COPY_AND_ASSIGN(InterArrivalOveruseDetector);
};

}  

#endif  
