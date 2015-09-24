/*
 * libjingle
 * Copyright 2004--2008, Google Inc.
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

#ifndef TALK_SESSION_PHONE_CHANNELMANAGER_H_
#define TALK_SESSION_PHONE_CHANNELMANAGER_H_

#include <string>
#include <vector>

#include "talk/base/criticalsection.h"
#include "talk/base/sigslotrepeater.h"
#include "talk/base/thread.h"
#include "talk/p2p/base/session.h"
#include "talk/session/phone/voicechannel.h"
#include "talk/session/phone/mediaengine.h"
#include "talk/session/phone/devicemanager.h"

namespace cricket {

class Soundclip;
class VoiceChannel;

class ChannelManager : public talk_base::MessageHandler,
                       public sigslot::has_slots<> {
 public:
  
  explicit ChannelManager(talk_base::Thread* worker);
  
  
  ChannelManager(MediaEngine* me, DeviceManager* dm, talk_base::Thread* worker);
  ~ChannelManager();

  
  
  talk_base::Thread* worker_thread() const { return worker_thread_; }
  bool set_worker_thread(talk_base::Thread* thread) {
    if (initialized_) return false;
    worker_thread_ = thread;
    return true;
  }

  
  int GetCapabilities();

  
  
  void GetSupportedAudioCodecs(std::vector<AudioCodec>* codecs) const;
  void GetSupportedVideoCodecs(std::vector<VideoCodec>* codecs) const;

  
  
  bool FindAudioCodec(const AudioCodec& codec) const {
    return media_engine_->FindAudioCodec(codec);
  }
  bool FindVideoCodec(const VideoCodec& video_codec) const {
    return media_engine_->FindVideoCodec(video_codec);
  }

  
  bool initialized() const { return initialized_; }
  
  bool Init();
  
  bool Init(talk_base::Thread* thread) {
    return set_worker_thread(thread) && Init();
  }
  
  void Terminate();

  

  
  VoiceChannel* CreateVoiceChannel(
      BaseSession* session, const std::string& content_name, bool rtcp);
  
  void DestroyVoiceChannel(VoiceChannel* voice_channel);
  
  
  VideoChannel* CreateVideoChannel(
      BaseSession* session, const std::string& content_name, bool rtcp,
      VoiceChannel* voice_channel);
  
  void DestroyVideoChannel(VideoChannel* video_channel);

  
  Soundclip* CreateSoundclip();
  
  void DestroySoundclip(Soundclip* soundclip);

  
  bool has_channels() const {
    return (!voice_channels_.empty() || !video_channels_.empty() ||
            !soundclips_.empty());
  }

  
  bool GetAudioOptions(std::string* wave_in_device,
                       std::string* wave_out_device, int* opts);
  bool SetAudioOptions(const std::string& wave_in_device,
                       const std::string& wave_out_device, int opts);
  bool SetOutputVolume(int level);
  bool GetVideoOptions(std::string* cam_device);
  bool SetVideoOptions(const std::string& cam_device);
  bool SetDefaultVideoEncoderConfig(const VideoEncoderConfig& config);

  
  bool SetLocalMonitor(bool enable);
  bool monitoring() const { return monitoring_; }
  
  bool SetLocalRenderer(VideoRenderer* renderer);
  
  CaptureResult SetVideoCapture(bool capture);
  bool capturing() const { return capturing_; }

  
  void SetVoiceLogging(int level, const char* filter);
  void SetVideoLogging(int level, const char* filter);

  

  bool GetAudioInputDevices(std::vector<std::string>* names);
  bool GetAudioOutputDevices(std::vector<std::string>* names);
  bool GetVideoCaptureDevices(std::vector<std::string>* names);
  sigslot::repeater0<> SignalDevicesChange;
  sigslot::signal1<CaptureResult> SignalVideoCaptureResult;

 private:
  typedef std::vector<VoiceChannel*> VoiceChannels;
  typedef std::vector<VideoChannel*> VideoChannels;
  typedef std::vector<Soundclip*> Soundclips;

  void Construct();
  bool Send(uint32 id, talk_base::MessageData* pdata);
  VoiceChannel* CreateVoiceChannel_w(
      BaseSession* session, const std::string& content_name, bool rtcp);
  void DestroyVoiceChannel_w(VoiceChannel* voice_channel);
  VideoChannel* CreateVideoChannel_w(
      BaseSession* session, const std::string& content_name, bool rtcp,
      VoiceChannel* voice_channel);
  void DestroyVideoChannel_w(VideoChannel* video_channel);
  Soundclip* CreateSoundclip_w();
  void DestroySoundclip_w(Soundclip* soundclip);
  bool SetAudioOptions_w(int opts, const Device* in_dev,
                         const Device* out_dev);
  bool SetOutputVolume_w(int level);
  bool SetLocalMonitor_w(bool enable);
  bool SetVideoOptions_w(const Device* cam_device);
  bool SetDefaultVideoEncoderConfig_w(const VideoEncoderConfig& config);
  bool SetLocalRenderer_w(VideoRenderer* renderer);
  CaptureResult SetVideoCapture_w(bool capture);
  void SetMediaLogging(bool video, int level, const char* filter);
  void SetMediaLogging_w(bool video, int level, const char* filter);
  void OnVideoCaptureResult(CaptureResult result);
  void OnMessage(talk_base::Message *message);

  talk_base::CriticalSection crit_;
  talk_base::scoped_ptr<MediaEngine> media_engine_;
  talk_base::scoped_ptr<DeviceManager> device_manager_;
  bool initialized_;
  talk_base::Thread* main_thread_;
  talk_base::Thread* worker_thread_;

  VoiceChannels voice_channels_;
  VideoChannels video_channels_;
  Soundclips soundclips_;

  std::string audio_in_device_;
  std::string audio_out_device_;
  int audio_options_;
  std::string camera_device_;
  VideoEncoderConfig default_video_encoder_config_;
  VideoRenderer* local_renderer_;

  bool capturing_;
  bool monitoring_;
};

}  

#endif  
