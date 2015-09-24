// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_WIN_AUDIO_MANAGER_WIN_H_
#define MEDIA_AUDIO_WIN_AUDIO_MANAGER_WIN_H_

#include <string>

#include "media/audio/audio_manager_base.h"

namespace media {

class AudioDeviceListenerWin;

class MEDIA_EXPORT AudioManagerWin : public AudioManagerBase {
 public:
  AudioManagerWin(AudioLogFactory* audio_log_factory);

  
  virtual bool HasAudioOutputDevices() OVERRIDE;
  virtual bool HasAudioInputDevices() OVERRIDE;
  virtual base::string16 GetAudioInputDeviceModel() OVERRIDE;
  virtual void ShowAudioInputSettings() OVERRIDE;
  virtual void GetAudioInputDeviceNames(
      AudioDeviceNames* device_names) OVERRIDE;
  virtual void GetAudioOutputDeviceNames(
      AudioDeviceNames* device_names) OVERRIDE;
  virtual AudioParameters GetInputStreamParameters(
      const std::string& device_id) OVERRIDE;
  virtual std::string GetAssociatedOutputDeviceID(
      const std::string& input_device_id) OVERRIDE;

  
  virtual AudioOutputStream* MakeLinearOutputStream(
      const AudioParameters& params) OVERRIDE;
  virtual AudioOutputStream* MakeLowLatencyOutputStream(
      const AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) OVERRIDE;
  virtual AudioInputStream* MakeLinearInputStream(
      const AudioParameters& params, const std::string& device_id) OVERRIDE;
  virtual AudioInputStream* MakeLowLatencyInputStream(
      const AudioParameters& params, const std::string& device_id) OVERRIDE;
  virtual std::string GetDefaultOutputDeviceID() OVERRIDE;

 protected:
  virtual ~AudioManagerWin();

  virtual AudioParameters GetPreferredOutputStreamParameters(
      const std::string& output_device_id,
      const AudioParameters& input_params) OVERRIDE;

 private:
  enum EnumerationType {
    kUninitializedEnumeration = 0,
    kMMDeviceEnumeration,
    kWaveEnumeration,
  };

  
  friend class AudioManagerTest;

  EnumerationType enumeration_type_;
  EnumerationType enumeration_type() { return enumeration_type_; }
  void SetEnumerationType(EnumerationType type) {
    enumeration_type_ = type;
  }

  
  
  
  
  
  AudioInputStream* CreatePCMWaveInAudioInputStream(
      const AudioParameters& params,
      const std::string& device_id);

  
  void CreateDeviceListener();
  void DestroyDeviceListener();

  void GetAudioDeviceNamesImpl(bool input, AudioDeviceNames* device_names);

  
  scoped_ptr<AudioDeviceListenerWin> output_device_listener_;

  DISALLOW_COPY_AND_ASSIGN(AudioManagerWin);
};

}  

#endif  
