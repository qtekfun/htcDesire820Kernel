// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_PIPELINE_H_
#define MEDIA_BASE_PIPELINE_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "base/time/default_tick_clock.h"
#include "media/base/audio_renderer.h"
#include "media/base/demuxer.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"
#include "media/base/ranges.h"
#include "media/base/serial_runner.h"
#include "ui/gfx/size.h"

namespace base {
class MessageLoopProxy;
class TimeDelta;
}

namespace media {

class Clock;
class FilterCollection;
class MediaLog;
class TextRenderer;
class TextTrackConfig;
class VideoRenderer;

class MEDIA_EXPORT Pipeline : public DemuxerHost {
 public:
  
  
  
  
  
  
  
  
  enum BufferingState {
    kHaveMetadata,
    kPrerollCompleted,
  };

  typedef base::Callback<void(BufferingState)> BufferingStateCB;

  
  Pipeline(const scoped_refptr<base::MessageLoopProxy>& message_loop,
           MediaLog* media_log);
  virtual ~Pipeline();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Start(scoped_ptr<FilterCollection> filter_collection,
             const base::Closure& ended_cb,
             const PipelineStatusCB& error_cb,
             const PipelineStatusCB& seek_cb,
             const BufferingStateCB& buffering_state_cb,
             const base::Closure& duration_change_cb);

  
  
  
  
  
  
  
  void Stop(const base::Closure& stop_cb);

  
  
  
  
  
  
  
  void Seek(base::TimeDelta time, const PipelineStatusCB& seek_cb);

  
  
  
  bool IsRunning() const;

  
  bool HasAudio() const;

  
  bool HasVideo() const;

  
  
  
  
  float GetPlaybackRate() const;

  
  
  
  
  
  void SetPlaybackRate(float playback_rate);

  
  
  
  float GetVolume() const;

  
  
  
  void SetVolume(float volume);

  
  
  base::TimeDelta GetMediaTime() const;

  
  Ranges<base::TimeDelta> GetBufferedTimeRanges();

  
  
  base::TimeDelta GetMediaDuration() const;

  
  
  int64 GetTotalBytes() const;

  
  
  
  void GetNaturalVideoSize(gfx::Size* out_size) const;

  
  
  bool DidLoadingProgress() const;

  
  PipelineStatistics GetStatistics() const;

  void SetClockForTesting(Clock* clock);
  void SetErrorForTesting(PipelineStatus status);

 private:
  FRIEND_TEST_ALL_PREFIXES(PipelineTest, GetBufferedTimeRanges);
  FRIEND_TEST_ALL_PREFIXES(PipelineTest, DisableAudioRenderer);
  FRIEND_TEST_ALL_PREFIXES(PipelineTest, DisableAudioRendererDuringInit);
  FRIEND_TEST_ALL_PREFIXES(PipelineTest, EndedCallback);
  FRIEND_TEST_ALL_PREFIXES(PipelineTest, AudioStreamShorterThanVideo);
  friend class MediaLog;

  
  enum State {
    kCreated,
    kInitDemuxer,
    kInitAudioRenderer,
    kInitVideoRenderer,
    kInitPrerolling,
    kSeeking,
    kStarting,
    kStarted,
    kStopping,
    kStopped,
  };

  
  void SetState(State next_state);

  static const char* GetStateString(State state);
  State GetNextState() const;

  
  
  void FinishSeek();

  
  virtual void SetTotalBytes(int64 total_bytes) OVERRIDE;
  virtual void AddBufferedByteRange(int64 start, int64 end) OVERRIDE;
  virtual void AddBufferedTimeRange(base::TimeDelta start,
                                    base::TimeDelta end) OVERRIDE;

  
  virtual void SetDuration(base::TimeDelta duration) OVERRIDE;
  virtual void OnDemuxerError(PipelineStatus error) OVERRIDE;
  virtual void AddTextStream(DemuxerStream* text_stream,
                             const TextTrackConfig& config) OVERRIDE;
  virtual void RemoveTextStream(DemuxerStream* text_stream) OVERRIDE;

  
  
  
  void SetError(PipelineStatus error);

  
  void OnNaturalVideoSizeChanged(const gfx::Size& size);

  
  void OnAudioRendererEnded();
  void OnVideoRendererEnded();
  void OnTextRendererEnded();

  
  void OnUpdateStatistics(const PipelineStatistics& stats);

  
  void OnAudioDisabled();

  
  void OnAudioTimeUpdate(base::TimeDelta time, base::TimeDelta max_time);

  
  void OnVideoTimeUpdate(base::TimeDelta max_time);

  
  
  
  void StartTask(scoped_ptr<FilterCollection> filter_collection,
                 const base::Closure& ended_cb,
                 const PipelineStatusCB& error_cb,
                 const PipelineStatusCB& seek_cb,
                 const BufferingStateCB& buffering_state_cb,
                 const base::Closure& duration_change_cb);

  
  void StopTask(const base::Closure& stop_cb);

  
  
  void ErrorChangedTask(PipelineStatus error);

  
  void PlaybackRateChangedTask(float playback_rate);

  
  void VolumeChangedTask(float volume);

  
  void SeekTask(base::TimeDelta time, const PipelineStatusCB& seek_cb);

  
  void DoAudioRendererEnded();
  void DoVideoRendererEnded();
  void DoTextRendererEnded();
  void RunEndedCallbackIfNeeded();

  
  void AudioDisabledTask();

  
  void AddTextStreamTask(DemuxerStream* text_stream,
                         const TextTrackConfig& config);

  
  void RemoveTextStreamTask(DemuxerStream* text_stream);

  
  
  void InitializeDemuxer(const PipelineStatusCB& done_cb);
  void InitializeAudioRenderer(const PipelineStatusCB& done_cb);
  void InitializeVideoRenderer(const PipelineStatusCB& done_cb);

  
  
  
  
  
  
  
  void TearDownPipeline();

  
  base::TimeDelta TimeForByteOffset_Locked(int64 byte_offset) const;

  void OnStateTransition(PipelineStatus status);
  void StateTransitionTask(PipelineStatus status);

  
  
  void DoInitialPreroll(const PipelineStatusCB& done_cb);

  
  
  
  
  
  
  void DoSeek(base::TimeDelta seek_timestamp, const PipelineStatusCB& done_cb);

  
  
  void DoPlay(const PipelineStatusCB& done_cb);

  
  
  void DoStop(const PipelineStatusCB& done_cb);
  void OnStopCompleted(PipelineStatus status);

  void OnAudioUnderflow();

  void StartClockIfWaitingForTimeUpdate_Locked();

  
  scoped_refptr<base::MessageLoopProxy> message_loop_;

  
  scoped_refptr<MediaLog> media_log_;

  
  mutable base::Lock lock_;

  
  bool running_;

  
  Ranges<int64> buffered_byte_ranges_;
  Ranges<base::TimeDelta> buffered_time_ranges_;

  
  
  mutable bool did_loading_progress_;

  
  int64 total_bytes_;

  
  gfx::Size natural_size_;

  
  
  
  float volume_;

  
  
  
  float playback_rate_;

  
  base::DefaultTickClock default_tick_clock_;

  
  
  
  scoped_ptr<Clock> clock_;

  
  
  
  bool waiting_for_clock_update_;

  
  
  
  
  PipelineStatus status_;

  
  
  
  
  bool has_audio_;
  bool has_video_;

  
  

  
  State state_;

  
  bool audio_ended_;
  bool video_ended_;
  bool text_ended_;

  
  bool audio_disabled_;

  
  PipelineStatusCB seek_cb_;

  
  base::Closure stop_cb_;

  
  base::Closure ended_cb_;
  PipelineStatusCB error_cb_;
  BufferingStateCB buffering_state_cb_;
  base::Closure duration_change_cb_;

  
  scoped_ptr<FilterCollection> filter_collection_;

  
  Demuxer* demuxer_;

  
  
  scoped_ptr<AudioRenderer> audio_renderer_;
  scoped_ptr<VideoRenderer> video_renderer_;
  scoped_ptr<TextRenderer> text_renderer_;

  PipelineStatistics statistics_;

  
  
  base::TimeTicks creation_time_;

  scoped_ptr<SerialRunner> pending_callbacks_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(Pipeline);
};

}  

#endif  
