/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_IME_INPUT_EVENT_DEV_H_
#define PPAPI_C_DEV_PPB_IME_INPUT_EVENT_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_input_event.h"

#define PPB_IME_INPUT_EVENT_DEV_INTERFACE_0_1 "PPB_IMEInputEvent(Dev);0.1"
#define PPB_IME_INPUT_EVENT_DEV_INTERFACE_0_2 "PPB_IMEInputEvent(Dev);0.2"
#define PPB_IME_INPUT_EVENT_DEV_INTERFACE PPB_IME_INPUT_EVENT_DEV_INTERFACE_0_2



struct PPB_IMEInputEvent_Dev_0_2 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_InputEvent_Type type,
                        PP_TimeTicks time_stamp,
                        struct PP_Var text,
                        uint32_t segment_number,
                        const uint32_t segment_offsets[],
                        int32_t target_segment,
                        uint32_t selection_start,
                        uint32_t selection_end);
  PP_Bool (*IsIMEInputEvent)(PP_Resource resource);
  struct PP_Var (*GetText)(PP_Resource ime_event);
  uint32_t (*GetSegmentNumber)(PP_Resource ime_event);
  uint32_t (*GetSegmentOffset)(PP_Resource ime_event, uint32_t index);
  int32_t (*GetTargetSegment)(PP_Resource ime_event);
  void (*GetSelection)(PP_Resource ime_event, uint32_t* start, uint32_t* end);
};

typedef struct PPB_IMEInputEvent_Dev_0_2 PPB_IMEInputEvent_Dev;

struct PPB_IMEInputEvent_Dev_0_1 {
  PP_Bool (*IsIMEInputEvent)(PP_Resource resource);
  struct PP_Var (*GetText)(PP_Resource ime_event);
  uint32_t (*GetSegmentNumber)(PP_Resource ime_event);
  uint32_t (*GetSegmentOffset)(PP_Resource ime_event, uint32_t index);
  int32_t (*GetTargetSegment)(PP_Resource ime_event);
  void (*GetSelection)(PP_Resource ime_event, uint32_t* start, uint32_t* end);
};

#endif  

