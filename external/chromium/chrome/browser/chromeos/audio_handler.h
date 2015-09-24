// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_AUDIO_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_AUDIO_HANDLER_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"

class InProcessBrowserTest;
template <typename T> struct DefaultSingletonTraits;

namespace chromeos {

class AudioMixer;

class AudioHandler {
 public:
  static AudioHandler* GetInstance();

  
  
  
  double GetVolumePercent();

  
  
  
  void SetVolumePercent(double volume_percent);

  
  
  
  void AdjustVolumeByPercent(double adjust_by_percent);

  
  
  bool IsMute();

  
  void SetMute(bool do_mute);

  
  void Disconnect();

 private:
  enum MixerType {
    MIXER_TYPE_ALSA = 0,
    MIXER_TYPE_NONE,
  };

  
  
  friend struct DefaultSingletonTraits<AudioHandler>;

  friend class ::InProcessBrowserTest;
  
  
  static void Disable();

  
  bool TryToConnect(bool async);

  void OnMixerInitialized(bool success);

  AudioHandler();
  virtual ~AudioHandler();
  bool VerifyMixerConnection();

  
  double VolumeDbToPercent(double volume_db) const;
  double PercentToVolumeDb(double volume_percent) const;

  scoped_ptr<AudioMixer> mixer_;

  bool connected_;
  int reconnect_tries_;

  
  
  double max_volume_db_;
  double min_volume_db_;

  
  MixerType mixer_type_;

  DISALLOW_COPY_AND_ASSIGN(AudioHandler);
};

}  

DISABLE_RUNNABLE_METHOD_REFCOUNT(chromeos::AudioHandler);

#endif  
