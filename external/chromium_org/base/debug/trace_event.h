// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_DEBUG_TRACE_EVENT_H_
#define BASE_DEBUG_TRACE_EVENT_H_

#include <string>

#include "base/atomicops.h"
#include "base/debug/trace_event_impl.h"
#include "base/debug/trace_event_memory.h"
#include "base/debug/trace_event_system_stats_monitor.h"
#include "build/build_config.h"

#define TRACE_STR_COPY(str) \
    trace_event_internal::TraceStringWithCopy(str)

#define TRACE_DISABLED_BY_DEFAULT(name) "disabled-by-default-" name

#define TRACE_ID_MANGLE(id) \
    trace_event_internal::TraceID::ForceMangle(id)

#define TRACE_ID_DONT_MANGLE(id) \
    trace_event_internal::TraceID::DontMangle(id)

#define TRACE_EVENT0(category_group, name) \
    INTERNAL_TRACE_MEMORY(category_group, name) \
    INTERNAL_TRACE_EVENT_ADD_SCOPED(category_group, name)
#define TRACE_EVENT1(category_group, name, arg1_name, arg1_val) \
    INTERNAL_TRACE_MEMORY(category_group, name) \
    INTERNAL_TRACE_EVENT_ADD_SCOPED(category_group, name, arg1_name, arg1_val)
#define TRACE_EVENT2( \
    category_group, name, arg1_name, arg1_val, arg2_name, arg2_val) \
  INTERNAL_TRACE_MEMORY(category_group, name) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED( \
      category_group, name, arg1_name, arg1_val, arg2_name, arg2_val)

#define TRACE_EVENT_WITH_MEMORY_TAG2( \
    category, name, memory_tag, arg1_name, arg1_val, arg2_name, arg2_val) \
  INTERNAL_TRACE_MEMORY(category, memory_tag) \
  INTERNAL_TRACE_EVENT_ADD_SCOPED( \
      category, name, arg1_name, arg1_val, arg2_name, arg2_val)


#if OFFICIAL_BUILD
#undef TRACING_IS_OFFICIAL_BUILD
#define TRACING_IS_OFFICIAL_BUILD 1
#elif !defined(TRACING_IS_OFFICIAL_BUILD)
#define TRACING_IS_OFFICIAL_BUILD 0
#endif

#if TRACING_IS_OFFICIAL_BUILD
#define UNSHIPPED_TRACE_EVENT0(category_group, name) (void)0
#define UNSHIPPED_TRACE_EVENT1(category_group, name, arg1_name, arg1_val) \
    (void)0
#define UNSHIPPED_TRACE_EVENT2(category_group, name, arg1_name, arg1_val, \
                               arg2_name, arg2_val) (void)0
#define UNSHIPPED_TRACE_EVENT_INSTANT0(category_group, name, scope) (void)0
#define UNSHIPPED_TRACE_EVENT_INSTANT1(category_group, name, scope, \
                                       arg1_name, arg1_val) (void)0
#define UNSHIPPED_TRACE_EVENT_INSTANT2(category_group, name, scope, \
                                       arg1_name, arg1_val, \
                                       arg2_name, arg2_val) (void)0
#else
#define UNSHIPPED_TRACE_EVENT0(category_group, name) \
    TRACE_EVENT0(category_group, name)
#define UNSHIPPED_TRACE_EVENT1(category_group, name, arg1_name, arg1_val) \
    TRACE_EVENT1(category_group, name, arg1_name, arg1_val)
#define UNSHIPPED_TRACE_EVENT2(category_group, name, arg1_name, arg1_val, \
                               arg2_name, arg2_val) \
    TRACE_EVENT2(category_group, name, arg1_name, arg1_val, arg2_name, arg2_val)
#define UNSHIPPED_TRACE_EVENT_INSTANT0(category_group, name, scope) \
    TRACE_EVENT_INSTANT0(category_group, name, scope)
#define UNSHIPPED_TRACE_EVENT_INSTANT1(category_group, name, scope, \
                                       arg1_name, arg1_val) \
    TRACE_EVENT_INSTANT1(category_group, name, scope, arg1_name, arg1_val)
#define UNSHIPPED_TRACE_EVENT_INSTANT2(category_group, name, scope, \
                                       arg1_name, arg1_val, \
                                       arg2_name, arg2_val) \
    TRACE_EVENT_INSTANT2(category_group, name, scope, arg1_name, arg1_val, \
                         arg2_name, arg2_val)
#endif

#define TRACE_EVENT_INSTANT0(category_group, name, scope) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, \
        category_group, name, TRACE_EVENT_FLAG_NONE | scope)
#define TRACE_EVENT_INSTANT1(category_group, name, scope, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, \
        category_group, name, TRACE_EVENT_FLAG_NONE | scope, \
        arg1_name, arg1_val)
#define TRACE_EVENT_INSTANT2(category_group, name, scope, arg1_name, arg1_val, \
                             arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, \
        category_group, name, TRACE_EVENT_FLAG_NONE | scope, \
        arg1_name, arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_INSTANT0(category_group, name, scope) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, \
        category_group, name, TRACE_EVENT_FLAG_COPY | scope)
#define TRACE_EVENT_COPY_INSTANT1(category_group, name, scope, \
                                  arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, \
        category_group, name, TRACE_EVENT_FLAG_COPY | scope, arg1_name, \
        arg1_val)
#define TRACE_EVENT_COPY_INSTANT2(category_group, name, scope, \
                                  arg1_name, arg1_val, \
                                  arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_INSTANT, \
        category_group, name, TRACE_EVENT_FLAG_COPY | scope, \
        arg1_name, arg1_val, arg2_name, arg2_val)

#define TRACE_EVENT_SET_SAMPLING_STATE_FOR_BUCKET( \
    bucket_number, category, name)                 \
        trace_event_internal::                     \
        TraceEventSamplingStateScope<bucket_number>::Set(category "\0" name)

#define TRACE_EVENT_GET_SAMPLING_STATE_FOR_BUCKET(bucket_number) \
    trace_event_internal::TraceEventSamplingStateScope<bucket_number>::Current()

#define TRACE_EVENT_SCOPED_SAMPLING_STATE_FOR_BUCKET(                   \
    bucket_number, category, name)                                      \
    trace_event_internal::TraceEventSamplingStateScope<bucket_number>   \
        traceEventSamplingScope(category "\0" name);

#define TRACE_EVENT_SCOPED_SAMPLING_STATE(category, name) \
    TRACE_EVENT_SCOPED_SAMPLING_STATE_FOR_BUCKET(0, category, name)
#define TRACE_EVENT_GET_SAMPLING_STATE() \
    TRACE_EVENT_GET_SAMPLING_STATE_FOR_BUCKET(0)
#define TRACE_EVENT_SET_SAMPLING_STATE(category, name) \
    TRACE_EVENT_SET_SAMPLING_STATE_FOR_BUCKET(0, category, name)


#define TRACE_EVENT_BEGIN0(category_group, name) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, \
        category_group, name, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_BEGIN1(category_group, name, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, \
        category_group, name, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_BEGIN2(category_group, name, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, \
        category_group, name, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val, \
        arg2_name, arg2_val)
#define TRACE_EVENT_COPY_BEGIN0(category_group, name) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, \
        category_group, name, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_BEGIN1(category_group, name, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, \
        category_group, name, TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val)
#define TRACE_EVENT_COPY_BEGIN2(category_group, name, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_BEGIN, \
        category_group, name, TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val, \
        arg2_name, arg2_val)

#define TRACE_EVENT_BEGIN_WITH_ID_TID_AND_TIMESTAMP0(category_group, \
        name, id, thread_id, timestamp) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID_TID_AND_TIMESTAMP( \
        TRACE_EVENT_PHASE_ASYNC_BEGIN, category_group, name, id, thread_id, \
        timestamp, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_COPY_BEGIN_WITH_ID_TID_AND_TIMESTAMP0( \
        category_group, name, id, thread_id, timestamp) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID_TID_AND_TIMESTAMP( \
        TRACE_EVENT_PHASE_ASYNC_BEGIN, category_group, name, id, thread_id, \
        timestamp, TRACE_EVENT_FLAG_COPY)

#define TRACE_EVENT_END0(category_group, name) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, \
        category_group, name, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_END1(category_group, name, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, \
        category_group, name, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_END2(category_group, name, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, \
        category_group, name, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val, \
        arg2_name, arg2_val)
#define TRACE_EVENT_COPY_END0(category_group, name) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, \
        category_group, name, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_END1(category_group, name, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, \
        category_group, name, TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val)
#define TRACE_EVENT_COPY_END2(category_group, name, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_END, \
        category_group, name, TRACE_EVENT_FLAG_COPY, arg1_name, arg1_val, \
        arg2_name, arg2_val)

#define TRACE_EVENT_END_WITH_ID_TID_AND_TIMESTAMP0(category_group, \
        name, id, thread_id, timestamp) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID_TID_AND_TIMESTAMP( \
        TRACE_EVENT_PHASE_ASYNC_END, category_group, name, id, thread_id, \
        timestamp, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_COPY_END_WITH_ID_TID_AND_TIMESTAMP0( \
        category_group, name, id, thread_id, timestamp) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID_TID_AND_TIMESTAMP( \
        TRACE_EVENT_PHASE_ASYNC_END, category_group, name, id, thread_id, \
        timestamp, TRACE_EVENT_FLAG_COPY)

#define TRACE_COUNTER1(category_group, name, value) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, TRACE_EVENT_FLAG_NONE, \
        "value", static_cast<int>(value))
#define TRACE_COPY_COUNTER1(category_group, name, value) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, TRACE_EVENT_FLAG_COPY, \
        "value", static_cast<int>(value))

#define TRACE_COUNTER2(category_group, name, value1_name, value1_val, \
        value2_name, value2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, TRACE_EVENT_FLAG_NONE, \
        value1_name, static_cast<int>(value1_val), \
        value2_name, static_cast<int>(value2_val))
#define TRACE_COPY_COUNTER2(category_group, name, value1_name, value1_val, \
        value2_name, value2_val) \
    INTERNAL_TRACE_EVENT_ADD(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, TRACE_EVENT_FLAG_COPY, \
        value1_name, static_cast<int>(value1_val), \
        value2_name, static_cast<int>(value2_val))

#define TRACE_COUNTER_ID1(category_group, name, id, value) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, \
        "value", static_cast<int>(value))
#define TRACE_COPY_COUNTER_ID1(category_group, name, id, value) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        "value", static_cast<int>(value))

#define TRACE_COUNTER_ID2(category_group, name, id, value1_name, value1_val, \
        value2_name, value2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, \
        value1_name, static_cast<int>(value1_val), \
        value2_name, static_cast<int>(value2_val))
#define TRACE_COPY_COUNTER_ID2(category_group, name, id, value1_name, \
        value1_val, value2_name, value2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_COUNTER, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        value1_name, static_cast<int>(value1_val), \
        value2_name, static_cast<int>(value2_val))


#define TRACE_EVENT_ASYNC_BEGIN0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_ASYNC_BEGIN1(category_group, name, id, arg1_name, \
        arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_ASYNC_BEGIN2(category_group, name, id, arg1_name, \
        arg1_val, arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, \
        arg1_name, arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_ASYNC_BEGIN0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_ASYNC_BEGIN1(category_group, name, id, arg1_name, \
        arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val)
#define TRACE_EVENT_COPY_ASYNC_BEGIN2(category_group, name, id, arg1_name, \
        arg1_val, arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val, arg2_name, arg2_val)

#define TRACE_EVENT_ASYNC_STEP_INTO0(category_group, name, id, step) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP_INTO, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, "step", step)
#define TRACE_EVENT_ASYNC_STEP_INTO1(category_group, name, id, step, \
                                     arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP_INTO, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, "step", step, \
        arg1_name, arg1_val)

#define TRACE_EVENT_ASYNC_STEP_PAST0(category_group, name, id, step) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP_PAST, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, "step", step)
#define TRACE_EVENT_ASYNC_STEP_PAST1(category_group, name, id, step, \
                                     arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_STEP_PAST, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, "step", step, \
        arg1_name, arg1_val)

#define TRACE_EVENT_ASYNC_END0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_ASYNC_END1(category_group, name, id, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_ASYNC_END2(category_group, name, id, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, \
        arg1_name, arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_ASYNC_END0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_ASYNC_END1(category_group, name, id, arg1_name, \
        arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val)
#define TRACE_EVENT_COPY_ASYNC_END2(category_group, name, id, arg1_name, \
        arg1_val, arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_ASYNC_END, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val, arg2_name, arg2_val)


#define TRACE_EVENT_FLOW_BEGIN0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_FLOW_BEGIN1(category_group, name, id, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_FLOW_BEGIN2(category_group, name, id, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, \
        arg1_name, arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_FLOW_BEGIN0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_FLOW_BEGIN1(category_group, name, id, arg1_name, \
        arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val)
#define TRACE_EVENT_COPY_FLOW_BEGIN2(category_group, name, id, arg1_name, \
        arg1_val, arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_BEGIN, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val, arg2_name, arg2_val)

#define TRACE_EVENT_FLOW_STEP0(category_group, name, id, step) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_STEP, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, "step", step)
#define TRACE_EVENT_FLOW_STEP1(category_group, name, id, step, \
        arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_STEP, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, "step", step, \
        arg1_name, arg1_val)
#define TRACE_EVENT_COPY_FLOW_STEP0(category_group, name, id, step) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_STEP, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, "step", step)
#define TRACE_EVENT_COPY_FLOW_STEP1(category_group, name, id, step, \
        arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_STEP, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, "step", step, \
        arg1_name, arg1_val)

#define TRACE_EVENT_FLOW_END0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_END, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE)
#define TRACE_EVENT_FLOW_END1(category_group, name, id, arg1_name, arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_END, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, arg1_name, arg1_val)
#define TRACE_EVENT_FLOW_END2(category_group, name, id, arg1_name, arg1_val, \
        arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_END, \
        category_group, name, id, TRACE_EVENT_FLAG_NONE, \
        arg1_name, arg1_val, arg2_name, arg2_val)
#define TRACE_EVENT_COPY_FLOW_END0(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_END, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY)
#define TRACE_EVENT_COPY_FLOW_END1(category_group, name, id, arg1_name, \
        arg1_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_END, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val)
#define TRACE_EVENT_COPY_FLOW_END2(category_group, name, id, arg1_name, \
        arg1_val, arg2_name, arg2_val) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_FLOW_END, \
        category_group, name, id, TRACE_EVENT_FLAG_COPY, \
        arg1_name, arg1_val, arg2_name, arg2_val)

#define TRACE_EVENT_OBJECT_CREATED_WITH_ID(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_CREATE_OBJECT, \
        category_group, name, TRACE_ID_DONT_MANGLE(id), TRACE_EVENT_FLAG_NONE)

#define TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(category_group, name, id, snapshot) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_SNAPSHOT_OBJECT, \
        category_group, name, TRACE_ID_DONT_MANGLE(id), TRACE_EVENT_FLAG_NONE,\
        "snapshot", snapshot)

#define TRACE_EVENT_OBJECT_DELETED_WITH_ID(category_group, name, id) \
    INTERNAL_TRACE_EVENT_ADD_WITH_ID(TRACE_EVENT_PHASE_DELETE_OBJECT, \
        category_group, name, TRACE_ID_DONT_MANGLE(id), TRACE_EVENT_FLAG_NONE)


#define TRACE_EVENT_CATEGORY_GROUP_ENABLED(category_group, ret) \
    do { \
      INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group); \
      if (*INTERNAL_TRACE_EVENT_UID(category_group_enabled)) { \
        *ret = true; \
      } else { \
        *ret = false; \
      } \
    } while (0)

#define TRACE_EVENT_IS_NEW_TRACE(ret) \
    do { \
      static int INTERNAL_TRACE_EVENT_UID(lastRecordingNumber) = 0; \
      int num_traces_recorded = TRACE_EVENT_API_GET_NUM_TRACES_RECORDED(); \
      if (num_traces_recorded != -1 && \
          num_traces_recorded != \
          INTERNAL_TRACE_EVENT_UID(lastRecordingNumber)) { \
        INTERNAL_TRACE_EVENT_UID(lastRecordingNumber) = \
            num_traces_recorded; \
        *ret = true; \
      } else { \
        *ret = false; \
      } \
    } while (0)


#define TRACE_EVENT_API_GET_CATEGORY_GROUP_ENABLED \
    base::debug::TraceLog::GetCategoryGroupEnabled

#define TRACE_EVENT_API_GET_NUM_TRACES_RECORDED \
    base::debug::TraceLog::GetInstance()->GetNumTracesRecorded

#define TRACE_EVENT_API_ADD_TRACE_EVENT \
    base::debug::TraceLog::GetInstance()->AddTraceEvent

#define TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP \
    base::debug::TraceLog::GetInstance()->AddTraceEventWithThreadIdAndTimestamp

#define TRACE_EVENT_API_UPDATE_TRACE_EVENT_DURATION \
    base::debug::TraceLog::GetInstance()->UpdateTraceEventDuration

#define TRACE_EVENT_API_ATOMIC_WORD base::subtle::AtomicWord
#define TRACE_EVENT_API_ATOMIC_LOAD(var) base::subtle::NoBarrier_Load(&(var))
#define TRACE_EVENT_API_ATOMIC_STORE(var, value) \
    base::subtle::NoBarrier_Store(&(var), (value))

#define TRACE_EVENT_API_CLASS_EXPORT BASE_EXPORT

TRACE_EVENT_API_CLASS_EXPORT extern \
    TRACE_EVENT_API_ATOMIC_WORD g_trace_state[3];

#define TRACE_EVENT_API_THREAD_BUCKET(thread_bucket)                           \
    g_trace_state[thread_bucket]


#define INTERNAL_TRACE_EVENT_UID3(a,b) \
    trace_event_unique_##a##b
#define INTERNAL_TRACE_EVENT_UID2(a,b) \
    INTERNAL_TRACE_EVENT_UID3(a,b)
#define INTERNAL_TRACE_EVENT_UID(name_prefix) \
    INTERNAL_TRACE_EVENT_UID2(name_prefix, __LINE__)

#define INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO_CUSTOM_VARIABLES( \
    category_group, atomic, category_group_enabled) \
    category_group_enabled = \
        reinterpret_cast<const unsigned char*>(TRACE_EVENT_API_ATOMIC_LOAD( \
            atomic)); \
    if (!category_group_enabled) { \
      category_group_enabled = \
          TRACE_EVENT_API_GET_CATEGORY_GROUP_ENABLED(category_group); \
      TRACE_EVENT_API_ATOMIC_STORE(atomic, \
          reinterpret_cast<TRACE_EVENT_API_ATOMIC_WORD>( \
              category_group_enabled)); \
    }

#define INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group) \
    static TRACE_EVENT_API_ATOMIC_WORD INTERNAL_TRACE_EVENT_UID(atomic) = 0; \
    const unsigned char* INTERNAL_TRACE_EVENT_UID(category_group_enabled); \
    INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO_CUSTOM_VARIABLES(category_group, \
        INTERNAL_TRACE_EVENT_UID(atomic), \
        INTERNAL_TRACE_EVENT_UID(category_group_enabled));

#define INTERNAL_TRACE_EVENT_ADD(phase, category_group, name, flags, ...) \
    do { \
      INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group); \
      if (*INTERNAL_TRACE_EVENT_UID(category_group_enabled)) { \
        trace_event_internal::AddTraceEvent( \
            phase, INTERNAL_TRACE_EVENT_UID(category_group_enabled), name, \
            trace_event_internal::kNoEventId, flags, ##__VA_ARGS__); \
      } \
    } while (0)

#define INTERNAL_TRACE_EVENT_ADD_SCOPED(category_group, name, ...) \
    INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group); \
    trace_event_internal::ScopedTracer INTERNAL_TRACE_EVENT_UID(tracer); \
    if (*INTERNAL_TRACE_EVENT_UID(category_group_enabled)) { \
      base::debug::TraceEventHandle h = trace_event_internal::AddTraceEvent( \
          TRACE_EVENT_PHASE_COMPLETE, \
          INTERNAL_TRACE_EVENT_UID(category_group_enabled), \
          name, trace_event_internal::kNoEventId, \
          TRACE_EVENT_FLAG_NONE, ##__VA_ARGS__); \
      INTERNAL_TRACE_EVENT_UID(tracer).Initialize( \
          INTERNAL_TRACE_EVENT_UID(category_group_enabled), name, h); \
    }

#define INTERNAL_TRACE_EVENT_ADD_WITH_ID(phase, category_group, name, id, \
                                         flags, ...) \
    do { \
      INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group); \
      if (*INTERNAL_TRACE_EVENT_UID(category_group_enabled)) { \
        unsigned char trace_event_flags = flags | TRACE_EVENT_FLAG_HAS_ID; \
        trace_event_internal::TraceID trace_event_trace_id( \
            id, &trace_event_flags); \
        trace_event_internal::AddTraceEvent( \
            phase, INTERNAL_TRACE_EVENT_UID(category_group_enabled), \
            name, trace_event_trace_id.data(), trace_event_flags, \
            ##__VA_ARGS__); \
      } \
    } while (0)

#define INTERNAL_TRACE_EVENT_ADD_WITH_ID_TID_AND_TIMESTAMP(phase, \
        category_group, name, id, thread_id, timestamp, flags, ...) \
    do { \
      INTERNAL_TRACE_EVENT_GET_CATEGORY_INFO(category_group); \
      if (*INTERNAL_TRACE_EVENT_UID(category_group_enabled)) { \
        unsigned char trace_event_flags = flags | TRACE_EVENT_FLAG_HAS_ID; \
        trace_event_internal::TraceID trace_event_trace_id( \
            id, &trace_event_flags); \
        trace_event_internal::AddTraceEventWithThreadIdAndTimestamp( \
            phase, INTERNAL_TRACE_EVENT_UID(category_group_enabled), \
            name, trace_event_trace_id.data(), \
            thread_id, base::TimeTicks::FromInternalValue(timestamp), \
            trace_event_flags, ##__VA_ARGS__); \
      } \
    } while (0)


#define TRACE_EVENT_PHASE_BEGIN    ('B')
#define TRACE_EVENT_PHASE_END      ('E')
#define TRACE_EVENT_PHASE_COMPLETE ('X')
#define TRACE_EVENT_PHASE_INSTANT  ('i')
#define TRACE_EVENT_PHASE_ASYNC_BEGIN ('S')
#define TRACE_EVENT_PHASE_ASYNC_STEP_INTO  ('T')
#define TRACE_EVENT_PHASE_ASYNC_STEP_PAST  ('p')
#define TRACE_EVENT_PHASE_ASYNC_END   ('F')
#define TRACE_EVENT_PHASE_FLOW_BEGIN ('s')
#define TRACE_EVENT_PHASE_FLOW_STEP  ('t')
#define TRACE_EVENT_PHASE_FLOW_END   ('f')
#define TRACE_EVENT_PHASE_METADATA ('M')
#define TRACE_EVENT_PHASE_COUNTER  ('C')
#define TRACE_EVENT_PHASE_SAMPLE  ('P')
#define TRACE_EVENT_PHASE_CREATE_OBJECT ('N')
#define TRACE_EVENT_PHASE_SNAPSHOT_OBJECT ('O')
#define TRACE_EVENT_PHASE_DELETE_OBJECT ('D')

#define TRACE_EVENT_FLAG_NONE         (static_cast<unsigned char>(0))
#define TRACE_EVENT_FLAG_COPY         (static_cast<unsigned char>(1 << 0))
#define TRACE_EVENT_FLAG_HAS_ID       (static_cast<unsigned char>(1 << 1))
#define TRACE_EVENT_FLAG_MANGLE_ID    (static_cast<unsigned char>(1 << 2))
#define TRACE_EVENT_FLAG_SCOPE_OFFSET (static_cast<unsigned char>(1 << 3))

#define TRACE_EVENT_FLAG_SCOPE_MASK   (static_cast<unsigned char>( \
    TRACE_EVENT_FLAG_SCOPE_OFFSET | (TRACE_EVENT_FLAG_SCOPE_OFFSET << 1)))

#define TRACE_VALUE_TYPE_BOOL         (static_cast<unsigned char>(1))
#define TRACE_VALUE_TYPE_UINT         (static_cast<unsigned char>(2))
#define TRACE_VALUE_TYPE_INT          (static_cast<unsigned char>(3))
#define TRACE_VALUE_TYPE_DOUBLE       (static_cast<unsigned char>(4))
#define TRACE_VALUE_TYPE_POINTER      (static_cast<unsigned char>(5))
#define TRACE_VALUE_TYPE_STRING       (static_cast<unsigned char>(6))
#define TRACE_VALUE_TYPE_COPY_STRING  (static_cast<unsigned char>(7))
#define TRACE_VALUE_TYPE_CONVERTABLE  (static_cast<unsigned char>(8))

#define TRACE_EVENT_SCOPE_GLOBAL  (static_cast<unsigned char>(0 << 3))
#define TRACE_EVENT_SCOPE_PROCESS (static_cast<unsigned char>(1 << 3))
#define TRACE_EVENT_SCOPE_THREAD  (static_cast<unsigned char>(2 << 3))

#define TRACE_EVENT_SCOPE_NAME_GLOBAL  ('g')
#define TRACE_EVENT_SCOPE_NAME_PROCESS ('p')
#define TRACE_EVENT_SCOPE_NAME_THREAD  ('t')

namespace trace_event_internal {

const int kZeroNumArgs = 0;
const unsigned long long kNoEventId = 0;

class TraceID {
 public:
  class DontMangle {
   public:
    explicit DontMangle(const void* id)
        : data_(static_cast<unsigned long long>(
              reinterpret_cast<unsigned long>(id))) {}
    explicit DontMangle(unsigned long long id) : data_(id) {}
    explicit DontMangle(unsigned long id) : data_(id) {}
    explicit DontMangle(unsigned int id) : data_(id) {}
    explicit DontMangle(unsigned short id) : data_(id) {}
    explicit DontMangle(unsigned char id) : data_(id) {}
    explicit DontMangle(long long id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit DontMangle(long id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit DontMangle(int id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit DontMangle(short id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit DontMangle(signed char id)
        : data_(static_cast<unsigned long long>(id)) {}
    unsigned long long data() const { return data_; }
   private:
    unsigned long long data_;
  };

  class ForceMangle {
   public:
    explicit ForceMangle(unsigned long long id) : data_(id) {}
    explicit ForceMangle(unsigned long id) : data_(id) {}
    explicit ForceMangle(unsigned int id) : data_(id) {}
    explicit ForceMangle(unsigned short id) : data_(id) {}
    explicit ForceMangle(unsigned char id) : data_(id) {}
    explicit ForceMangle(long long id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit ForceMangle(long id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit ForceMangle(int id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit ForceMangle(short id)
        : data_(static_cast<unsigned long long>(id)) {}
    explicit ForceMangle(signed char id)
        : data_(static_cast<unsigned long long>(id)) {}
    unsigned long long data() const { return data_; }
   private:
    unsigned long long data_;
  };

  TraceID(const void* id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(
              reinterpret_cast<unsigned long>(id))) {
    *flags |= TRACE_EVENT_FLAG_MANGLE_ID;
  }
  TraceID(ForceMangle id, unsigned char* flags) : data_(id.data()) {
    *flags |= TRACE_EVENT_FLAG_MANGLE_ID;
  }
  TraceID(DontMangle id, unsigned char* flags) : data_(id.data()) {
  }
  TraceID(unsigned long long id, unsigned char* flags)
      : data_(id) { (void)flags; }
  TraceID(unsigned long id, unsigned char* flags)
      : data_(id) { (void)flags; }
  TraceID(unsigned int id, unsigned char* flags)
      : data_(id) { (void)flags; }
  TraceID(unsigned short id, unsigned char* flags)
      : data_(id) { (void)flags; }
  TraceID(unsigned char id, unsigned char* flags)
      : data_(id) { (void)flags; }
  TraceID(long long id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) { (void)flags; }
  TraceID(long id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) { (void)flags; }
  TraceID(int id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) { (void)flags; }
  TraceID(short id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) { (void)flags; }
  TraceID(signed char id, unsigned char* flags)
      : data_(static_cast<unsigned long long>(id)) { (void)flags; }

  unsigned long long data() const { return data_; }

 private:
  unsigned long long data_;
};

union TraceValueUnion {
  bool as_bool;
  unsigned long long as_uint;
  long long as_int;
  double as_double;
  const void* as_pointer;
  const char* as_string;
};

class TraceStringWithCopy {
 public:
  explicit TraceStringWithCopy(const char* str) : str_(str) {}
  operator const char* () const { return str_; }
 private:
  const char* str_;
};

#define INTERNAL_DECLARE_SET_TRACE_VALUE(actual_type, \
                                         union_member, \
                                         value_type_id) \
    static inline void SetTraceValue( \
        actual_type arg, \
        unsigned char* type, \
        unsigned long long* value) { \
      TraceValueUnion type_value; \
      type_value.union_member = arg; \
      *type = value_type_id; \
      *value = type_value.as_uint; \
    }
#define INTERNAL_DECLARE_SET_TRACE_VALUE_INT(actual_type, \
                                             value_type_id) \
    static inline void SetTraceValue( \
        actual_type arg, \
        unsigned char* type, \
        unsigned long long* value) { \
      *type = value_type_id; \
      *value = static_cast<unsigned long long>(arg); \
    }

INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned long long, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned long, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned int, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned short, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(unsigned char, TRACE_VALUE_TYPE_UINT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(long long, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(long, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(int, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(short, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE_INT(signed char, TRACE_VALUE_TYPE_INT)
INTERNAL_DECLARE_SET_TRACE_VALUE(bool, as_bool, TRACE_VALUE_TYPE_BOOL)
INTERNAL_DECLARE_SET_TRACE_VALUE(double, as_double, TRACE_VALUE_TYPE_DOUBLE)
INTERNAL_DECLARE_SET_TRACE_VALUE(const void*, as_pointer,
                                 TRACE_VALUE_TYPE_POINTER)
INTERNAL_DECLARE_SET_TRACE_VALUE(const char*, as_string,
                                 TRACE_VALUE_TYPE_STRING)
INTERNAL_DECLARE_SET_TRACE_VALUE(const TraceStringWithCopy&, as_string,
                                 TRACE_VALUE_TYPE_COPY_STRING)

#undef INTERNAL_DECLARE_SET_TRACE_VALUE
#undef INTERNAL_DECLARE_SET_TRACE_VALUE_INT

static inline void SetTraceValue(const std::string& arg,
                                 unsigned char* type,
                                 unsigned long long* value) {
  TraceValueUnion type_value;
  type_value.as_string = arg.c_str();
  *type = TRACE_VALUE_TYPE_COPY_STRING;
  *value = type_value.as_uint;
}


static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags,
    const char* arg1_name,
    const scoped_refptr<base::debug::ConvertableToTraceFormat>& arg1_val) {
  const int num_args = 1;
  unsigned char arg_types[1] = { TRACE_VALUE_TYPE_CONVERTABLE };
  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      num_args, &arg1_name, arg_types, NULL, &arg1_val, flags);
}

template<class ARG1_TYPE>
static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags,
    const char* arg1_name,
    const ARG1_TYPE& arg1_val,
    const char* arg2_name,
    const scoped_refptr<base::debug::ConvertableToTraceFormat>& arg2_val) {
  const int num_args = 2;
  const char* arg_names[2] = { arg1_name, arg2_name };

  unsigned char arg_types[2];
  unsigned long long arg_values[2];
  SetTraceValue(arg1_val, &arg_types[0], &arg_values[0]);
  arg_types[1] = TRACE_VALUE_TYPE_CONVERTABLE;

  scoped_refptr<base::debug::ConvertableToTraceFormat> convertable_values[2];
  convertable_values[1] = arg2_val;

  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      num_args, arg_names, arg_types, arg_values, convertable_values, flags);
}

template<class ARG2_TYPE>
static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags,
    const char* arg1_name,
    const scoped_refptr<base::debug::ConvertableToTraceFormat>& arg1_val,
    const char* arg2_name,
    const ARG2_TYPE& arg2_val) {
  const int num_args = 2;
  const char* arg_names[2] = { arg1_name, arg2_name };

  unsigned char arg_types[2];
  unsigned long long arg_values[2];
  arg_types[0] = TRACE_VALUE_TYPE_CONVERTABLE;
  arg_values[0] = 0;
  SetTraceValue(arg2_val, &arg_types[1], &arg_values[1]);

  scoped_refptr<base::debug::ConvertableToTraceFormat> convertable_values[2];
  convertable_values[0] = arg1_val;

  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      num_args, arg_names, arg_types, arg_values, convertable_values, flags);
}

static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags,
    const char* arg1_name,
    const scoped_refptr<base::debug::ConvertableToTraceFormat>& arg1_val,
    const char* arg2_name,
    const scoped_refptr<base::debug::ConvertableToTraceFormat>& arg2_val) {
  const int num_args = 2;
  const char* arg_names[2] = { arg1_name, arg2_name };
  unsigned char arg_types[2] =
      { TRACE_VALUE_TYPE_CONVERTABLE, TRACE_VALUE_TYPE_CONVERTABLE };
  scoped_refptr<base::debug::ConvertableToTraceFormat> convertable_values[2] =
      { arg1_val, arg2_val };

  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      num_args, arg_names, arg_types, NULL, convertable_values, flags);
}

static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags) {
  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      kZeroNumArgs, NULL, NULL, NULL, NULL, flags);
}

static inline base::debug::TraceEventHandle AddTraceEvent(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    unsigned char flags) {
  int thread_id = static_cast<int>(base::PlatformThread::CurrentId());
  base::TimeTicks now = base::TimeTicks::NowFromSystemTraceTime();
  return AddTraceEventWithThreadIdAndTimestamp(phase, category_group_enabled,
                                               name, id, thread_id, now, flags);
}

template<class ARG1_TYPE>
static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags,
    const char* arg1_name,
    const ARG1_TYPE& arg1_val) {
  const int num_args = 1;
  unsigned char arg_types[1];
  unsigned long long arg_values[1];
  SetTraceValue(arg1_val, &arg_types[0], &arg_values[0]);
  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      num_args, &arg1_name, arg_types, arg_values, NULL, flags);
}

template<class ARG1_TYPE>
static inline base::debug::TraceEventHandle AddTraceEvent(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    unsigned char flags,
    const char* arg1_name,
    const ARG1_TYPE& arg1_val) {
  int thread_id = static_cast<int>(base::PlatformThread::CurrentId());
  base::TimeTicks now = base::TimeTicks::NowFromSystemTraceTime();
  return AddTraceEventWithThreadIdAndTimestamp(phase, category_group_enabled,
                                               name, id, thread_id, now, flags,
                                               arg1_name, arg1_val);
}

template<class ARG1_TYPE, class ARG2_TYPE>
static inline base::debug::TraceEventHandle
AddTraceEventWithThreadIdAndTimestamp(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int thread_id,
    const base::TimeTicks& timestamp,
    unsigned char flags,
    const char* arg1_name,
    const ARG1_TYPE& arg1_val,
    const char* arg2_name,
    const ARG2_TYPE& arg2_val) {
  const int num_args = 2;
  const char* arg_names[2] = { arg1_name, arg2_name };
  unsigned char arg_types[2];
  unsigned long long arg_values[2];
  SetTraceValue(arg1_val, &arg_types[0], &arg_values[0]);
  SetTraceValue(arg2_val, &arg_types[1], &arg_values[1]);
  return TRACE_EVENT_API_ADD_TRACE_EVENT_WITH_THREAD_ID_AND_TIMESTAMP(
      phase, category_group_enabled, name, id, thread_id, timestamp,
      num_args, arg_names, arg_types, arg_values, NULL, flags);
}

template<class ARG1_TYPE, class ARG2_TYPE>
static inline base::debug::TraceEventHandle AddTraceEvent(
    char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    unsigned char flags,
    const char* arg1_name,
    const ARG1_TYPE& arg1_val,
    const char* arg2_name,
    const ARG2_TYPE& arg2_val) {
  int thread_id = static_cast<int>(base::PlatformThread::CurrentId());
  base::TimeTicks now = base::TimeTicks::NowFromSystemTraceTime();
  return AddTraceEventWithThreadIdAndTimestamp(phase, category_group_enabled,
                                               name, id, thread_id, now, flags,
                                               arg1_name, arg1_val,
                                               arg2_name, arg2_val);
}

class TRACE_EVENT_API_CLASS_EXPORT ScopedTracer {
 public:
  
  ScopedTracer() : p_data_(NULL) {}

  ~ScopedTracer() {
    if (p_data_ && *data_.category_group_enabled)
      TRACE_EVENT_API_UPDATE_TRACE_EVENT_DURATION(
          data_.category_group_enabled, data_.name, data_.event_handle);
  }

  void Initialize(const unsigned char* category_group_enabled,
                  const char* name,
                  base::debug::TraceEventHandle event_handle) {
    data_.category_group_enabled = category_group_enabled;
    data_.name = name;
    data_.event_handle = event_handle;
    p_data_ = &data_;
  }

 private:
  
  
  
  
  
  struct Data {
    const unsigned char* category_group_enabled;
    const char* name;
    base::debug::TraceEventHandle event_handle;
  };
  Data* p_data_;
  Data data_;
};

class TRACE_EVENT_API_CLASS_EXPORT ScopedTraceBinaryEfficient {
 public:
  ScopedTraceBinaryEfficient(const char* category_group, const char* name);
  ~ScopedTraceBinaryEfficient();

 private:
  const unsigned char* category_group_enabled_;
  const char* name_;
  base::debug::TraceEventHandle event_handle_;
};

#define TRACE_EVENT_BINARY_EFFICIENT0(category_group, name) \
    trace_event_internal::ScopedTraceBinaryEfficient \
        INTERNAL_TRACE_EVENT_UID(scoped_trace)(category_group, name);

template<size_t BucketNumber>
class TraceEventSamplingStateScope {
 public:
  TraceEventSamplingStateScope(const char* category_and_name) {
    previous_state_ = TraceEventSamplingStateScope<BucketNumber>::Current();
    TraceEventSamplingStateScope<BucketNumber>::Set(category_and_name);
  }

  ~TraceEventSamplingStateScope() {
    TraceEventSamplingStateScope<BucketNumber>::Set(previous_state_);
  }

  static inline const char* Current() {
    return reinterpret_cast<const char*>(TRACE_EVENT_API_ATOMIC_LOAD(
      g_trace_state[BucketNumber]));
  }

  static inline void Set(const char* category_and_name) {
    TRACE_EVENT_API_ATOMIC_STORE(
      g_trace_state[BucketNumber],
      reinterpret_cast<TRACE_EVENT_API_ATOMIC_WORD>(
        const_cast<char*>(category_and_name)));
  }

 private:
  const char* previous_state_;
};

}  

namespace base {
namespace debug {

template<typename IDType> class TraceScopedTrackableObject {
 public:
  TraceScopedTrackableObject(const char* category_group, const char* name,
      IDType id)
    : category_group_(category_group),
      name_(name),
      id_(id) {
    TRACE_EVENT_OBJECT_CREATED_WITH_ID(category_group_, name_, id_);
  }

  template <typename ArgType> void snapshot(ArgType snapshot) {
    TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(category_group_, name_, id_, snapshot);
  }

  ~TraceScopedTrackableObject() {
    TRACE_EVENT_OBJECT_DELETED_WITH_ID(category_group_, name_, id_);
  }

 private:
  const char* category_group_;
  const char* name_;
  IDType id_;

  DISALLOW_COPY_AND_ASSIGN(TraceScopedTrackableObject);
};

} 
} 

#endif 
