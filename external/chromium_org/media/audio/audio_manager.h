// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_MANAGER_H_
#define MEDIA_AUDIO_AUDIO_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "media/audio/audio_device_name.h"
#include "media/audio/audio_logging.h"
#include "media/audio/audio_parameters.h"

namespace base {
class MessageLoop;
class MessageLoopProxy;
}

namespace media {

class AudioInputStream;
class AudioOutputStream;

class MEDIA_EXPORT AudioManager {
  public:
   virtual ~AudioManager();

  
  
  
  static AudioManager* Create(AudioLogFactory* audio_log_factory);

  
  static AudioManager* CreateForTesting();

  
  
  
  static AudioManager* Get();

  
  
  virtual bool HasAudioOutputDevices() = 0;

  
  
  
  virtual bool HasAudioInputDevices() = 0;

  
  
  virtual base::string16 GetAudioInputDeviceModel() = 0;

  
  
  
  
  virtual void ShowAudioInputSettings() = 0;

  
  
  
  
  
  
  
  virtual void GetAudioInputDeviceNames(AudioDeviceNames* device_names) = 0;

  
  
  
  
  
  virtual void GetAudioOutputDeviceNames(AudioDeviceNames* device_names) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual AudioOutputStream* MakeAudioOutputStream(
      const AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) = 0;

  
  
  
  
  virtual AudioOutputStream* MakeAudioOutputStreamProxy(
      const AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual AudioInputStream* MakeAudioInputStream(
      const AudioParameters& params, const std::string& device_id) = 0;

  
  virtual scoped_refptr<base::MessageLoopProxy> GetMessageLoop() = 0;

  
  
  
  virtual scoped_refptr<base::MessageLoopProxy> GetWorkerLoop() = 0;

  
  
  
  class AudioDeviceListener {
   public:
    virtual void OnDeviceChange() = 0;
  };

  virtual void AddOutputDeviceChangeListener(AudioDeviceListener* listener) = 0;
  virtual void RemoveOutputDeviceChangeListener(
      AudioDeviceListener* listener) = 0;

  
  
  
  
  
  virtual AudioParameters GetDefaultOutputStreamParameters() = 0;

  
  virtual AudioParameters GetOutputStreamParameters(
      const std::string& device_id) = 0;

  
  
  
  virtual AudioParameters GetInputStreamParameters(
      const std::string& device_id) = 0;

  
  
  
  
  
  
  virtual std::string GetAssociatedOutputDeviceID(
      const std::string& input_device_id) = 0;

  
  
  virtual scoped_ptr<AudioLog> CreateAudioLog(
      AudioLogFactory::AudioComponent component) = 0;

  
  
  
  virtual void FixWedgedAudio() = 0;

 protected:
  AudioManager();

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioManager);
};

}  

#endif  
