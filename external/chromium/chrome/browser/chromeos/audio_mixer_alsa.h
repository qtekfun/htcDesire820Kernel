// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_AUDIO_MIXER_ALSA_H_
#define CHROME_BROWSER_CHROMEOS_AUDIO_MIXER_ALSA_H_
#pragma once

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "chrome/browser/chromeos/audio_mixer.h"

class PrefService;

struct _snd_mixer_elem;
struct _snd_mixer;

namespace chromeos {

class AudioMixerAlsa : public AudioMixer {
 public:
  AudioMixerAlsa();
  virtual ~AudioMixerAlsa();

  
  virtual void Init(InitDoneCallback* callback);
  virtual bool InitSync();
  virtual double GetVolumeDb() const;
  virtual bool GetVolumeLimits(double* vol_min, double* vol_max);
  virtual void SetVolumeDb(double vol_db);
  virtual bool IsMute() const;
  virtual void SetMute(bool mute);
  virtual State GetState() const;

  
  static void RegisterPrefs(PrefService* local_state);

 private:
  
  void DoInit(InitDoneCallback* callback);

  
  bool InitThread();
  void InitPrefs();

  
  
  bool InitializeAlsaMixer();
  void FreeAlsaMixer();
  void DoSetVolumeMute(double pref_volume, int pref_mute);

  
  void RestoreVolumeMuteOnUIThread();

  
  double DoGetVolumeDb_Locked() const;
  void DoSetVolumeDb_Locked(double vol_db);

  _snd_mixer_elem* FindElementWithName_Locked(_snd_mixer* handle,
                                              const char* element_name) const;

  bool GetElementVolume_Locked(_snd_mixer_elem* elem,
                               double* current_vol) const;

  
  
  
  
  
  bool SetElementVolume_Locked(_snd_mixer_elem* elem,
                               double new_vol,
                               double* actual_vol,
                               double rounding_bias);

  
  
  bool GetElementMuted_Locked(_snd_mixer_elem* elem) const;
  void SetElementMuted_Locked(_snd_mixer_elem* elem, bool mute);

  
  double min_volume_;
  double max_volume_;

  
  
  
  
  double save_volume_;

  mutable base::Lock mixer_state_lock_;
  mutable State mixer_state_;

  
  _snd_mixer* alsa_mixer_;
  _snd_mixer_elem* elem_master_;
  _snd_mixer_elem* elem_pcm_;

  PrefService* prefs_;
  base::WaitableEvent done_event_;

  scoped_ptr<base::Thread> thread_;

  DISALLOW_COPY_AND_ASSIGN(AudioMixerAlsa);
};

}  

DISABLE_RUNNABLE_METHOD_REFCOUNT(chromeos::AudioMixerAlsa);

#endif  
