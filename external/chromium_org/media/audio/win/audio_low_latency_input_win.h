// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef MEDIA_AUDIO_WIN_AUDIO_LOW_LATENCY_INPUT_WIN_H_
#define MEDIA_AUDIO_WIN_AUDIO_LOW_LATENCY_INPUT_WIN_H_

#include <Audioclient.h>
#include <MMDeviceAPI.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/platform_thread.h"
#include "base/threading/simple_thread.h"
#include "base/win/scoped_co_mem.h"
#include "base/win/scoped_com_initializer.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_handle.h"
#include "media/audio/agc_audio_stream.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

namespace media {

class AudioManagerWin;

class MEDIA_EXPORT WASAPIAudioInputStream
    : public AgcAudioStream<AudioInputStream>,
      public base::DelegateSimpleThread::Delegate,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  WASAPIAudioInputStream(AudioManagerWin* manager,
                         const AudioParameters& params,
                         const std::string& device_id);
  
  
  virtual ~WASAPIAudioInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;

  
  
  static int HardwareSampleRate(const std::string& device_id);

  
  
  static uint32 HardwareChannelCount(const std::string& device_id);

  bool started() const { return started_; }

 private:
  
  virtual void Run() OVERRIDE;

  
  void HandleError(HRESULT err);

  
  HRESULT SetCaptureDevice();
  HRESULT ActivateCaptureDevice();
  HRESULT GetAudioEngineStreamFormat();
  bool DesiredFormatIsSupported();
  HRESULT InitializeAudioEngine();

  
  
  static HRESULT GetMixFormat(const std::string& device_id,
                              WAVEFORMATEX** device_format);

  
  AudioManagerWin* manager_;

  
  
  base::DelegateSimpleThread* capture_thread_;

  
  WAVEFORMATEX format_;

  bool opened_;
  bool started_;

  
  size_t frame_size_;

  
  
  
  size_t packet_size_frames_;

  
  size_t packet_size_bytes_;

  
  uint32 endpoint_buffer_size_frames_;

  
  
  
  std::string device_id_;

  
  
  double perf_count_to_100ns_units_;

  
  
  double ms_to_frame_count_;

  
  AudioInputCallback* sink_;

  

  
  base::win::ScopedComPtr<IMMDevice> endpoint_device_;

  

  
  
  base::win::ScopedComPtr<IAudioClient> audio_client_;

  
  
  
  
  
  
  base::win::ScopedComPtr<IAudioClient> audio_render_client_for_loopback_;

  
  
  base::win::ScopedComPtr<IAudioCaptureClient> audio_capture_client_;

  
  
  
  
  base::win::ScopedComPtr<ISimpleAudioVolume> simple_audio_volume_;

  
  
  base::win::ScopedHandle audio_samples_ready_event_;

  
  base::win::ScopedHandle stop_capture_event_;

  DISALLOW_COPY_AND_ASSIGN(WASAPIAudioInputStream);
};

}  

#endif  
