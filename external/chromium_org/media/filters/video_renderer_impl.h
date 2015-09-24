// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_VIDEO_RENDERER_IMPL_H_
#define MEDIA_FILTERS_VIDEO_RENDERER_IMPL_H_

#include <deque>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "media/base/decryptor.h"
#include "media/base/demuxer_stream.h"
#include "media/base/pipeline_status.h"
#include "media/base/video_decoder.h"
#include "media/base/video_frame.h"
#include "media/base/video_renderer.h"
#include "media/filters/video_frame_stream.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class MEDIA_EXPORT VideoRendererImpl
    : public VideoRenderer,
      public base::PlatformThread::Delegate {
 public:
  typedef base::Callback<void(const scoped_refptr<VideoFrame>&)> PaintCB;
  typedef base::Callback<void(bool)> SetOpaqueCB;

  
  static base::TimeDelta kMaxLastFrameDuration();

  
  
  
  
  
  
  
  
  
  
  
  
  
  VideoRendererImpl(const scoped_refptr<base::MessageLoopProxy>& message_loop,
                    ScopedVector<VideoDecoder> decoders,
                    const SetDecryptorReadyCB& set_decryptor_ready_cb,
                    const PaintCB& paint_cb,
                    const SetOpaqueCB& set_opaque_cb,
                    bool drop_frames);
  virtual ~VideoRendererImpl();

  
  virtual void Initialize(DemuxerStream* stream,
                          const PipelineStatusCB& init_cb,
                          const StatisticsCB& statistics_cb,
                          const TimeCB& max_time_cb,
                          const NaturalSizeChangedCB& size_changed_cb,
                          const base::Closure& ended_cb,
                          const PipelineStatusCB& error_cb,
                          const TimeDeltaCB& get_time_cb,
                          const TimeDeltaCB& get_duration_cb) OVERRIDE;
  virtual void Play(const base::Closure& callback) OVERRIDE;
  virtual void Pause(const base::Closure& callback) OVERRIDE;
  virtual void Flush(const base::Closure& callback) OVERRIDE;
  virtual void Preroll(base::TimeDelta time,
                       const PipelineStatusCB& cb) OVERRIDE;
  virtual void Stop(const base::Closure& callback) OVERRIDE;
  virtual void SetPlaybackRate(float playback_rate) OVERRIDE;

  
  virtual void ThreadMain() OVERRIDE;

 private:
  
  void OnVideoFrameStreamInitialized(bool success, bool has_alpha);

  
  
  void FrameReady(VideoFrameStream::Status status,
                  const scoped_refptr<VideoFrame>& frame);

  
  void AddReadyFrame_Locked(const scoped_refptr<VideoFrame>& frame);

  
  
  
  void AttemptRead();
  void AttemptRead_Locked();

  
  void OnVideoFrameStreamResetDone();

  
  
  
  
  base::TimeDelta CalculateSleepDuration(
      const scoped_refptr<VideoFrame>& next_frame,
      float playback_rate);

  
  void DoStopOrError_Locked();

  
  
  
  
  
  void PaintNextReadyFrame_Locked();

  
  
  
  void DropNextReadyFrame_Locked();

  void TransitionToPrerolled_Locked();

  
  
  bool ShouldTransitionToPrerolled_Locked();

  
  
  
  void UpdateStatsAndWait_Locked(base::TimeDelta wait_duration);

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<VideoRendererImpl> weak_factory_;
  base::WeakPtr<VideoRendererImpl> weak_this_;

  
  base::Lock lock_;

  
  VideoFrameStream video_frame_stream_;

  
  typedef std::deque<scoped_refptr<VideoFrame> > VideoFrameQueue;
  VideoFrameQueue ready_frames_;

  
  bool received_end_of_stream_;

  
  
  base::ConditionVariable frame_available_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  enum State {
    kUninitialized,
    kInitializing,
    kPrerolled,
    kPaused,
    kFlushing,
    kFlushed,
    kPrerolling,
    kPlaying,
    kEnded,
    kStopped,
    kError,
  };
  State state_;

  
  base::PlatformThreadHandle thread_;

  
  
  bool pending_read_;

  bool drop_frames_;

  float playback_rate_;

  
  base::Closure flush_cb_;
  PipelineStatusCB preroll_cb_;

  
  PipelineStatusCB init_cb_;
  StatisticsCB statistics_cb_;
  TimeCB max_time_cb_;
  NaturalSizeChangedCB size_changed_cb_;
  base::Closure ended_cb_;
  PipelineStatusCB error_cb_;
  TimeDeltaCB get_time_cb_;
  TimeDeltaCB get_duration_cb_;

  base::TimeDelta preroll_timestamp_;

  
  PaintCB paint_cb_;

  
  SetOpaqueCB set_opaque_cb_;

  
  
  
  
  
  gfx::Size last_natural_size_;

  
  
  
  base::TimeDelta last_timestamp_;

  
  
  int frames_decoded_;
  int frames_dropped_;

  DISALLOW_COPY_AND_ASSIGN(VideoRendererImpl);
};

}  

#endif  
