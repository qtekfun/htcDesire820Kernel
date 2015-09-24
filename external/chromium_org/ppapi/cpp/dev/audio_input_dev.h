// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_AUDIO_INPUT_DEV_H_
#define PPAPI_CPP_DEV_AUDIO_INPUT_DEV_H_

#include <vector>

#include "ppapi/c/dev/ppb_audio_input_dev.h"
#include "ppapi/cpp/audio_config.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/dev/device_ref_dev.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class InstanceHandle;

class AudioInput_Dev : public Resource {
 public:
  
  AudioInput_Dev();

  
  explicit AudioInput_Dev(const InstanceHandle& instance);

  virtual ~AudioInput_Dev();

  
  
  
  
  static bool IsAvailable();

  int32_t EnumerateDevices(
      const CompletionCallbackWithOutput<std::vector<DeviceRef_Dev> >&
          callback);

  int32_t MonitorDeviceChange(PP_MonitorDeviceChangeCallback callback,
                              void* user_data);

  
  
  
  
  int32_t Open(const DeviceRef_Dev& device_ref,
               const AudioConfig& config,
               PPB_AudioInput_Callback audio_input_callback,
               void* user_data,
               const CompletionCallback& callback);

  
  int32_t Open(const DeviceRef_Dev& device_ref,
               const AudioConfig& config,
               PPB_AudioInput_Callback_0_2 audio_input_callback_0_2,
               void* user_data,
               const CompletionCallback& callback);

  bool StartCapture();
  bool StopCapture();
  void Close();
};

}  

#endif  
