// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_AUDIO_CAPTURER_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_AUDIO_CAPTURER_H_

#include <list>
#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "content/renderer/media/tagged_list.h"
#include "content/renderer/media/webrtc_audio_device_impl.h"
#include "media/audio/audio_input_device.h"
#include "media/base/audio_capturer_source.h"

namespace media {
class AudioBus;
}

namespace content {

class WebRtcLocalAudioRenderer;
class WebRtcLocalAudioTrack;

class CONTENT_EXPORT WebRtcAudioCapturer
    : public base::RefCountedThreadSafe<WebRtcAudioCapturer>,
      NON_EXPORTED_BASE(public media::AudioCapturerSource::CaptureCallback) {
 public:
  
  
  static scoped_refptr<WebRtcAudioCapturer> CreateCapturer();

  
  
  
  
  
  bool Initialize(int render_view_id,
                  media::ChannelLayout channel_layout,
                  int sample_rate,
                  int buffer_size,
                  int session_id,
                  const std::string& device_id,
                  int paired_output_sample_rate,
                  int paired_output_frames_per_buffer,
                  int effects);

  
  
  
  
  
  
  void AddTrack(WebRtcLocalAudioTrack* track);

  
  
  
  
  void RemoveTrack(WebRtcLocalAudioTrack* track);

  
  
  
  
  void SetCapturerSource(
      const scoped_refptr<media::AudioCapturerSource>& source,
      media::ChannelLayout channel_layout,
      float sample_rate,
      int effects);

  
  
  
  void EnablePeerConnectionMode();

  
  
  void SetVolume(int volume);
  int Volume() const;
  int MaxVolume() const;

  bool is_recording() const { return running_; }

  
  
  
  
  
  
  media::AudioParameters audio_parameters() const;

  
  
  bool GetPairedOutputParameters(int* session_id,
                                 int* output_sample_rate,
                                 int* output_frames_per_buffer) const;

  const std::string& device_id() const { return device_id_; }
  int session_id() const { return session_id_; }

  
  
  
  
  void Stop();

  
  
  
  void GetAudioProcessingParams(base::TimeDelta* delay, int* volume,
                                bool* key_pressed);

 protected:
  friend class base::RefCountedThreadSafe<WebRtcAudioCapturer>;
  WebRtcAudioCapturer();
  virtual ~WebRtcAudioCapturer();

 private:
  class TrackOwner;
  typedef TaggedList<TrackOwner> TrackList;

  
  
  virtual void Capture(media::AudioBus* audio_source,
                       int audio_delay_milliseconds,
                       double volume,
                       bool key_pressed) OVERRIDE;
  virtual void OnCaptureError() OVERRIDE;

  
  
  void Reconfigure(int sample_rate, media::ChannelLayout channel_layout,
                   int effects);

  
  
  
  void Start();

  
  
  int GetBufferSize(int sample_rate) const;

  
  base::ThreadChecker thread_checker_;

  
  
  mutable base::Lock lock_;

  
  
  
  TrackList tracks_;

  
  scoped_refptr<media::AudioCapturerSource> source_;

  
  media::AudioParameters params_;

  bool running_;

  int render_view_id_;

  
  
  int hardware_buffer_size_;

  
  
  int session_id_;

  
  std::string device_id_;

  
  
  int volume_;

  
  bool peer_connection_mode_;

  int output_sample_rate_;
  int output_frames_per_buffer_;

  
  base::TimeDelta audio_delay_;
  bool key_pressed_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcAudioCapturer);
};

}  

#endif  
