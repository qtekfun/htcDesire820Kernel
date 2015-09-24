// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_AUDIO_WIN_AUDIO_LOW_LATENCY_OUTPUT_WIN_H_
#define MEDIA_AUDIO_WIN_AUDIO_LOW_LATENCY_OUTPUT_WIN_H_

#include <Audioclient.h>
#include <MMDeviceAPI.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/platform_thread.h"
#include "base/threading/simple_thread.h"
#include "base/win/scoped_co_mem.h"
#include "base/win/scoped_com_initializer.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_handle.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

namespace media {

class AudioManagerWin;

class MEDIA_EXPORT WASAPIAudioOutputStream :
      public AudioOutputStream,
      public base::DelegateSimpleThread::Delegate {
 public:
  
  
  WASAPIAudioOutputStream(AudioManagerWin* manager,
                          const std::string& device_id,
                          const AudioParameters& params,
                          ERole device_role);

  
  
  virtual ~WASAPIAudioOutputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;

  
  
  
  static int HardwareSampleRate(const std::string& device_id);

  
  
  static AUDCLNT_SHAREMODE GetShareMode();

  bool started() const { return render_thread_.get() != NULL; }

 private:
  
  virtual void Run() OVERRIDE;

  
  
  
  
  bool RenderAudioFromSource(UINT64 device_frequency);

  
  
  
  
  HRESULT ExclusiveModeInitialization(IAudioClient* client,
                                      HANDLE event_handle,
                                      uint32* endpoint_buffer_size);

  
  
  
  void StopThread();

  
  base::PlatformThreadId creating_thread_id_;

  
  AudioManagerWin* manager_;

  
  
  scoped_ptr<base::DelegateSimpleThread> render_thread_;

  
  
  
  WAVEFORMATPCMEX format_;

  
  bool opened_;

  
  
  
  bool audio_parameters_are_valid_;

  
  float volume_;

  
  
  
  size_t packet_size_frames_;

  
  size_t packet_size_bytes_;

  
  uint32 endpoint_buffer_size_frames_;

  
  const std::string device_id_;

  
  ERole device_role_;

  
  
  
  AUDCLNT_SHAREMODE share_mode_;

  // Counts the number of audio frames written to the endpoint buffer.
  UINT64 num_written_frames_;

  
  AudioSourceCallback* source_;

  
  
  base::win::ScopedComPtr<IAudioClient> audio_client_;

  
  
  base::win::ScopedComPtr<IAudioRenderClient> audio_render_client_;

  
  
  base::win::ScopedHandle audio_samples_render_event_;

  
  base::win::ScopedHandle stop_render_event_;

  
  scoped_ptr<AudioBus> audio_bus_;

  base::win::ScopedComPtr<IAudioClock> audio_clock_;

  DISALLOW_COPY_AND_ASSIGN(WASAPIAudioOutputStream);
};

}  

#endif  
