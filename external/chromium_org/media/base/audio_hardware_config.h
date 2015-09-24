// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_HARDWARE_CONFIG_H_
#define MEDIA_BASE_AUDIO_HARDWARE_CONFIG_H_

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "media/audio/audio_parameters.h"
#include "media/base/channel_layout.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioHardwareConfig {
 public:
  AudioHardwareConfig(const media::AudioParameters& input_params,
                      const media::AudioParameters& output_params);
  virtual ~AudioHardwareConfig();

  
  
  int GetOutputBufferSize() const;
  int GetOutputSampleRate() const;
  ChannelLayout GetOutputChannelLayout() const;
  int GetOutputChannels() const;

  int GetInputSampleRate() const;
  ChannelLayout GetInputChannelLayout() const;
  int GetInputChannels() const;

  media::AudioParameters GetInputConfig() const;
  media::AudioParameters GetOutputConfig() const;

  
  
  
  
  void UpdateInputConfig(const media::AudioParameters& input_params);
  void UpdateOutputConfig(const media::AudioParameters& output_params);

 private:
  
  mutable base::Lock config_lock_;
  media::AudioParameters input_params_;
  media::AudioParameters output_params_;

  DISALLOW_COPY_AND_ASSIGN(AudioHardwareConfig);
};

}  

#endif  
