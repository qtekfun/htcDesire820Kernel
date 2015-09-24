/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_AUDIO_INPUT_DEV_H_
#define PPAPI_C_DEV_PPB_AUDIO_INPUT_DEV_H_

#include "ppapi/c/dev/ppb_device_ref_dev.h"
#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

#define PPB_AUDIO_INPUT_DEV_INTERFACE_0_2 "PPB_AudioInput(Dev);0.2"
#define PPB_AUDIO_INPUT_DEV_INTERFACE_0_3 "PPB_AudioInput(Dev);0.3"
#define PPB_AUDIO_INPUT_DEV_INTERFACE_0_4 "PPB_AudioInput(Dev);0.4"
#define PPB_AUDIO_INPUT_DEV_INTERFACE PPB_AUDIO_INPUT_DEV_INTERFACE_0_4



typedef void (*PPB_AudioInput_Callback)(const void* sample_buffer,
                                        uint32_t buffer_size_in_bytes,
                                        PP_TimeDelta latency,
                                        void* user_data);

typedef void (*PPB_AudioInput_Callback_0_2)(const void* sample_buffer,
                                            uint32_t buffer_size_in_bytes,
                                            void* user_data);

struct PPB_AudioInput_Dev_0_4 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsAudioInput)(PP_Resource resource);
  int32_t (*EnumerateDevices)(PP_Resource audio_input,
                              struct PP_ArrayOutput output,
                              struct PP_CompletionCallback callback);
  int32_t (*MonitorDeviceChange)(PP_Resource audio_input,
                                 PP_MonitorDeviceChangeCallback callback,
                                 void* user_data);
  int32_t (*Open)(PP_Resource audio_input,
                  PP_Resource device_ref,
                  PP_Resource config,
                  PPB_AudioInput_Callback audio_input_callback,
                  void* user_data,
                  struct PP_CompletionCallback callback);
  PP_Resource (*GetCurrentConfig)(PP_Resource audio_input);
  PP_Bool (*StartCapture)(PP_Resource audio_input);
  PP_Bool (*StopCapture)(PP_Resource audio_input);
  void (*Close)(PP_Resource audio_input);
};

typedef struct PPB_AudioInput_Dev_0_4 PPB_AudioInput_Dev;

struct PPB_AudioInput_Dev_0_2 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsAudioInput)(PP_Resource resource);
  int32_t (*EnumerateDevices)(PP_Resource audio_input,
                              PP_Resource* devices,
                              struct PP_CompletionCallback callback);
  int32_t (*Open)(PP_Resource audio_input,
                  PP_Resource device_ref,
                  PP_Resource config,
                  PPB_AudioInput_Callback_0_2 audio_input_callback,
                  void* user_data,
                  struct PP_CompletionCallback callback);
  PP_Resource (*GetCurrentConfig)(PP_Resource audio_input);
  PP_Bool (*StartCapture)(PP_Resource audio_input);
  PP_Bool (*StopCapture)(PP_Resource audio_input);
  void (*Close)(PP_Resource audio_input);
};

struct PPB_AudioInput_Dev_0_3 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsAudioInput)(PP_Resource resource);
  int32_t (*EnumerateDevices)(PP_Resource audio_input,
                              struct PP_ArrayOutput output,
                              struct PP_CompletionCallback callback);
  int32_t (*MonitorDeviceChange)(PP_Resource audio_input,
                                 PP_MonitorDeviceChangeCallback callback,
                                 void* user_data);
  int32_t (*Open)(PP_Resource audio_input,
                  PP_Resource device_ref,
                  PP_Resource config,
                  PPB_AudioInput_Callback_0_2 audio_input_callback,
                  void* user_data,
                  struct PP_CompletionCallback callback);
  PP_Resource (*GetCurrentConfig)(PP_Resource audio_input);
  PP_Bool (*StartCapture)(PP_Resource audio_input);
  PP_Bool (*StopCapture)(PP_Resource audio_input);
  void (*Close)(PP_Resource audio_input);
};

#endif  

