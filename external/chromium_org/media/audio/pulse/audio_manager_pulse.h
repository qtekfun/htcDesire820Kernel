// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_PULSE_AUDIO_MANAGER_PULSE_H_
#define MEDIA_AUDIO_PULSE_AUDIO_MANAGER_PULSE_H_

#include <pulse/pulseaudio.h>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "media/audio/audio_manager_base.h"

namespace media {

class MEDIA_EXPORT AudioManagerPulse : public AudioManagerBase {
 public:
  AudioManagerPulse(AudioLogFactory* audio_log_factory);
  virtual ~AudioManagerPulse();

  static AudioManager* Create(AudioLogFactory* audio_log_factory);

  
  virtual bool HasAudioOutputDevices() OVERRIDE;
  virtual bool HasAudioInputDevices() OVERRIDE;
  virtual void ShowAudioInputSettings() OVERRIDE;
  virtual void GetAudioInputDeviceNames(
      AudioDeviceNames* device_names) OVERRIDE;
  virtual void GetAudioOutputDeviceNames(
      AudioDeviceNames* device_names) OVERRIDE;
  virtual AudioParameters GetInputStreamParameters(
      const std::string& device_id) OVERRIDE;

  
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

 protected:
  virtual AudioParameters GetPreferredOutputStreamParameters(
      const std::string& output_device_id,
      const AudioParameters& input_params) OVERRIDE;

 private:
  bool Init();
  void DestroyPulse();

  void GetAudioDeviceNames(bool input, media::AudioDeviceNames* device_names);

  
  static void InputDevicesInfoCallback(pa_context* context,
                                       const pa_source_info* info,
                                       int error, void* user_data);
  static void OutputDevicesInfoCallback(pa_context* context,
                                        const pa_sink_info* info,
                                        int error, void* user_data);

  
  
  static void SampleRateInfoCallback(pa_context* context,
                                     const pa_server_info* info,
                                     void* user_data);

  
  AudioOutputStream* MakeOutputStream(const AudioParameters& params,
                                      const std::string& input_device_id);

  
  AudioInputStream* MakeInputStream(const AudioParameters& params,
                                    const std::string& device_id);

  
  int GetNativeSampleRate();

  pa_threaded_mainloop* input_mainloop_;
  pa_context* input_context_;
  AudioDeviceNames* devices_;
  int native_input_sample_rate_;

  DISALLOW_COPY_AND_ASSIGN(AudioManagerPulse);
};

}  

#endif  
