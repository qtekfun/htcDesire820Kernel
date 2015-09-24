/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_AUDIO_H_
#define PPAPI_C_PPB_AUDIO_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

#define PPB_AUDIO_INTERFACE_1_0 "PPB_Audio;1.0"
#define PPB_AUDIO_INTERFACE_1_1 "PPB_Audio;1.1"
#define PPB_AUDIO_INTERFACE PPB_AUDIO_INTERFACE_1_1



typedef void (*PPB_Audio_Callback)(void* sample_buffer,
                                   uint32_t buffer_size_in_bytes,
                                   PP_TimeDelta latency,
                                   void* user_data);

typedef void (*PPB_Audio_Callback_1_0)(void* sample_buffer,
                                       uint32_t buffer_size_in_bytes,
                                       void* user_data);

struct PPB_Audio_1_1 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_Resource config,
                        PPB_Audio_Callback audio_callback,
                        void* user_data);
  PP_Bool (*IsAudio)(PP_Resource resource);
  PP_Resource (*GetCurrentConfig)(PP_Resource audio);
  PP_Bool (*StartPlayback)(PP_Resource audio);
  PP_Bool (*StopPlayback)(PP_Resource audio);
};

typedef struct PPB_Audio_1_1 PPB_Audio;

struct PPB_Audio_1_0 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_Resource config,
                        PPB_Audio_Callback_1_0 audio_callback,
                        void* user_data);
  PP_Bool (*IsAudio)(PP_Resource resource);
  PP_Resource (*GetCurrentConfig)(PP_Resource audio);
  PP_Bool (*StartPlayback)(PP_Resource audio);
  PP_Bool (*StopPlayback)(PP_Resource audio);
};

#endif  

