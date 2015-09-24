// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_AUDIO_H_
#define PPAPI_CPP_AUDIO_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_audio.h"
#include "ppapi/cpp/audio_config.h"
#include "ppapi/cpp/resource.h"


namespace pp {

class InstanceHandle;

class Audio : public Resource {
 public:

  
  Audio() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Audio(const InstanceHandle& instance,
        const AudioConfig& config,
        PPB_Audio_Callback callback,
        void* user_data);

  
  
  
  
  
  
  
  
  
  Audio(const InstanceHandle& instance,
        const AudioConfig& config,
        PPB_Audio_Callback_1_0 callback,
        void* user_data);

  
  
  
  
  AudioConfig& config() { return config_; }

  
  
  
  
  
  const AudioConfig& config() const { return config_; }

  
  
  
  bool StartPlayback();

  
  
  
  bool StopPlayback();

 private:
  AudioConfig config_;
  bool use_1_0_interface_;
};

}  

#endif  

