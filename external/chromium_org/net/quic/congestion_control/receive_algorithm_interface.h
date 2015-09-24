// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_RECEIVE_ALGORITHM_INTERFACE_H_
#define NET_QUIC_CONGESTION_CONTROL_RECEIVE_ALGORITHM_INTERFACE_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE ReceiveAlgorithmInterface {
 public:
  static ReceiveAlgorithmInterface* Create(CongestionFeedbackType type);

  virtual ~ReceiveAlgorithmInterface() {}

  
  
  virtual bool GenerateCongestionFeedback(
      QuicCongestionFeedbackFrame* feedback) = 0;

  
  
  
  
  
  
  virtual void RecordIncomingPacket(QuicByteCount bytes,
                                    QuicPacketSequenceNumber sequence_number,
                                    QuicTime timestamp,
                                    bool revived) = 0;
};

}  

#endif  
