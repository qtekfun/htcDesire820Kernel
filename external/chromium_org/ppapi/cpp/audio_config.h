// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_AUDIO_CONFIG_H_
#define PPAPI_CPP_AUDIO_CONFIG_H_

#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"


namespace pp {

class InstanceHandle;

class AudioConfig : public Resource {
 public:
  
  AudioConfig();

  
  
  
  
  
  
  
  
  
  
  
  
  
  AudioConfig(const InstanceHandle& instance,
              PP_AudioSampleRate sample_rate,
              uint32_t sample_frame_count);

  
  
  
  
  
  static PP_AudioSampleRate RecommendSampleRate(
      const InstanceHandle& instance);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static uint32_t RecommendSampleFrameCount(
      const InstanceHandle& instance,
      PP_AudioSampleRate sample_rate,
      uint32_t requested_sample_frame_count);

  
  
  
  
  PP_AudioSampleRate sample_rate() const { return sample_rate_; }

  
  
  
  uint32_t sample_frame_count() const { return sample_frame_count_; }

 private:
  PP_AudioSampleRate sample_rate_;
  uint32_t sample_frame_count_;
};

}  

#endif  

