/*
 * libjingle
 * Copyright 2004--2007, Google Inc.
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

#ifndef TALK_SESSION_PHONE_MEDIAENGINE_H_
#define TALK_SESSION_PHONE_MEDIAENGINE_H_

#ifdef OSX
#include <CoreAudio/CoreAudio.h>
#endif

#include <string>
#include <vector>

#include "talk/base/sigslotrepeater.h"
#include "talk/session/phone/codec.h"
#include "talk/session/phone/devicemanager.h"
#include "talk/session/phone/mediachannel.h"
#include "talk/session/phone/videocommon.h"

namespace cricket {

class SoundclipMedia {
 public:
  enum SoundclipFlags {
    SF_LOOP = 1,
  };

  virtual ~SoundclipMedia() {}

  
  
  
  
  virtual bool PlaySound(const char *clip, int len, int flags) = 0;
};

class MediaEngine {
 public:
  
  
  enum Capabilities {
    AUDIO_RECV = 1 << 0,
    AUDIO_SEND = 1 << 1,
    VIDEO_RECV = 1 << 2,
    VIDEO_SEND = 1 << 3,
  };

  
  
  enum AudioOptions {
    ECHO_CANCELLATION = 1 << 0,
    AUTO_GAIN_CONTROL = 1 << 1,
    DEFAULT_AUDIO_OPTIONS = ECHO_CANCELLATION | AUTO_GAIN_CONTROL
  };
  enum VideoOptions {
  };

  virtual ~MediaEngine() {}
  static MediaEngine* Create();

  
  
  virtual bool Init() = 0;
  
  virtual void Terminate() = 0;
  
  virtual int GetCapabilities() = 0;

  
  
  virtual VoiceMediaChannel *CreateChannel() = 0;
  
  
  virtual VideoMediaChannel *CreateVideoChannel(
      VoiceMediaChannel* voice_media_channel) = 0;

  
  virtual SoundclipMedia *CreateSoundclip() = 0;

  
  
  virtual bool SetAudioOptions(int options) = 0;
  
  virtual bool SetVideoOptions(int options) = 0;
  
  
  virtual bool SetDefaultVideoEncoderConfig(const VideoEncoderConfig& config)
      = 0;

  
  
  virtual bool SetSoundDevices(const Device* in_device,
                               const Device* out_device) = 0;
  virtual bool SetVideoCaptureDevice(const Device* cam_device) = 0;

  
  
  virtual bool SetOutputVolume(int level) = 0;

  
  
  virtual int GetInputLevel() = 0;
  
  
  
  virtual bool SetLocalMonitor(bool enable) = 0;
  
  virtual bool SetLocalRenderer(VideoRenderer* renderer) = 0;
  
  virtual CaptureResult SetVideoCapture(bool capture) = 0;

  virtual const std::vector<AudioCodec>& audio_codecs() = 0;
  virtual const std::vector<VideoCodec>& video_codecs() = 0;
  virtual bool FindAudioCodec(const AudioCodec &codec) = 0;
  virtual bool FindVideoCodec(const VideoCodec &codec) = 0;

  
  virtual void SetVoiceLogging(int min_sev, const char* filter) = 0;
  virtual void SetVideoLogging(int min_sev, const char* filter) = 0;

  sigslot::repeater1<CaptureResult> SignalVideoCaptureResult;
};

template<class VOICE, class VIDEO>
class CompositeMediaEngine : public MediaEngine {
 public:
  CompositeMediaEngine() {}
  virtual bool Init() {
    if (!voice_.Init())
      return false;
    if (!video_.Init()) {
      voice_.Terminate();
      return false;
    }
    SignalVideoCaptureResult.repeat(video_.SignalCaptureResult);
    return true;
  }
  virtual void Terminate() {
    video_.Terminate();
    voice_.Terminate();
  }

  virtual int GetCapabilities() {
    return (voice_.GetCapabilities() | video_.GetCapabilities());
  }
  virtual VoiceMediaChannel *CreateChannel() {
    return voice_.CreateChannel();
  }
  virtual VideoMediaChannel *CreateVideoChannel(VoiceMediaChannel* channel) {
    return video_.CreateChannel(channel);
  }
  virtual SoundclipMedia *CreateSoundclip() {
    return voice_.CreateSoundclip();
  }

  virtual bool SetAudioOptions(int o) {
    return voice_.SetOptions(o);
  }
  virtual bool SetVideoOptions(int o) {
    return video_.SetOptions(o);
  }
  virtual bool SetDefaultVideoEncoderConfig(const VideoEncoderConfig& config) {
    return video_.SetDefaultEncoderConfig(config);
  }

  virtual bool SetSoundDevices(const Device* in_device,
                               const Device* out_device) {
    return voice_.SetDevices(in_device, out_device);
  }
  virtual bool SetVideoCaptureDevice(const Device* cam_device) {
    return video_.SetCaptureDevice(cam_device);
  }

  virtual bool SetOutputVolume(int level) {
    return voice_.SetOutputVolume(level);
  }

  virtual int GetInputLevel() {
    return voice_.GetInputLevel();
  }
  virtual bool SetLocalMonitor(bool enable) {
    return voice_.SetLocalMonitor(enable);
  }
  virtual bool SetLocalRenderer(VideoRenderer* renderer) {
    return video_.SetLocalRenderer(renderer);
  }
  virtual CaptureResult SetVideoCapture(bool capture) {
    return video_.SetCapture(capture);
  }

  virtual const std::vector<AudioCodec>& audio_codecs() {
    return voice_.codecs();
  }
  virtual const std::vector<VideoCodec>& video_codecs() {
    return video_.codecs();
  }

  virtual bool FindAudioCodec(const AudioCodec &codec) {
    return voice_.FindCodec(codec);
  }
  virtual bool FindVideoCodec(const VideoCodec &codec) {
    return video_.FindCodec(codec);
  }

  virtual void SetVoiceLogging(int min_sev, const char* filter) {
    return voice_.SetLogging(min_sev, filter);
  }
  virtual void SetVideoLogging(int min_sev, const char* filter) {
    return video_.SetLogging(min_sev, filter);
  }

 private:
  VOICE voice_;
  VIDEO video_;
};

class NullVoiceMediaChannel : public VoiceMediaChannel {
 public:
  explicit NullVoiceMediaChannel() {}
  ~NullVoiceMediaChannel() {}
  
  virtual void OnPacketReceived(talk_base::Buffer* packet) {}
  virtual void OnRtcpReceived(talk_base::Buffer* packet) {}
  virtual void SetSendSsrc(uint32 id) {}
  virtual bool SetRtcpCName(const std::string& cname) { return true; }
  virtual bool Mute(bool on) { return true; }
  virtual bool SetSendBandwidth(bool autobw, int bps) { return true; }
  virtual bool SetOptions(int options) { return true; }
  
  virtual bool SetRecvCodecs(const std::vector<AudioCodec> &codecs) {
    return true;
  }
  virtual bool SetSendCodecs(const std::vector<AudioCodec> &codecs) {
    return true;
  }
  virtual bool SetPlayout(bool playout) { return true; }
  virtual bool SetSend(SendFlags flag) { return true; }
  virtual bool AddStream(uint32 ssrc) { return true; }
  virtual bool RemoveStream(uint32 ssrc) { return true; }
  virtual bool GetActiveStreams(AudioInfo::StreamList* streams) { return true; }
  virtual int GetOutputLevel() { return 0; }
  virtual void SetRingbackTone(const char *buf, int len) {}
  virtual bool PlayRingbackTone(bool play, bool loop) { return true; }
  virtual bool PressDTMF(int event, bool playout) { return true; }
  virtual bool GetStats(VoiceMediaInfo* info) { return false; }
};

class NullVoiceEngine {
 public:
  bool Init() { return true; }
  void Terminate() {}
  int GetCapabilities() { return 0; }
  VoiceMediaChannel* CreateChannel() {
    
    
    return new NullVoiceMediaChannel();
  }
  SoundclipMedia* CreateSoundclip() {
    return NULL;
  }
  bool SetOptions(int opts) { return true; }
  bool SetDevices(const Device* in_device, const Device* out_device) {
    return true;
  }
  bool SetOutputVolume(int level) { return true; }
  int GetInputLevel() { return 0; }
  bool SetLocalMonitor(bool enable) { return true; }
  const std::vector<AudioCodec>& codecs() { return codecs_; }
  bool FindCodec(const AudioCodec&) { return false; }
  void SetLogging(int min_sev, const char* filter) {}
 private:
  std::vector<AudioCodec> codecs_;
};

class NullVideoEngine {
 public:
  bool Init() { return true; }
  void Terminate() {}
  int GetCapabilities() { return 0; }
  VideoMediaChannel* CreateChannel(VoiceMediaChannel* voice_media_channel) {
    return NULL;
  }
  bool SetOptions(int opts) { return true; }
  bool SetDefaultEncoderConfig(const VideoEncoderConfig& config) {
    return true;
  }
  bool SetCaptureDevice(const Device* cam_device) { return true; }
  bool SetLocalRenderer(VideoRenderer* renderer) { return true; }
  CaptureResult SetCapture(bool capture) { return CR_SUCCESS;  }
  const std::vector<VideoCodec>& codecs() { return codecs_; }
  bool FindCodec(const VideoCodec&) { return false; }
  void SetLogging(int min_sev, const char* filter) {}
  sigslot::signal1<CaptureResult> SignalCaptureResult;
 private:
  std::vector<VideoCodec> codecs_;
};

typedef CompositeMediaEngine<NullVoiceEngine, NullVideoEngine> NullMediaEngine;

}  

#endif  
