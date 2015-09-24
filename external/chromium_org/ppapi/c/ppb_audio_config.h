/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_AUDIO_CONFIG_H_
#define PPAPI_C_PPB_AUDIO_CONFIG_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_AUDIO_CONFIG_INTERFACE_1_0 "PPB_AudioConfig;1.0"
#define PPB_AUDIO_CONFIG_INTERFACE_1_1 "PPB_AudioConfig;1.1"
#define PPB_AUDIO_CONFIG_INTERFACE PPB_AUDIO_CONFIG_INTERFACE_1_1



enum {
  PP_AUDIOMINSAMPLEFRAMECOUNT = 64,
  PP_AUDIOMAXSAMPLEFRAMECOUNT = 32768
};

typedef enum {
  PP_AUDIOSAMPLERATE_NONE = 0,
  PP_AUDIOSAMPLERATE_44100 = 44100,
  PP_AUDIOSAMPLERATE_48000 = 48000
} PP_AudioSampleRate;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_AudioSampleRate, 4);

struct PPB_AudioConfig_1_1 {
  PP_Resource (*CreateStereo16Bit)(PP_Instance instance,
                                   PP_AudioSampleRate sample_rate,
                                   uint32_t sample_frame_count);
  uint32_t (*RecommendSampleFrameCount)(
      PP_Instance instance,
      PP_AudioSampleRate sample_rate,
      uint32_t requested_sample_frame_count);
  PP_Bool (*IsAudioConfig)(PP_Resource resource);
  PP_AudioSampleRate (*GetSampleRate)(PP_Resource config);
  uint32_t (*GetSampleFrameCount)(PP_Resource config);
  PP_AudioSampleRate (*RecommendSampleRate)(PP_Instance instance);
};

typedef struct PPB_AudioConfig_1_1 PPB_AudioConfig;

struct PPB_AudioConfig_1_0 {
  PP_Resource (*CreateStereo16Bit)(PP_Instance instance,
                                   PP_AudioSampleRate sample_rate,
                                   uint32_t sample_frame_count);
  uint32_t (*RecommendSampleFrameCount)(
      PP_AudioSampleRate sample_rate,
      uint32_t requested_sample_frame_count);
  PP_Bool (*IsAudioConfig)(PP_Resource resource);
  PP_AudioSampleRate (*GetSampleRate)(PP_Resource config);
  uint32_t (*GetSampleFrameCount)(PP_Resource config);
};

#endif  

