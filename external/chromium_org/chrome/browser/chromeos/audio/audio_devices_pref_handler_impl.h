// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_AUDIO_AUDIO_DEVICES_PREF_HANDLER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_AUDIO_AUDIO_DEVICES_PREF_HANDLER_IMPL_H_

#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/values.h"
#include "chromeos/audio/audio_devices_pref_handler.h"

class PrefRegistrySimple;
class PrefService;

namespace chromeos {

class AudioDevicesPrefHandlerImpl : public AudioDevicesPrefHandler {
 public:
  explicit AudioDevicesPrefHandlerImpl(PrefService* local_state);

  
  virtual double GetOutputVolumeValue(const AudioDevice* device) OVERRIDE;
  virtual double GetInputGainValue(const AudioDevice* device) OVERRIDE;
  virtual void SetVolumeGainValue(const AudioDevice& device,
                                  double value) OVERRIDE;

  virtual bool GetMuteValue(const AudioDevice& device) OVERRIDE;
  virtual void SetMuteValue(const AudioDevice& device, bool mute_on) OVERRIDE;

  virtual bool GetAudioCaptureAllowedValue() OVERRIDE;
  virtual bool GetAudioOutputAllowedValue() OVERRIDE;

  virtual void AddAudioPrefObserver(AudioPrefObserver* observer) OVERRIDE;
  virtual void RemoveAudioPrefObserver(AudioPrefObserver* observer) OVERRIDE;

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

 protected:
  virtual ~AudioDevicesPrefHandlerImpl();

 private:
  
  void InitializePrefObservers();

  
  void UpdateDevicesMutePref();
  void SaveDevicesMutePref();

  
  void UpdateDevicesVolumePref();
  void SaveDevicesVolumePref();

  double GetVolumeGainPrefValue(const AudioDevice& device);
  double GetDeviceDefaultOutputVolume(const AudioDevice& device);

  
  
  
  
  void MigrateDeviceMuteSettings(std::string active_device);
  void MigrateDeviceVolumeSettings(std::string active_device);

  
  void NotifyAudioPolicyChange();

  scoped_ptr<base::DictionaryValue> device_mute_settings_;
  scoped_ptr<base::DictionaryValue> device_volume_settings_;

  PrefService* local_state_;  
  PrefChangeRegistrar pref_change_registrar_;
  ObserverList<AudioPrefObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(AudioDevicesPrefHandlerImpl);
};

}  

#endif  
