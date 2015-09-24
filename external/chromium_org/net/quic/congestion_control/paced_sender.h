// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_PACED_SENDER_H_
#define NET_QUIC_CONGESTION_CONTROL_PACED_SENDER_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/congestion_control/leaky_bucket.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE PacedSender {
 public:
  PacedSender(QuicBandwidth bandwidth_estimate, QuicByteCount max_segment_size);

  void set_max_segment_size(QuicByteCount max_segment_size);

  
  void UpdateBandwidthEstimate(QuicTime now, QuicBandwidth bandwidth_estimate);

  
  void OnPacketSent(QuicTime now, QuicByteCount bytes);

  
  QuicTime::Delta TimeUntilSend(QuicTime now, QuicTime::Delta time_until_send);

 private:
  
  LeakyBucket leaky_bucket_;
  QuicBandwidth pace_;
  QuicByteCount max_segment_size_;

  DISALLOW_COPY_AND_ASSIGN(PacedSender);
};

}  

#endif  
