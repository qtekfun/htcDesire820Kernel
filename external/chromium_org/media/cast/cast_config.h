// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_CAST_CONFIG_H_
#define MEDIA_CAST_CAST_CONFIG_H_

#include <list>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "media/cast/cast_defines.h"

namespace media {
namespace cast {

enum RtcpMode {
  kRtcpCompound,  
  kRtcpReducedSize,  
};

enum VideoCodec {
  kVp8,
  kH264,
  kExternalVideo,
};

enum AudioCodec {
  kOpus,
  kPcm16,
  kExternalAudio,
};

struct AudioSenderConfig {
  AudioSenderConfig();

  uint32 sender_ssrc;
  uint32 incoming_feedback_ssrc;

  int rtcp_interval;
  std::string rtcp_c_name;
  RtcpMode rtcp_mode;

  int rtp_history_ms;  
  int rtp_max_delay_ms;
  int rtp_payload_type;

  bool use_external_encoder;
  int frequency;
  int channels;
  int bitrate;  
  AudioCodec codec;

  std::string aes_key;  
  std::string aes_iv_mask;  
};

struct VideoSenderConfig {
  VideoSenderConfig();

  uint32 sender_ssrc;
  uint32 incoming_feedback_ssrc;

  int rtcp_interval;
  std::string rtcp_c_name;
  RtcpMode rtcp_mode;

  int rtp_history_ms;  
  int rtp_max_delay_ms;
  int rtp_payload_type;

  bool use_external_encoder;
  int width;  
  int height;

  float congestion_control_back_off;
  int max_bitrate;
  int min_bitrate;
  int start_bitrate;
  int max_qp;
  int min_qp;
  int max_frame_rate;
  int max_number_of_video_buffers_used;  
  VideoCodec codec;
  int number_of_cores;

  std::string aes_key;  
  std::string aes_iv_mask;  
};

struct AudioReceiverConfig {
  AudioReceiverConfig();

  uint32 feedback_ssrc;
  uint32 incoming_ssrc;

  int rtcp_interval;
  std::string rtcp_c_name;
  RtcpMode rtcp_mode;

  
  int rtp_max_delay_ms;
  int rtp_payload_type;

  bool use_external_decoder;
  int frequency;
  int channels;
  AudioCodec codec;

  std::string aes_key;  
  std::string aes_iv_mask;  
};

struct VideoReceiverConfig {
  VideoReceiverConfig();

  uint32 feedback_ssrc;
  uint32 incoming_ssrc;

  int rtcp_interval;
  std::string rtcp_c_name;
  RtcpMode rtcp_mode;

  
  int rtp_max_delay_ms;
  int rtp_payload_type;

  bool use_external_decoder;
  int max_frame_rate;

  
  
  bool decoder_faster_than_max_frame_rate;
  VideoCodec codec;

  std::string aes_key;  
  std::string aes_iv_mask;  
};

struct EncodedVideoFrame {
  EncodedVideoFrame();
  ~EncodedVideoFrame();

  VideoCodec codec;
  bool key_frame;
  uint32 frame_id;
  uint32 last_referenced_frame_id;
  std::string data;
};

struct PcmAudioFrame {
  PcmAudioFrame();
  ~PcmAudioFrame();

  int channels;  
  int frequency;
  std::vector<int16> samples;
};

struct EncodedAudioFrame {
  EncodedAudioFrame();
  ~EncodedAudioFrame();

  AudioCodec codec;
  uint32 frame_id;  
  int samples;  
                
  
  static const int kMaxNumberOfSamples = 48 * 2 * 100;
  std::string data;
};

typedef std::vector<uint8> Packet;
typedef std::vector<Packet> PacketList;

class PacketSender {
 public:
  
  
  virtual bool SendPackets(const PacketList& packets) = 0;

  virtual bool SendPacket(const Packet& packet) = 0;

  virtual ~PacketSender() {}
};

class PacketReceiver : public base::RefCountedThreadSafe<PacketReceiver> {
 public:
  
  
  virtual void ReceivedPacket(const uint8* packet, size_t length,
                              const base::Closure callback) = 0;

  static void DeletePacket(const uint8* packet);

 protected:
  virtual ~PacketReceiver() {}

 private:
  friend class base::RefCountedThreadSafe<PacketReceiver>;
};

class VideoEncoderController {
 public:
  
  virtual void SetBitRate(int new_bit_rate) = 0;

  
  
  virtual void SkipNextFrame(bool skip_next_frame) = 0;

  
  virtual void GenerateKeyFrame() = 0;

  
  virtual void LatestFrameIdToReference(uint32 frame_id) = 0;

  
  virtual int NumberOfSkippedFrames() const = 0;

 protected:
  virtual ~VideoEncoderController() {}
};

}  
}  

#endif  
