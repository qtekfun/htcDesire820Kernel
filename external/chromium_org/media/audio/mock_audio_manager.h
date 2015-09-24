// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_MOCK_AUDIO_MANAGER_H_
#define MEDIA_AUDIO_MOCK_AUDIO_MANAGER_H_

#include "media/audio/audio_manager.h"

namespace media {

class MockAudioManager : public media::AudioManager {
 public:
  explicit MockAudioManager(base::MessageLoopProxy* message_loop_proxy);

  virtual bool HasAudioOutputDevices() OVERRIDE;

  virtual bool HasAudioInputDevices() OVERRIDE;

  virtual base::string16 GetAudioInputDeviceModel() OVERRIDE;

  virtual void ShowAudioInputSettings() OVERRIDE;

  virtual void GetAudioInputDeviceNames(
      media::AudioDeviceNames* device_names) OVERRIDE;

  virtual void GetAudioOutputDeviceNames(
      media::AudioDeviceNames* device_names) OVERRIDE;

  virtual media::AudioOutputStream* MakeAudioOutputStream(
      const media::AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) OVERRIDE;

  virtual media::AudioOutputStream* MakeAudioOutputStreamProxy(
      const media::AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) OVERRIDE;

  virtual media::AudioInputStream* MakeAudioInputStream(
      const media::AudioParameters& params,
      const std::string& device_id) OVERRIDE;

  virtual scoped_refptr<base::MessageLoopProxy> GetMessageLoop() OVERRIDE;
  virtual scoped_refptr<base::MessageLoopProxy> GetWorkerLoop() OVERRIDE;

  virtual void AddOutputDeviceChangeListener(
      AudioDeviceListener* listener) OVERRIDE;
  virtual void RemoveOutputDeviceChangeListener(
      AudioDeviceListener* listener) OVERRIDE;

  virtual AudioParameters GetDefaultOutputStreamParameters() OVERRIDE;
  virtual AudioParameters GetOutputStreamParameters(
      const std::string& device_id) OVERRIDE;
  virtual AudioParameters GetInputStreamParameters(
      const std::string& device_id) OVERRIDE;
  virtual std::string GetAssociatedOutputDeviceID(
      const std::string& input_device_id) OVERRIDE;

  virtual scoped_ptr<AudioLog> CreateAudioLog(
      AudioLogFactory::AudioComponent component) OVERRIDE;

  virtual void FixWedgedAudio() OVERRIDE;

 protected:
  virtual ~MockAudioManager();

 private:
  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(MockAudioManager);
};

}  

#endif  
