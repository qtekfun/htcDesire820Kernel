// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_MEDIA_SOURCE_PLAYER_H_
#define MEDIA_BASE_ANDROID_MEDIA_SOURCE_PLAYER_H_

#include <jni.h>
#include <map>
#include <string>
#include <vector>

#include "base/android/scoped_java_ref.h"
#include "base/callback.h"
#include "base/cancelable_callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "base/time/default_tick_clock.h"
#include "base/time/time.h"
#include "media/base/android/demuxer_android.h"
#include "media/base/android/media_codec_bridge.h"
#include "media/base/android/media_decoder_job.h"
#include "media/base/android/media_player_android.h"
#include "media/base/clock.h"
#include "media/base/media_export.h"

namespace media {

class AudioDecoderJob;
class AudioTimestampHelper;
class VideoDecoderJob;

class MEDIA_EXPORT MediaSourcePlayer : public MediaPlayerAndroid,
                                       public DemuxerAndroidClient {
 public:
  
  
  MediaSourcePlayer(int player_id,
                    MediaPlayerManager* manager,
                    scoped_ptr<DemuxerAndroid> demuxer);
  virtual ~MediaSourcePlayer();

  static bool IsTypeSupported(const std::vector<uint8>& scheme_uuid,
                              const std::string& security_level,
                              const std::string& container,
                              const std::vector<std::string>& codecs);

  
  virtual void SetVideoSurface(gfx::ScopedJavaSurface surface) OVERRIDE;
  virtual void Start() OVERRIDE;
  virtual void Pause(bool is_media_related_action ALLOW_UNUSED) OVERRIDE;
  virtual void SeekTo(const base::TimeDelta& timestamp) OVERRIDE;
  virtual void Release() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual int GetVideoWidth() OVERRIDE;
  virtual int GetVideoHeight() OVERRIDE;
  virtual base::TimeDelta GetCurrentTime() OVERRIDE;
  virtual base::TimeDelta GetDuration() OVERRIDE;
  virtual bool IsPlaying() OVERRIDE;
  virtual bool CanPause() OVERRIDE;
  virtual bool CanSeekForward() OVERRIDE;
  virtual bool CanSeekBackward() OVERRIDE;
  virtual bool IsPlayerReady() OVERRIDE;
  virtual void SetDrmBridge(MediaDrmBridge* drm_bridge) OVERRIDE;
  virtual void OnKeyAdded() OVERRIDE;

  
  virtual void OnDemuxerConfigsAvailable(const DemuxerConfigs& params) OVERRIDE;
  virtual void OnDemuxerDataAvailable(const DemuxerData& params) OVERRIDE;
  virtual void OnDemuxerSeekDone(
      const base::TimeDelta& actual_browser_seek_time) OVERRIDE;
  virtual void OnDemuxerDurationChanged(base::TimeDelta duration) OVERRIDE;

 private:
  
  void UpdateTimestamps(const base::TimeDelta& presentation_timestamp,
                        size_t audio_output_bytes);

  
  void StartInternal();

  
  void PlaybackCompleted(bool is_audio);

  
  void MediaDecoderCallback(
        bool is_audio, MediaCodecStatus status,
        const base::TimeDelta& presentation_timestamp,
        size_t audio_output_bytes);

  
  base::android::ScopedJavaLocalRef<jobject> GetMediaCrypto();

  
  void OnMediaCryptoReady();

  
  void ProcessPendingEvents();

  
  
  void ResetVideoDecoderJob();

  
  void ConfigureVideoDecoderJob();
  void ConfigureAudioDecoderJob();

  
  void ClearDecodingData();

  
  void DecodeMoreAudio();
  void DecodeMoreVideo();

  
  bool HasVideo();
  bool HasAudio();

  
  
  bool AudioFinished();
  bool VideoFinished();

  
  bool Seekable();

  
  void OnDecoderStarved();

  
  
  
  
  void StartStarvationCallback(const base::TimeDelta& presentation_timestamp);

  
  
  
  
  void ScheduleSeekEventAndStopDecoding(const base::TimeDelta& seek_time);

  
  
  
  
  
  
  void BrowserSeekToCurrentTime();

  
  void SetVolumeInternal();

  
  
  bool IsProtectedSurfaceRequired();

  
  
  
  
  void OnPrefetchDone();

  
  
  
  
  void set_decode_callback_for_testing(const base::Closure& test_decode_cb) {
    decode_callback_for_testing_ = test_decode_cb;
  }

  
  
  
  enum PendingEventFlags {
    NO_EVENT_PENDING = 0,
    SEEK_EVENT_PENDING = 1 << 0,
    SURFACE_CHANGE_EVENT_PENDING = 1 << 1,
    CONFIG_CHANGE_EVENT_PENDING = 1 << 2,
    PREFETCH_REQUEST_EVENT_PENDING = 1 << 3,
    PREFETCH_DONE_EVENT_PENDING = 1 << 4,
  };

  static const char* GetEventName(PendingEventFlags event);
  bool IsEventPending(PendingEventFlags event) const;
  void SetPendingEvent(PendingEventFlags event);
  void ClearPendingEvent(PendingEventFlags event);

  scoped_ptr<DemuxerAndroid> demuxer_;

  
  unsigned pending_event_;

  
  base::TimeDelta duration_;
  int width_;
  int height_;
  AudioCodec audio_codec_;
  VideoCodec video_codec_;
  int num_channels_;
  int sampling_rate_;
  
  std::vector<uint8> audio_extra_data_;
  bool reached_audio_eos_;
  bool reached_video_eos_;
  bool playing_;
  bool is_audio_encrypted_;
  bool is_video_encrypted_;
  double volume_;

  
  base::DefaultTickClock default_tick_clock_;

  
  Clock clock_;

  
  
  
  
  
  
  base::TimeTicks start_time_ticks_;
  base::TimeDelta start_presentation_timestamp_;

  
  gfx::ScopedJavaSurface surface_;

  
  
  
  
  
  bool next_video_data_is_iframe_;

  
  
  
  
  bool doing_browser_seek_;

  
  
  
  
  bool pending_seek_;
  base::TimeDelta pending_seek_time_;

  
  scoped_ptr<AudioDecoderJob, MediaDecoderJob::Deleter> audio_decoder_job_;
  scoped_ptr<VideoDecoderJob, MediaDecoderJob::Deleter> video_decoder_job_;

  bool reconfig_audio_decoder_;
  bool reconfig_video_decoder_;

  
  
  base::TimeDelta preroll_timestamp_;

  
  
  
  base::CancelableClosure decoder_starvation_callback_;

  
  scoped_ptr<AudioTimestampHelper> audio_timestamp_helper_;

  
  base::WeakPtrFactory<MediaSourcePlayer> weak_this_;

  MediaDrmBridge* drm_bridge_;

  
  
  
  bool is_waiting_for_key_;

  
  base::Closure decode_callback_for_testing_;

  friend class MediaSourcePlayerTest;
  DISALLOW_COPY_AND_ASSIGN(MediaSourcePlayer);
};

}  

#endif  
