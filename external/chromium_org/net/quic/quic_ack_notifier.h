// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_ACK_NOTIFIER_H_
#define NET_QUIC_QUIC_ACK_NOTIFIER_H_

#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicAckNotifier {
 public:
  class NET_EXPORT_PRIVATE DelegateInterface {
   public:
    DelegateInterface();
    virtual ~DelegateInterface();
    virtual void OnAckNotification() = 0;
  };

  explicit QuicAckNotifier(DelegateInterface* delegate);
  virtual ~QuicAckNotifier();

  
  void AddSequenceNumber(const QuicPacketSequenceNumber& sequence_number);

  
  
  void AddSequenceNumbers(const SequenceNumberSet& sequence_numbers);

  
  
  
  
  
  
  
  
  bool OnAck(QuicPacketSequenceNumber sequence_number);

  bool IsEmpty() { return sequence_numbers_.empty(); }

  
  
  
  void UpdateSequenceNumber(QuicPacketSequenceNumber old_sequence_number,
                            QuicPacketSequenceNumber new_sequence_number);

 private:
  
  
  
  DelegateInterface* delegate_;

  
  
  SequenceNumberSet sequence_numbers_;
};

};  

#endif  
