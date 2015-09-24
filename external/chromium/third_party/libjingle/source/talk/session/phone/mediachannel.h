/*
 * libjingle
 * Copyright 2004--2010, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_SESSION_PHONE_MEDIACHANNEL_H_
#define TALK_SESSION_PHONE_MEDIACHANNEL_H_

#include <string>
#include <vector>

#include "talk/base/basictypes.h"
#include "talk/base/sigslot.h"
#include "talk/base/socket.h"
#include "talk/session/phone/codec.h"
#include "talk/session/phone/audiomonitor.h"

namespace talk_base {
class Buffer;
}

namespace flute {
class MagicCamVideoRenderer;
}

namespace cricket {

const size_t kMinRtpPacketLen = 12;
const size_t kMinRtcpPacketLen = 4;
const size_t kMaxRtpPacketLen = 2048;

enum VoiceMediaChannelOptions {
  OPT_CONFERENCE = 0x10000,   
  OPT_ENERGYLEVEL = 0x20000,  
                              
                              

};

enum VideoMediaChannelOptions {
  OPT_INTERPOLATE = 0x10000   
                              
};

class MediaChannel : public sigslot::has_slots<> {
 public:
  class NetworkInterface {
   public:
    enum SocketType { ST_RTP, ST_RTCP };
    virtual bool SendPacket(talk_base::Buffer* packet) = 0;
    virtual bool SendRtcp(talk_base::Buffer* packet) = 0;
    virtual int SetOption(SocketType type, talk_base::Socket::Option opt,
                          int option) = 0;
    virtual ~NetworkInterface() {}
  };

  MediaChannel() : network_interface_(NULL) {}
  virtual ~MediaChannel() {}

  
  NetworkInterface *network_interface() { return network_interface_; }
  virtual void SetInterface(NetworkInterface *iface) {
    network_interface_ = iface;
  }

  
  virtual void OnPacketReceived(talk_base::Buffer* packet) = 0;
  
  virtual void OnRtcpReceived(talk_base::Buffer* packet) = 0;
  
  virtual void SetSendSsrc(uint32 id) = 0;
  
  virtual bool SetRtcpCName(const std::string& cname) = 0;
  
  virtual bool Mute(bool on) = 0;

  virtual bool SetRtpExtensionHeaders(bool enable_all) { return true; }
  virtual bool SetSendBandwidth(bool autobw, int bps) = 0;
  virtual bool SetOptions(int options) = 0;

 protected:
  NetworkInterface *network_interface_;
};

enum SendFlags {
  SEND_NOTHING,
  SEND_RINGBACKTONE,
  SEND_MICROPHONE
};

struct VoiceSenderInfo {
  uint32 ssrc;
  int bytes_sent;
  int packets_sent;
  int packets_lost;
  float fraction_lost;
  int ext_seqnum;
  int rtt_ms;
  int jitter_ms;
  int audio_level;
};

struct VoiceReceiverInfo {
  uint32 ssrc;
  int bytes_rcvd;
  int packets_rcvd;
  int packets_lost;
  float fraction_lost;
  int ext_seqnum;
  int jitter_ms;
  int audio_level;
};

struct VideoSenderInfo {
  uint32 ssrc;
  int bytes_sent;
  int packets_sent;
  int packets_cached;
  int packets_lost;
  float fraction_lost;
  int firs_rcvd;
  int nacks_rcvd;
  int rtt_ms;
  int frame_width;
  int frame_height;
  int framerate_input;
  int framerate_sent;
};

struct VideoReceiverInfo {
  uint32 ssrc;
  int bytes_rcvd;
  
  int packets_rcvd;
  int packets_lost;
  int packets_concealed;
  float fraction_lost;
  int firs_sent;
  int nacks_sent;
  int frame_width;
  int frame_height;
  int framerate_rcvd;
  int framerate_decoded;
  int framerate_output;
};

struct VoiceMediaInfo {
  void Clear() {
    senders.clear();
    receivers.clear();
  }
  std::vector<VoiceSenderInfo> senders;
  std::vector<VoiceReceiverInfo> receivers;
};

struct VideoMediaInfo {
  void Clear() {
    senders.clear();
    receivers.clear();
  }
  std::vector<VideoSenderInfo> senders;
  std::vector<VideoReceiverInfo> receivers;
};

class VoiceMediaChannel : public MediaChannel {
 public:
  enum Error {
    ERROR_NONE = 0,                       
    ERROR_OTHER,                          
    ERROR_REC_DEVICE_OPEN_FAILED = 100,   
    ERROR_REC_DEVICE_MUTED,               
    ERROR_REC_DEVICE_SILENT,              
    ERROR_REC_DEVICE_SATURATION,          
    ERROR_REC_DEVICE_REMOVED,             
    ERROR_REC_RUNTIME_ERROR,              
    ERROR_REC_SRTP_ERROR,                 
    ERROR_PLAY_DEVICE_OPEN_FAILED = 200,  
    ERROR_PLAY_DEVICE_MUTED,              
    ERROR_PLAY_DEVICE_REMOVED,            
    ERROR_PLAY_RUNTIME_ERROR,             
    ERROR_PLAY_SRTP_ERROR,                
    ERROR_PLAY_SRTP_AUTH_FAILED,          
  };

  VoiceMediaChannel() {}
  virtual ~VoiceMediaChannel() {}
  
  virtual bool SetRecvCodecs(const std::vector<AudioCodec>& codecs) = 0;
  
  virtual bool SetSendCodecs(const std::vector<AudioCodec>& codecs) = 0;
  
  virtual bool SetPlayout(bool playout) = 0;
  
  virtual bool SetSend(SendFlags flag) = 0;
  
  virtual bool AddStream(uint32 ssrc) = 0;
  
  virtual bool RemoveStream(uint32 ssrc) = 0;
  
  virtual bool GetActiveStreams(AudioInfo::StreamList* actives) = 0;
  
  virtual int GetOutputLevel() = 0;
  
  virtual void SetRingbackTone(const char *buf, int len) = 0;
  
  virtual bool PlayRingbackTone(bool play, bool loop) = 0;
  
  virtual bool PressDTMF(int event, bool playout) = 0;
  
  virtual bool GetStats(VoiceMediaInfo* info) = 0;
  
  virtual void GetLastMediaError(uint32* ssrc,
                                 VoiceMediaChannel::Error* error) {
    ASSERT(error != NULL);
    *error = ERROR_NONE;
  }

  
  
  sigslot::signal2<uint32, VoiceMediaChannel::Error> SignalMediaError;
};

class VideoFrame {
  friend class flute::MagicCamVideoRenderer;

 public:
  VideoFrame() : rendered_(false) {}

  virtual ~VideoFrame() {}

  virtual size_t GetWidth() const = 0;
  virtual size_t GetHeight() const = 0;
  virtual const uint8 *GetYPlane() const = 0;
  virtual const uint8 *GetUPlane() const = 0;
  virtual const uint8 *GetVPlane() const = 0;
  virtual uint8 *GetYPlane() = 0;
  virtual uint8 *GetUPlane() = 0;
  virtual uint8 *GetVPlane() = 0;
  virtual int32 GetYPitch() const = 0;
  virtual int32 GetUPitch() const = 0;
  virtual int32 GetVPitch() const = 0;

  
  
  virtual size_t GetPixelWidth() const = 0;
  virtual size_t GetPixelHeight() const = 0;

  
  
  virtual int64 GetElapsedTime() const = 0;
  virtual int64 GetTimeStamp() const = 0;
  virtual void SetElapsedTime(int64 elapsed_time) = 0;
  virtual void SetTimeStamp(int64 time_stamp) = 0;

  
  
  
  virtual VideoFrame *Copy() const = 0;

  
  
  // it was written or not (like snprintf). If there is insufficient space,
  // nothing is written.
  virtual size_t CopyToBuffer(uint8 *buffer, size_t size) const = 0;

  
  // Returns the frame's actual size, regardless of whether it was written or
  
  // If there is insufficient space, nothing is written.
  virtual size_t ConvertToRgbBuffer(uint32 to_fourcc, uint8 *buffer,
                                    size_t size, size_t pitch_rgb) const = 0;

  
  
  
  
  virtual void StretchToPlanes(uint8 *y, uint8 *u, uint8 *v,
                               int32 pitchY, int32 pitchU, int32 pitchV,
                               size_t width, size_t height,
                               bool interpolate, bool crop) const = 0;

  
  
  // actual size, regardless of whether it was written or not (like snprintf).
  // If there is insufficient space, nothing is written. The parameter
  
  
  
  virtual size_t StretchToBuffer(size_t w, size_t h, uint8 *buffer, size_t size,
                                 bool interpolate, bool crop) const = 0;

  
  
  
  
  virtual void StretchToFrame(VideoFrame *target, bool interpolate,
                              bool crop) const = 0;

  
  
  
  
  virtual VideoFrame *Stretch(size_t w, size_t h, bool interpolate,
                              bool crop) const = 0;

  
  static size_t SizeOf(size_t w, size_t h) {
    return w * h + ((w + 1) / 2) * ((h + 1) / 2) * 2;
  }

 protected:
  
  
  
  mutable bool rendered_;
};

class NullVideoFrame : public VideoFrame {
 public:
  virtual size_t GetWidth() const { return 0; }
  virtual size_t GetHeight() const { return 0; }
  virtual const uint8 *GetYPlane() const { return NULL; }
  virtual const uint8 *GetUPlane() const { return NULL; }
  virtual const uint8 *GetVPlane() const { return NULL; }
  virtual uint8 *GetYPlane() { return NULL; }
  virtual uint8 *GetUPlane() { return NULL; }
  virtual uint8 *GetVPlane() { return NULL; }
  virtual int32 GetYPitch() const { return 0; }
  virtual int32 GetUPitch() const { return 0; }
  virtual int32 GetVPitch() const { return 0; }

  virtual size_t GetPixelWidth() const { return 1; }
  virtual size_t GetPixelHeight() const { return 1; }
  virtual int64 GetElapsedTime() const { return 0; }
  virtual int64 GetTimeStamp() const { return 0; }
  virtual void SetElapsedTime(int64 elapsed_time) {}
  virtual void SetTimeStamp(int64 time_stamp) {}

  virtual VideoFrame *Copy() const {
    return NULL;
  }

  virtual size_t CopyToBuffer(uint8 *buffer, size_t size) const {
    return 0;
  }

  virtual size_t ConvertToRgbBuffer(uint32 to_fourcc, uint8 *buffer,
                                    size_t size, size_t pitch_rgb) const {
    return 0;
  }

  virtual void StretchToPlanes(uint8 *y, uint8 *u, uint8 *v,
                               int32 pitchY, int32 pitchU, int32 pitchV,
                               size_t width, size_t height,
                               bool interpolate, bool crop) const {
  }

  virtual size_t StretchToBuffer(size_t w, size_t h, uint8 *buffer, size_t size,
                                 bool interpolate, bool crop) const {
    return 0;
  }

  virtual void StretchToFrame(VideoFrame *target, bool interpolate,
                              bool crop) const {
  }

  virtual VideoFrame *Stretch(size_t w, size_t h, bool interpolate,
                              bool crop) const {
    return NULL;
  }
};

class VideoRenderer {
 public:
  virtual ~VideoRenderer() {}
  
  virtual bool SetSize(int width, int height, int reserved) = 0;
  
  virtual bool RenderFrame(const VideoFrame *frame) = 0;
};

class NullVideoRenderer : public VideoRenderer {
  virtual bool SetSize(int width, int height, int reserved) {
    return true;
  }
  
  virtual bool RenderFrame(const VideoFrame *frame) {
    return true;
  }
};

class VideoMediaChannel : public MediaChannel {
 public:
  enum Error {
    ERROR_NONE = 0,                       
    ERROR_OTHER,                          
    ERROR_REC_DEVICE_OPEN_FAILED = 100,   
    ERROR_REC_DEVICE_NO_DEVICE,           
    ERROR_REC_DEVICE_IN_USE,              
    ERROR_REC_DEVICE_REMOVED,             
    ERROR_REC_SRTP_ERROR,                 
    ERROR_PLAY_SRTP_ERROR = 200,          
    ERROR_PLAY_SRTP_AUTH_FAILED,          
  };

  VideoMediaChannel() { renderer_ = NULL; }
  virtual ~VideoMediaChannel() {}
  
  virtual bool SetRecvCodecs(const std::vector<VideoCodec> &codecs) = 0;
  
  virtual bool SetSendCodecs(const std::vector<VideoCodec> &codecs) = 0;
  
  virtual bool SetRender(bool render) = 0;
  
  virtual bool SetSend(bool send) = 0;
  
  virtual bool AddStream(uint32 ssrc, uint32 voice_ssrc) = 0;
  
  virtual bool RemoveStream(uint32 ssrc) = 0;
  
  
  virtual bool SetRenderer(uint32 ssrc, VideoRenderer* renderer) = 0;
  
  virtual bool GetStats(VideoMediaInfo* info) = 0;

  
  virtual bool SendIntraFrame() = 0;
  
  virtual bool RequestIntraFrame() = 0;

  sigslot::signal2<uint32, Error> SignalMediaError;

 protected:
  VideoRenderer *renderer_;
};

}  

#endif  
