// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_AUDIO_AUDIO_DEVICES_PREF_HANDLER_H_
#define CHROMEOS_AUDIO_AUDIO_DEVICES_PREF_HANDLER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chromeos/audio/audio_pref_observer.h"
#include "chromeos/chromeos_export.h"

class PrefService;

namespace chromeos {

struct AudioDevice;

class CHROMEOS_EXPORT AudioDevicesPrefHandler
    : public base::RefCountedThreadSafe<AudioDevicesPrefHandler> {
 public:
  
  
  
  virtual double GetOutputVolumeValue(const AudioDevice* device) = 0;
  virtual double GetInputGainValue(const AudioDevice* device) = 0;
  
  virtual void SetVolumeGainValue(const AudioDevice& device, double value) = 0;

  
  virtual bool GetMuteValue(const AudioDevice& device) = 0;
  
  virtual void SetMuteValue(const AudioDevice& device, bool mute_on) = 0;

  
  virtual bool GetAudioCaptureAllowedValue() = 0;
  
  virtual bool GetAudioOutputAllowedValue() = 0;

  
  virtual void AddAudioPrefObserver(AudioPrefObserver* observer) = 0;
  
  virtual void RemoveAudioPrefObserver(AudioPrefObserver* observer) = 0;

  
  static AudioDevicesPrefHandler* Create(PrefService* local_state);

 protected:
  virtual ~AudioDevicesPrefHandler() {}

 private:
  friend class base::RefCountedThreadSafe<AudioDevicesPrefHandler>;
};

}  

#endif  
