// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_PACKET_GENERATOR_H_
#define NET_QUIC_QUIC_PACKET_GENERATOR_H_

#include "net/quic/quic_packet_creator.h"

namespace net {

class QuicAckNotifier;

class NET_EXPORT_PRIVATE QuicPacketGenerator {
 public:
  class NET_EXPORT_PRIVATE DelegateInterface {
   public:
    virtual ~DelegateInterface() {}
    virtual bool ShouldGeneratePacket(TransmissionType transmission_type,
                                      HasRetransmittableData retransmittable,
                                      IsHandshake handshake) = 0;
    virtual QuicAckFrame* CreateAckFrame() = 0;
    virtual QuicCongestionFeedbackFrame* CreateFeedbackFrame() = 0;
    
    virtual bool OnSerializedPacket(const SerializedPacket& packet) = 0;
    virtual void CloseConnection(QuicErrorCode error, bool from_peer) = 0;
  };

  
  
  
  class NET_EXPORT_PRIVATE DebugDelegateInterface {
   public:
    virtual ~DebugDelegateInterface() {}

    
    virtual void OnFrameAddedToPacket(const QuicFrame& frame) = 0;
  };

  QuicPacketGenerator(DelegateInterface* delegate,
                      DebugDelegateInterface* debug_delegate,
                      QuicPacketCreator* creator);

  virtual ~QuicPacketGenerator();

  
  
  
  
  void SetShouldSendAck(bool also_send_feedback);
  void AddControlFrame(const QuicFrame& frame);

  
  
  
  
  
  
  
  QuicConsumedData ConsumeData(QuicStreamId id,
                               const IOVector& data,
                               QuicStreamOffset offset,
                               bool fin,
                               QuicAckNotifier* notifier);

  
  bool InBatchMode();
  
  void StartBatchOperations();
  
  void FinishBatchOperations();

  
  void FlushAllQueuedFrames();

  bool HasQueuedFrames() const;

  void set_debug_delegate(DebugDelegateInterface* debug_delegate) {
    debug_delegate_ = debug_delegate;
  }

 private:
  void SendQueuedFrames(bool flush);

  
  bool HasPendingFrames() const;
  
  
  bool CanSendWithNextPendingFrameAddition() const;
  
  
  bool AddNextPendingFrame();

  bool AddFrame(const QuicFrame& frame);

  void SerializeAndSendPacket();

  DelegateInterface* delegate_;
  DebugDelegateInterface* debug_delegate_;

  QuicPacketCreator* packet_creator_;
  QuicFrames queued_control_frames_;

  
  bool batch_mode_;

  
  bool should_send_ack_;
  bool should_send_feedback_;
  
  
  
  
  scoped_ptr<QuicAckFrame> pending_ack_frame_;
  scoped_ptr<QuicCongestionFeedbackFrame> pending_feedback_frame_;

  DISALLOW_COPY_AND_ASSIGN(QuicPacketGenerator);
};

}  

#endif  
