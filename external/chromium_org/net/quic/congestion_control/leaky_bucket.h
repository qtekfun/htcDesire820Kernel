// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_LEAKY_BUCKET_H_
#define NET_QUIC_CONGESTION_CONTROL_LEAKY_BUCKET_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE LeakyBucket {
 public:
  explicit LeakyBucket(QuicBandwidth draining_rate);

  
  void SetDrainingRate(QuicTime now, QuicBandwidth draining_rate);

  
  void Add(QuicTime now, QuicByteCount bytes);

  
  QuicTime::Delta TimeRemaining(QuicTime now);

  
  QuicByteCount BytesPending(QuicTime now);

 private:
  void Update(QuicTime now);

  QuicByteCount bytes_;
  QuicTime time_last_updated_;
  QuicBandwidth draining_rate_;

  DISALLOW_COPY_AND_ASSIGN(LeakyBucket);
};

}  

#endif  
