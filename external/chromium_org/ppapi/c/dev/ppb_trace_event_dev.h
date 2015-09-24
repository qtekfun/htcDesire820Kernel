/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_TRACE_EVENT_DEV_H_
#define PPAPI_C_DEV_PPB_TRACE_EVENT_DEV_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_TRACE_EVENT_DEV_INTERFACE_0_1 "PPB_Trace_Event(Dev);0.1"
#define PPB_TRACE_EVENT_DEV_INTERFACE_0_2 "PPB_Trace_Event(Dev);0.2"
#define PPB_TRACE_EVENT_DEV_INTERFACE PPB_TRACE_EVENT_DEV_INTERFACE_0_2



typedef int64_t PP_TraceEventTime;

struct PPB_Trace_Event_Dev_0_2 {
  void* (*GetCategoryEnabled)(const char* category_name);
  void (*AddTraceEvent)(int8_t phase,
                        const void* category_enabled,
                        const char* name,
                        uint64_t id,
                        uint32_t num_args,
                        const char* arg_names[],
                        const uint8_t arg_types[],
                        const uint64_t arg_values[],
                        uint8_t flags);
  void (*AddTraceEventWithThreadIdAndTimestamp)(int8_t phase,
                                                const void* category_enabled,
                                                const char* name,
                                                uint64_t id,
                                                int32_t thread_id,
                                                PP_TraceEventTime timestamp,
                                                uint32_t num_args,
                                                const char* arg_names[],
                                                const uint8_t arg_types[],
                                                const uint64_t arg_values[],
                                                uint8_t flags);
  PP_TraceEventTime (*Now)(void);
  void (*SetThreadName)(const char* thread_name);
};

typedef struct PPB_Trace_Event_Dev_0_2 PPB_Trace_Event_Dev;

struct PPB_Trace_Event_Dev_0_1 {
  void* (*GetCategoryEnabled)(const char* category_name);
  void (*AddTraceEvent)(int8_t phase,
                        const void* category_enabled,
                        const char* name,
                        uint64_t id,
                        uint32_t num_args,
                        const char* arg_names[],
                        const uint8_t arg_types[],
                        const uint64_t arg_values[],
                        uint8_t flags);
  void (*SetThreadName)(const char* thread_name);
};

#endif  

