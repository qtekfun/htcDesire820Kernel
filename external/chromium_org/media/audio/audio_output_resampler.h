// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_OUTPUT_RESAMPLER_H_
#define MEDIA_AUDIO_AUDIO_OUTPUT_RESAMPLER_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_manager.h"
#include "media/audio/audio_output_dispatcher.h"
#include "media/audio/audio_parameters.h"

namespace media {

class OnMoreDataConverter;

class MEDIA_EXPORT AudioOutputResampler : public AudioOutputDispatcher {
 public:
  AudioOutputResampler(AudioManager* audio_manager,
                       const AudioParameters& input_params,
                       const AudioParameters& output_params,
                       const std::string& output_device_id,
                       const std::string& input_device_id,
                       const base::TimeDelta& close_delay);

  
  virtual bool OpenStream() OVERRIDE;
  virtual bool StartStream(AudioOutputStream::AudioSourceCallback* callback,
                           AudioOutputProxy* stream_proxy) OVERRIDE;
  virtual void StopStream(AudioOutputProxy* stream_proxy) OVERRIDE;
  virtual void StreamVolumeSet(AudioOutputProxy* stream_proxy,
                               double volume) OVERRIDE;
  virtual void CloseStream(AudioOutputProxy* stream_proxy) OVERRIDE;
  virtual void Shutdown() OVERRIDE;
  virtual void CloseStreamsForWedgeFix() OVERRIDE;
  virtual void RestartStreamsForWedgeFix() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<AudioOutputResampler>;
  virtual ~AudioOutputResampler();

  
  
  void SetupFallbackParams();

  
  void Initialize();

  
  scoped_refptr<AudioOutputDispatcher> dispatcher_;

  
  
  typedef std::map<AudioOutputProxy*, OnMoreDataConverter*> CallbackMap;
  CallbackMap callbacks_;

  
  base::TimeDelta close_delay_;

  
  AudioParameters output_params_;

  
  
  bool streams_opened_;

  DISALLOW_COPY_AND_ASSIGN(AudioOutputResampler);
};

}  

#endif  
