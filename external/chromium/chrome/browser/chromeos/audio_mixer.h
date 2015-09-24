// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_AUDIO_MIXER_H_
#define CHROME_BROWSER_CHROMEOS_AUDIO_MIXER_H_
#pragma once

#include "base/basictypes.h"
#include "base/callback.h"

namespace chromeos {

class AudioMixer {
 public:
  
  static const double kSilenceDb = -200.0;

  enum State {
    UNINITIALIZED = 0,
    INITIALIZING,
    READY,
    SHUTTING_DOWN,
    IN_ERROR,
  };

  AudioMixer() {}
  virtual ~AudioMixer() {}

  
  
  typedef Callback1<bool>::Type InitDoneCallback;
  virtual void Init(InitDoneCallback* callback) = 0;

  
  virtual bool InitSync() = 0;

  
  virtual double GetVolumeDb() const = 0;

  
  
  virtual bool GetVolumeLimits(double* vol_min, double* vol_max) = 0;

  
  virtual void SetVolumeDb(double vol_db) = 0;

  
  
  virtual bool IsMute() const = 0;

  
  virtual void SetMute(bool mute) = 0;

  
  
  
  virtual State GetState() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioMixer);
};

}  

#endif  

