// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_VIDEO_SENDER_VIDEO_SENDER_H_
#define MEDIA_CAST_VIDEO_SENDER_VIDEO_SENDER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"
#include "media/cast/congestion_control/congestion_control.h"
#include "media/cast/net/rtp_sender/rtp_sender.h"
#include "media/cast/rtcp/rtcp.h"

namespace crypto {
  class Encryptor;
}

namespace media {
class VideoFrame;
}

namespace media {
namespace cast {

class VideoEncoder;
class LocalRtcpVideoSenderFeedback;
class LocalRtpVideoSenderStatistics;
class LocalVideoEncoderCallback;
class PacedPacketSender;

class VideoSender : public base::NonThreadSafe,
                    public base::SupportsWeakPtr<VideoSender> {
 public:
  VideoSender(scoped_refptr<CastEnvironment> cast_environment,
              const VideoSenderConfig& video_config,
              VideoEncoderController* const video_encoder_controller,
              PacedPacketSender* const paced_packet_sender);

  virtual ~VideoSender();

  
  
  
  
  void InsertRawVideoFrame(
      const scoped_refptr<media::VideoFrame>& video_frame,
      const base::TimeTicks& capture_time);

  
  
  
  
  void InsertCodedVideoFrame(const EncodedVideoFrame* video_frame,
                             const base::TimeTicks& capture_time,
                             const base::Closure callback);

  
  void IncomingRtcpPacket(const uint8* packet, size_t length,
                          const base::Closure callback);

 protected:
  
  void OnReceivedCastFeedback(const RtcpCastMessage& cast_feedback);

 private:
  friend class LocalRtcpVideoSenderFeedback;

  
  
  void ScheduleNextRtcpReport();
  void SendRtcpReport();

  
  
  
  
  
  
  void ScheduleNextResendCheck();
  void ResendCheck();

  
  
  void ScheduleNextSkippedFramesCheck();
  void SkippedFramesCheck();

  void SendEncodedVideoFrame(const EncodedVideoFrame* video_frame,
                             const base::TimeTicks& capture_time);
  void ResendFrame(uint32 resend_frame_id);
  void ReceivedAck(uint32 acked_frame_id);
  void UpdateFramesInFlight();

  void SendEncodedVideoFrameMainThread(
      scoped_ptr<EncodedVideoFrame> video_frame,
      const base::TimeTicks& capture_time);

  void InitializeTimers();

  
  
  bool EncryptVideoFrame(const EncodedVideoFrame& encoded_frame,
                         EncodedVideoFrame* encrypted_video_frame);

  const base::TimeDelta rtp_max_delay_;
  const int max_frame_rate_;

  scoped_refptr<CastEnvironment> cast_environment_;
  scoped_ptr<LocalRtcpVideoSenderFeedback> rtcp_feedback_;
  scoped_ptr<LocalRtpVideoSenderStatistics> rtp_video_sender_statistics_;
  scoped_ptr<VideoEncoder> video_encoder_;
  scoped_ptr<Rtcp> rtcp_;
  scoped_ptr<RtpSender> rtp_sender_;
  VideoEncoderController* video_encoder_controller_;
  uint8 max_unacked_frames_;
  scoped_ptr<crypto::Encryptor> encryptor_;
  std::string iv_mask_;
  int last_acked_frame_id_;
  int last_sent_frame_id_;
  int duplicate_ack_;
  base::TimeTicks last_send_time_;
  base::TimeTicks last_checked_skip_count_time_;
  int last_skip_count_;
  CongestionControl congestion_control_;

  bool initialized_;
  base::WeakPtrFactory<VideoSender> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(VideoSender);
};

}  
}  

#endif  

