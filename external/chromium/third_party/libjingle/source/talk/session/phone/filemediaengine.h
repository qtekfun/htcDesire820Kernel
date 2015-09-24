// Copyright 2004--2005, Google Inc.
//  1. Redistributions of source code must retain the above copyright notice,
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     derived from this software without specific prior written permission.

#ifndef TALK_SESSION_PHONE_FILEMEDIAENGINE_H_
#define TALK_SESSION_PHONE_FILEMEDIAENGINE_H_

#include <string>
#include <vector>

#include "talk/base/scoped_ptr.h"
#include "talk/session/phone/codec.h"
#include "talk/session/phone/mediachannel.h"
#include "talk/session/phone/mediaengine.h"

namespace talk_base {
class StreamInterface;
}

namespace cricket {

class FileMediaEngine : public MediaEngine {
 public:
  FileMediaEngine() {}
  virtual ~FileMediaEngine() {}

  
  
  void set_voice_input_filename(const std::string& filename) {
    voice_input_filename_ = filename;
  }
  void set_voice_output_filename(const std::string& filename) {
    voice_output_filename_ = filename;
  }
  void set_video_input_filename(const std::string& filename) {
    video_input_filename_ = filename;
  }
  void set_video_output_filename(const std::string& filename) {
    video_output_filename_ = filename;
  }

  
  
  void set_voice_codecs(const std::vector<AudioCodec>& codecs) {
    voice_codecs_ = codecs;
  }
  void set_video_codecs(const std::vector<VideoCodec>& codecs) {
    video_codecs_ = codecs;
  }

  
  virtual bool Init() { return true; }
  virtual void Terminate() {}
  virtual int GetCapabilities();
  virtual VoiceMediaChannel* CreateChannel();
  virtual VideoMediaChannel* CreateVideoChannel(VoiceMediaChannel* voice_ch);
  virtual SoundclipMedia* CreateSoundclip() { return NULL; }
  virtual bool SetAudioOptions(int options) { return true; }
  virtual bool SetVideoOptions(int options) { return true; }
  virtual bool SetDefaultVideoEncoderConfig(const VideoEncoderConfig& config) {
    return true;
  }
  virtual bool SetSoundDevices(const Device* in_dev, const Device* out_dev) {
    return true;
  }
  virtual bool SetVideoCaptureDevice(const Device* cam_device) { return true; }
  virtual bool SetOutputVolume(int level) { return true; }
  virtual int GetInputLevel() { return 0; }
  virtual bool SetLocalMonitor(bool enable) { return true; }
  virtual bool SetLocalRenderer(VideoRenderer* renderer) { return true; }
  
  virtual CaptureResult SetVideoCapture(bool capture) { return CR_SUCCESS; }
  virtual const std::vector<AudioCodec>& audio_codecs() {
    return voice_codecs_;
  }
  virtual const std::vector<VideoCodec>& video_codecs() {
    return video_codecs_;
  }
  virtual bool FindAudioCodec(const AudioCodec& codec) { return true; }
  virtual bool FindVideoCodec(const VideoCodec& codec) { return true; }
  virtual void SetVoiceLogging(int min_sev, const char* filter) {}
  virtual void SetVideoLogging(int min_sev, const char* filter) {}

 private:
  std::string voice_input_filename_;
  std::string voice_output_filename_;
  std::string video_input_filename_;
  std::string video_output_filename_;
  std::vector<AudioCodec> voice_codecs_;
  std::vector<VideoCodec> video_codecs_;

  DISALLOW_COPY_AND_ASSIGN(FileMediaEngine);
};

class RtpSenderReceiver;  

class FileVoiceChannel : public VoiceMediaChannel {
 public:
  FileVoiceChannel(const std::string& in_file, const std::string& out_file);
  virtual ~FileVoiceChannel();

  
  virtual bool SetRecvCodecs(const std::vector<AudioCodec>& codecs) {
    return true;
  }
  virtual bool SetSendCodecs(const std::vector<AudioCodec>& codecs);
  virtual bool SetPlayout(bool playout) { return true; }
  virtual bool SetSend(SendFlags flag);
  virtual bool AddStream(uint32 ssrc) { return true; }
  virtual bool RemoveStream(uint32 ssrc) { return true; }
  virtual bool GetActiveStreams(AudioInfo::StreamList* actives) { return true; }
  virtual int GetOutputLevel() { return 0; }
  virtual void SetRingbackTone(const char* buf, int len) {}
  virtual bool PlayRingbackTone(bool play, bool loop) { return true; }
  virtual bool PressDTMF(int event, bool playout) { return true; }
  virtual bool GetStats(VoiceMediaInfo* info) { return true; }

  
  virtual void OnPacketReceived(talk_base::Buffer* packet);
  virtual void OnRtcpReceived(talk_base::Buffer* packet) {}
  virtual void SetSendSsrc(uint32 id) {}  
  virtual bool SetRtcpCName(const std::string& cname) { return true; }
  virtual bool Mute(bool on) { return false; }
  virtual bool SetSendBandwidth(bool autobw, int bps) { return true; }
  virtual bool SetOptions(int options) { return true; }

 private:
  talk_base::scoped_ptr<RtpSenderReceiver> rtp_sender_receiver_;
  DISALLOW_COPY_AND_ASSIGN(FileVoiceChannel);
};

class FileVideoChannel : public VideoMediaChannel {
 public:
  FileVideoChannel(const std::string& in_file, const std::string& out_file);
  virtual ~FileVideoChannel();

  
  virtual bool SetRecvCodecs(const std::vector<VideoCodec>& codecs) {
    return true;
  }
  virtual bool SetSendCodecs(const std::vector<VideoCodec>& codecs);
  virtual bool SetRender(bool render) { return true; }
  virtual bool SetSend(bool send);
  virtual bool AddStream(uint32 ssrc, uint32 voice_ssrc) { return true; }
  virtual bool RemoveStream(uint32 ssrc) { return true; }
  virtual bool SetRenderer(uint32 ssrc, VideoRenderer* renderer) {
    return true;
  }
  virtual bool GetStats(VideoMediaInfo* info) { return true; }
  virtual bool SendIntraFrame() { return false; }
  virtual bool RequestIntraFrame() { return false; }

  
  virtual void OnPacketReceived(talk_base::Buffer* packet);
  virtual void OnRtcpReceived(talk_base::Buffer* packet) {}
  virtual void SetSendSsrc(uint32 id) {}  
  virtual bool SetRtcpCName(const std::string& cname) { return true; }
  virtual bool Mute(bool on) { return false; }
  virtual bool SetSendBandwidth(bool autobw, int bps) { return true; }
  virtual bool SetOptions(int options) { return true; }

 private:
  talk_base::scoped_ptr<RtpSenderReceiver> rtp_sender_receiver_;
  DISALLOW_COPY_AND_ASSIGN(FileVideoChannel);
};

}  

#endif  
