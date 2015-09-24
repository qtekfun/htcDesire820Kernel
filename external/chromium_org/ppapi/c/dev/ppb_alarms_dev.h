/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_ALARMS_DEV_H_
#define PPAPI_C_DEV_PPB_ALARMS_DEV_H_

#include "ppapi/c/dev/pp_optional_structs_dev.h"
#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_ALARMS_DEV_INTERFACE_0_1 "PPB_Alarms(Dev);0.1"
#define PPB_ALARMS_DEV_INTERFACE PPB_ALARMS_DEV_INTERFACE_0_1



struct PP_Alarms_Alarm_Dev {
  struct PP_Var name;
  double scheduled_time;
  struct PP_Optional_Double_Dev period_in_minutes;
};

struct PP_Alarms_AlarmCreateInfo_Dev {
  struct PP_Optional_Double_Dev when;
  struct PP_Optional_Double_Dev delay_in_minutes;
  struct PP_Optional_Double_Dev period_in_minutes;
};

struct PP_Alarms_Alarm_Array_Dev {
  uint32_t size;
  struct PP_Alarms_Alarm_Dev *elements;
};

typedef void (*PP_Alarms_OnAlarm_Dev)(
    uint32_t listener_id,
    void* user_data,
    const struct PP_Alarms_Alarm_Dev* alarm);

struct PPB_Alarms_Dev_0_1 {
  void (*Create)(PP_Instance instance,
                 struct PP_Var name,
                 const struct PP_Alarms_AlarmCreateInfo_Dev* alarm_info);
  int32_t (*Get)(PP_Instance instance,
                 struct PP_Var name,
                 struct PP_Alarms_Alarm_Dev* alarm,
                 struct PP_CompletionCallback callback);
  int32_t (*GetAll)(PP_Instance instance,
                    struct PP_Alarms_Alarm_Array_Dev* alarms,
                    struct PP_ArrayOutput array_allocator,
                    struct PP_CompletionCallback callback);
  void (*Clear)(PP_Instance instance, struct PP_Var name);
  void (*ClearAll)(PP_Instance instance);
  uint32_t (*AddOnAlarmListener)(PP_Instance instance,
                                 PP_Alarms_OnAlarm_Dev callback,
                                 void* user_data);
};

typedef struct PPB_Alarms_Dev_0_1 PPB_Alarms_Dev;

#endif  

