// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_AUDIO_DEVICE_IMPL_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_AUDIO_DEVICE_IMPL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread_checker.h"
#include "content/common/content_export.h"
#include "content/renderer/media/webrtc_audio_capturer.h"
#include "content/renderer/media/webrtc_audio_device_not_impl.h"
#include "content/renderer/media/webrtc_audio_renderer.h"
#include "media/base/audio_capturer_source.h"
#include "media/base/audio_renderer_sink.h"


namespace content {

class WebRtcAudioCapturer;
class WebRtcAudioRenderer;

class WebRtcAudioRendererSource {
 public:
  
  
  virtual void RenderData(uint8* audio_data,
                          int number_of_channels,
                          int number_of_frames,
                          int audio_delay_milliseconds) = 0;

  
  virtual void SetRenderFormat(const media::AudioParameters& params) = 0;

  
  virtual void RemoveAudioRenderer(WebRtcAudioRenderer* renderer) = 0;

 protected:
  virtual ~WebRtcAudioRendererSource() {}
};

class PeerConnectionAudioSink {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int OnData(const int16* audio_data,
                     int sample_rate,
                     int number_of_channels,
                     int number_of_frames,
                     const std::vector<int>& channels,
                     int audio_delay_milliseconds,
                     int current_volume,
                     bool need_audio_processing,
                     bool key_pressed) = 0;

  
  
  
  virtual void OnSetFormat(const media::AudioParameters& params) = 0;

 protected:
 virtual ~PeerConnectionAudioSink() {}
};

class CONTENT_EXPORT WebRtcAudioDeviceImpl
    : NON_EXPORTED_BASE(public PeerConnectionAudioSink),
      NON_EXPORTED_BASE(public WebRtcAudioDeviceNotImpl),
      NON_EXPORTED_BASE(public WebRtcAudioRendererSource) {
 public:
  
  static const int kMaxVolumeLevel = 255;

  
  WebRtcAudioDeviceImpl();

  
  
  
  
  virtual int32_t AddRef() OVERRIDE;
  virtual int32_t Release() OVERRIDE;

  
  
  

  virtual int32_t RegisterAudioCallback(webrtc::AudioTransport* audio_callback)
      OVERRIDE;

  virtual int32_t Init() OVERRIDE;
  virtual int32_t Terminate() OVERRIDE;
  virtual bool Initialized() const OVERRIDE;

  virtual int32_t PlayoutIsAvailable(bool* available) OVERRIDE;
  virtual bool PlayoutIsInitialized() const OVERRIDE;
  virtual int32_t RecordingIsAvailable(bool* available) OVERRIDE;
  virtual bool RecordingIsInitialized() const OVERRIDE;

  
  virtual int32_t StartPlayout() OVERRIDE;
  virtual int32_t StopPlayout() OVERRIDE;
  virtual bool Playing() const OVERRIDE;
  virtual int32_t StartRecording() OVERRIDE;
  virtual int32_t StopRecording() OVERRIDE;
  virtual bool Recording() const OVERRIDE;

  
  virtual int32_t SetMicrophoneVolume(uint32_t volume) OVERRIDE;

  
  virtual int32_t MicrophoneVolume(uint32_t* volume) const OVERRIDE;

  virtual int32_t MaxMicrophoneVolume(uint32_t* max_volume) const OVERRIDE;
  virtual int32_t MinMicrophoneVolume(uint32_t* min_volume) const OVERRIDE;
  virtual int32_t StereoPlayoutIsAvailable(bool* available) const OVERRIDE;
  virtual int32_t StereoRecordingIsAvailable(bool* available) const OVERRIDE;
  virtual int32_t PlayoutDelay(uint16_t* delay_ms) const OVERRIDE;
  virtual int32_t RecordingDelay(uint16_t* delay_ms) const OVERRIDE;
  virtual int32_t RecordingSampleRate(uint32_t* samples_per_sec) const OVERRIDE;
  virtual int32_t PlayoutSampleRate(uint32_t* samples_per_sec) const OVERRIDE;

  
  
  bool SetAudioRenderer(WebRtcAudioRenderer* renderer);

  
  void AddAudioCapturer(const scoped_refptr<WebRtcAudioCapturer>& capturer);

  
  
  
  
  scoped_refptr<WebRtcAudioCapturer> GetDefaultCapturer() const;

  const scoped_refptr<WebRtcAudioRenderer>& renderer() const {
    return renderer_;
  }
  int output_buffer_size() const {
    return output_audio_parameters_.frames_per_buffer();
  }
  int output_channels() const {
    return output_audio_parameters_.channels();
  }
  int output_sample_rate() const {
    return output_audio_parameters_.sample_rate();
  }

 private:
  typedef std::list<scoped_refptr<WebRtcAudioCapturer> > CapturerList;

  
  virtual ~WebRtcAudioDeviceImpl();

  

  
  virtual int OnData(const int16* audio_data,
                     int sample_rate,
                     int number_of_channels,
                     int number_of_frames,
                     const std::vector<int>& channels,
                     int audio_delay_milliseconds,
                     int current_volume,
                     bool need_audio_processing,
                     bool key_pressed) OVERRIDE;

  
  virtual void OnSetFormat(const media::AudioParameters& params) OVERRIDE;

  

  
  virtual void RenderData(uint8* audio_data,
                          int number_of_channels,
                          int number_of_frames,
                          int audio_delay_milliseconds) OVERRIDE;

  
  virtual void SetRenderFormat(const media::AudioParameters& params) OVERRIDE;
  virtual void RemoveAudioRenderer(WebRtcAudioRenderer* renderer) OVERRIDE;

  
  base::ThreadChecker thread_checker_;

  int ref_count_;

  
  
  CapturerList capturers_;

  
  scoped_refptr<WebRtcAudioRenderer> renderer_;

  
  
  
  webrtc::AudioTransport* audio_transport_callback_;

  
  media::AudioParameters output_audio_parameters_;

  
  int input_delay_ms_;

  
  int output_delay_ms_;

  
  
  mutable base::Lock lock_;

  bool initialized_;
  bool playing_;
  bool recording_;

  
  base::Time start_capture_time_;
  base::Time start_render_time_;

  
  
  uint32_t microphone_volume_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcAudioDeviceImpl);
};

}  

#endif  
