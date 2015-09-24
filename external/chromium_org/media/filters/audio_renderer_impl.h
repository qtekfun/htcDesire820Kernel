// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_FILTERS_AUDIO_RENDERER_IMPL_H_
#define MEDIA_FILTERS_AUDIO_RENDERER_IMPL_H_

#include <deque>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "media/base/audio_decoder.h"
#include "media/base/audio_renderer.h"
#include "media/base/audio_renderer_sink.h"
#include "media/base/decryptor.h"
#include "media/filters/audio_renderer_algorithm.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class AudioBus;
class AudioDecoderSelector;
class AudioSplicer;
class DecryptingDemuxerStream;

class MEDIA_EXPORT AudioRendererImpl
    : public AudioRenderer,
      NON_EXPORTED_BASE(public AudioRendererSink::RenderCallback) {
 public:
  
  
  
  
  
  
  
  
  AudioRendererImpl(const scoped_refptr<base::MessageLoopProxy>& message_loop,
                    AudioRendererSink* sink,
                    ScopedVector<AudioDecoder> decoders,
                    const SetDecryptorReadyCB& set_decryptor_ready_cb);
  virtual ~AudioRendererImpl();

  
  virtual void Initialize(DemuxerStream* stream,
                          const PipelineStatusCB& init_cb,
                          const StatisticsCB& statistics_cb,
                          const base::Closure& underflow_cb,
                          const TimeCB& time_cb,
                          const base::Closure& ended_cb,
                          const base::Closure& disabled_cb,
                          const PipelineStatusCB& error_cb) OVERRIDE;
  virtual void Play(const base::Closure& callback) OVERRIDE;
  virtual void Pause(const base::Closure& callback) OVERRIDE;
  virtual void Flush(const base::Closure& callback) OVERRIDE;
  virtual void Stop(const base::Closure& callback) OVERRIDE;
  virtual void SetPlaybackRate(float rate) OVERRIDE;
  virtual void Preroll(base::TimeDelta time,
                       const PipelineStatusCB& cb) OVERRIDE;
  virtual void ResumeAfterUnderflow() OVERRIDE;
  virtual void SetVolume(float volume) OVERRIDE;

  
  
  
  
  void DisableUnderflowForTesting();

  
  typedef base::Callback<base::TimeTicks()> NowCB;
  void set_now_cb_for_testing(const NowCB& now_cb) {
    now_cb_ = now_cb;
  }

 private:
  friend class AudioRendererImplTest;

  
  enum State {
    kUninitialized,
    kPaused,
    kFlushing,
    kPrerolling,
    kPlaying,
    kStopped,
    kUnderflow,
    kRebuffering,
  };

  
  void DecodedAudioReady(AudioDecoder::Status status,
                         const scoped_refptr<AudioBuffer>& buffer);

  
  
  bool HandleSplicerBuffer(const scoped_refptr<AudioBuffer>& buffer);

  
  
  void HandleAbortedReadOrDecodeError(bool is_decode_error);

  
  void UpdateEarliestEndTime_Locked(int frames_filled,
                                    const base::TimeDelta& playback_delay,
                                    const base::TimeTicks& time_now);

  void DoPlay_Locked();
  void DoPause_Locked();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Render(AudioBus* audio_bus,
                     int audio_delay_milliseconds) OVERRIDE;
  virtual void OnRenderError() OVERRIDE;

  
  
  
  
  void AttemptRead();
  void AttemptRead_Locked();
  bool CanRead_Locked();
  void ChangeState_Locked(State new_state);

  
  
  
  bool IsBeforePrerollTime(const scoped_refptr<AudioBuffer>& buffer);

  
  
  
  
  
  void OnDecoderSelected(
      scoped_ptr<AudioDecoder> decoder,
      scoped_ptr<DecryptingDemuxerStream> decrypting_demuxer_stream);

  
  
  void DoFlush_Locked();

  
  
  void ResetDecoder();

  
  void ResetDecoderDone();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<AudioRendererImpl> weak_factory_;
  base::WeakPtr<AudioRendererImpl> weak_this_;

  scoped_ptr<AudioSplicer> splicer_;

  
  
  
  scoped_refptr<media::AudioRendererSink> sink_;

  scoped_ptr<AudioDecoderSelector> decoder_selector_;

  
  scoped_ptr<AudioDecoder> decoder_;
  scoped_ptr<DecryptingDemuxerStream> decrypting_demuxer_stream_;

  
  AudioParameters audio_parameters_;

  
  PipelineStatusCB init_cb_;
  StatisticsCB statistics_cb_;
  base::Closure underflow_cb_;
  TimeCB time_cb_;
  base::Closure ended_cb_;
  base::Closure disabled_cb_;
  PipelineStatusCB error_cb_;

  
  base::Closure flush_cb_;

  
  PipelineStatusCB preroll_cb_;

  
  NowCB now_cb_;

  
  
  base::Lock lock_;

  
  scoped_ptr<AudioRendererAlgorithm> algorithm_;

  
  State state_;

  
  bool sink_playing_;

  
  bool pending_read_;

  
  bool received_end_of_stream_;
  bool rendered_end_of_stream_;

  
  
  base::TimeDelta audio_time_buffered_;
  base::TimeDelta current_time_;

  base::TimeDelta preroll_timestamp_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  base::TimeTicks earliest_end_time_;
  size_t total_frames_filled_;

  bool underflow_disabled_;

  
  
  bool preroll_aborted_;

  

  DISALLOW_COPY_AND_ASSIGN(AudioRendererImpl);
};

}  

#endif  
