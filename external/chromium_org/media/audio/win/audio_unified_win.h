// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_WIN_AUDIO_UNIFIED_WIN_H_
#define MEDIA_AUDIO_WIN_AUDIO_UNIFIED_WIN_H_

#include <Audioclient.h>
#include <MMDeviceAPI.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/threading/platform_thread.h"
#include "base/threading/simple_thread.h"
#include "base/win/scoped_co_mem.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_handle.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_fifo.h"
#include "media/base/channel_mixer.h"
#include "media/base/media_export.h"
#include "media/base/multi_channel_resampler.h"

namespace media {

class AudioManagerWin;

class MEDIA_EXPORT WASAPIUnifiedStream
    : public AudioOutputStream,
      public base::DelegateSimpleThread::Delegate {
 public:
  
  
  WASAPIUnifiedStream(AudioManagerWin* manager,
                      const AudioParameters& params,
                      const std::string& input_device_id);

  
  
  virtual ~WASAPIUnifiedStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

  bool started() const {
    return audio_io_thread_.get() != NULL;
  }

  
  
  bool VarispeedMode() const { return (fifo_ && resampler_); }

 private:
  enum {
    
    
    
    kTimeDiffInMillisecondsBetweenDelayMeasurements = 1000,

    
    kFifoSize = 16384,

    
    
    
    
    
    kTargetFifoSafetyFactor = 2
  };

  
  
  
  
  void DoVarispeedInitialization(const AudioParameters& input_params,
                                 const AudioParameters& output_params);

  
  void ResetVarispeed();

  
  
  void SetIOFormats(const AudioParameters& input_params,
                    const AudioParameters& output_params);

  
  virtual void Run() OVERRIDE;

  
  
  
  virtual void ProvideInput(int frame_delay, AudioBus* audio_bus);

  
  void HandleError(HRESULT err);

  
  void StopAndJoinThread(HRESULT err);

  
  std::string GetDeviceName(LPCWSTR device_id) const;

  
  
  
  void ProcessInputAudio();

  
  
  
  
  
  
  
  
  void ProcessOutputAudio(IAudioClock* audio_output_clock);

  
  base::PlatformThreadId creating_thread_id_;

  
  AudioManagerWin* manager_;

  
  AudioParameters params_;
  
  int input_channels_;
  int output_channels_;

  
  const std::string input_device_id_;

  
  
  
  AUDCLNT_SHAREMODE share_mode_;

  
  
  scoped_ptr<base::DelegateSimpleThread> audio_io_thread_;

  
  
  
  WAVEFORMATPCMEX output_format_;

  
  
  WAVEFORMATPCMEX input_format_;

  
  bool opened_;

  
  double volume_;

  
  
  
  size_t output_buffer_size_frames_;

  
  
  
  size_t input_buffer_size_frames_;

  
  uint32 endpoint_render_buffer_size_frames_;
  uint32 endpoint_capture_buffer_size_frames_;

  // Counts the number of audio frames written to the endpoint buffer.
  uint64 num_written_frames_;

  
  base::TimeTicks last_delay_sample_time_;

  
  double total_delay_ms_;

  
  
  
  int total_delay_bytes_;

  
  AudioSourceCallback* source_;

  
  base::win::ScopedComPtr<IMMDevice> endpoint_render_device_;
  base::win::ScopedComPtr<IMMDevice> endpoint_capture_device_;

  
  
  base::win::ScopedComPtr<IAudioClient> audio_output_client_;
  base::win::ScopedComPtr<IAudioClient> audio_input_client_;

  
  
  base::win::ScopedComPtr<IAudioRenderClient> audio_render_client_;

  
  
  base::win::ScopedComPtr<IAudioCaptureClient> audio_capture_client_;

  
  
  base::win::ScopedHandle capture_event_;

  
  
  
  base::win::ScopedHandle render_event_;

  
  base::win::ScopedHandle stop_streaming_event_;

  
  scoped_ptr<AudioBus> output_bus_;

  
  scoped_ptr<AudioBus> input_bus_;

  
  scoped_ptr<AudioBus> channel_bus_;

  

  
  scoped_ptr<AudioBus> resampled_bus_;

  
  
  bool input_callback_received_;

  
  
  scoped_ptr<MultiChannelResampler> resampler_;

  
  double io_sample_rate_ratio_;

  
  scoped_ptr<AudioFifo> fifo_;

  
  
  
  scoped_ptr<ChannelMixer> channel_mixer_;

  
  int target_fifo_frames_;

  
  
  double average_delta_;

  
  double fifo_rate_compensation_;

  
  
  bool update_output_delay_;

  
  
  double capture_delay_ms_;

  
  
#ifndef NDEBUG
  enum LogElementNames {
    INPUT_TIME_STAMP,
    NUM_FRAMES_IN_FIFO,
    RESAMPLER_MARGIN,
    RATE_COMPENSATION
  };

  scoped_ptr<int64[]> input_time_stamps_;
  scoped_ptr<int[]> num_frames_in_fifo_;
  scoped_ptr<int[]> resampler_margin_;
  scoped_ptr<double[]> fifo_rate_comps_;
  scoped_ptr<int[]> num_elements_;
  scoped_ptr<int[]> input_params_;
  scoped_ptr<int[]> output_params_;

  FILE* data_file_;
  FILE* param_file_;
#endif

  DISALLOW_COPY_AND_ASSIGN(WASAPIUnifiedStream);
};

}  

#endif  
