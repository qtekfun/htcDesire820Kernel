// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_AVAILABLE_CHANNEL_ESTIMATOR_H_
#define NET_QUIC_CONGESTION_CONTROL_AVAILABLE_CHANNEL_ESTIMATOR_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

enum NET_EXPORT_PRIVATE AvailableChannelEstimateState {
  kAvailableChannelEstimateUnknown = 0,
  kAvailableChannelEstimateUncertain = 1,
  kAvailableChannelEstimateGood = 2,
  kAvailableChannelEstimateSenderLimited = 3,
};

class NET_EXPORT_PRIVATE AvailableChannelEstimator {
 public:
  explicit AvailableChannelEstimator(
      QuicPacketSequenceNumber first_sequence_number,
      QuicTime first_send_time,
      QuicTime first_receive_time);

  
  
  void OnIncomingFeedback(QuicPacketSequenceNumber sequence_number,
                          QuicByteCount packet_size,
                          QuicTime sent_time,
                          QuicTime receive_time);

  
  
  
  AvailableChannelEstimateState GetAvailableChannelEstimate(
      QuicBandwidth* bandwidth_estimate) const;

 private:
  const QuicPacketSequenceNumber first_sequence_number_;
  const QuicTime first_send_time_;
  const QuicTime first_receive_time_;
  QuicPacketSequenceNumber last_incorporated_sequence_number_;
  QuicTime last_time_sent_;
  QuicTime last_receive_time_;
  int number_of_sequence_numbers_;
  QuicByteCount received_bytes_;
};

}  
#endif  
