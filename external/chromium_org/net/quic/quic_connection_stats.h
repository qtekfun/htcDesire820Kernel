// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CONNECTION_STATS_H_
#define NET_QUIC_QUIC_CONNECTION_STATS_H_

#include <ostream>

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

struct NET_EXPORT_PRIVATE QuicConnectionStats {
  QuicConnectionStats();
  ~QuicConnectionStats();

  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const QuicConnectionStats& s);

  uint64 bytes_sent;  
  uint32 packets_sent;
  uint64 stream_bytes_sent;  

  uint64 bytes_received;  
  uint32 packets_received;  
  uint64 stream_bytes_received;  

  uint64 bytes_retransmitted;
  uint32 packets_retransmitted;

  uint32 packets_revived;
  uint32 packets_dropped;  
  uint32 rto_count;

  uint32 rtt;  
  uint64 estimated_bandwidth;
  
};

}  

#endif  
