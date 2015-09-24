// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_PROBE_H_
#define NET_QUIC_CONGESTION_CONTROL_INTER_ARRIVAL_PROBE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/quic/congestion_control/available_channel_estimator.h"
#include "net/quic/quic_bandwidth.h"

namespace net {

class NET_EXPORT_PRIVATE InterArrivalProbe {
 public:
  explicit InterArrivalProbe(QuicByteCount max_segment_size);
  ~InterArrivalProbe();

  void set_max_segment_size(QuicByteCount max_segment_size);

  
  void OnPacketSent(QuicByteCount bytes);

  
  
  void OnAcknowledgedPacket(QuicByteCount bytes);

  
  QuicByteCount GetAvailableCongestionWindow();

  
  
  
  
  void OnIncomingFeedback(QuicPacketSequenceNumber sequence_number,
                          QuicByteCount bytes_sent,
                          QuicTime time_sent,
                          QuicTime time_received);

  
  
  
  bool GetEstimate(QuicBandwidth* available_channel_estimate);

 private:
  QuicByteCount max_segment_size_;
  scoped_ptr<AvailableChannelEstimator> available_channel_estimator_;
  QuicPacketSequenceNumber first_sequence_number_;
  bool estimate_available_;
  QuicBandwidth available_channel_estimate_;
  QuicByteCount unacked_data_;

  DISALLOW_COPY_AND_ASSIGN(InterArrivalProbe);
};

}  
#endif  
