// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_VIRTUAL_AUDIO_OUTPUT_STREAM_H_
#define MEDIA_AUDIO_VIRTUAL_AUDIO_OUTPUT_STREAM_H_

#include "base/callback.h"
#include "base/threading/thread_checker.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_converter.h"

namespace media {

class VirtualAudioInputStream;

class MEDIA_EXPORT VirtualAudioOutputStream
    : public AudioOutputStream,
      public AudioConverter::InputCallback {
 public:
  
  typedef base::Callback<void(VirtualAudioOutputStream* vaos)>
      AfterCloseCallback;

  
  
  VirtualAudioOutputStream(const AudioParameters& params,
                           VirtualAudioInputStream* target,
                           const AfterCloseCallback& after_close_cb);

  virtual ~VirtualAudioOutputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;
  virtual void Close() OVERRIDE;

 private:
  
  virtual double ProvideInput(AudioBus* audio_bus,
                              base::TimeDelta buffer_delay) OVERRIDE;

  const AudioParameters params_;
  
  
  
  VirtualAudioInputStream* const target_input_stream_;

  AfterCloseCallback after_close_cb_;

  AudioSourceCallback* callback_;
  double volume_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(VirtualAudioOutputStream);
};

}  

#endif  
