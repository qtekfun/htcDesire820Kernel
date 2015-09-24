// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_LATENCY_INFO_H_
#define UI_EVENTS_LATENCY_INFO_H_

#include <map>
#include <utility>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "ui/events/events_base_export.h"

namespace ui {

enum LatencyComponentType {
  
  
  
  INPUT_EVENT_LATENCY_BEGIN_RWH_COMPONENT,
  
  
  
  
  
  
  
  INPUT_EVENT_LATENCY_SCROLL_UPDATE_RWH_COMPONENT,
  
  INPUT_EVENT_LATENCY_SCROLL_UPDATE_ORIGINAL_COMPONENT,
  
  INPUT_EVENT_LATENCY_ORIGINAL_COMPONENT,
  
  INPUT_EVENT_LATENCY_UI_COMPONENT,
  
  
  INPUT_EVENT_LATENCY_RENDERING_SCHEDULED_COMPONENT,
  
  INPUT_EVENT_LATENCY_ACKED_TOUCH_COMPONENT,
  
  
  WINDOW_SNAPSHOT_FRAME_NUMBER_COMPONENT,
  
  
  
  
  INPUT_EVENT_LATENCY_TERMINATED_MOUSE_COMPONENT,
  
  
  INPUT_EVENT_LATENCY_TERMINATED_TOUCH_COMPONENT,
  
  
  INPUT_EVENT_LATENCY_TERMINATED_GESTURE_COMPONENT,
  
  
  INPUT_EVENT_LATENCY_TERMINATED_FRAME_SWAP_COMPONENT,
  
  
  INPUT_EVENT_LATENCY_TERMINATED_COMMIT_FAILED_COMPONENT,
  
  
  INPUT_EVENT_LATENCY_TERMINATED_SWAP_FAILED_COMPONENT,
  
  
  LATENCY_INFO_LIST_TERMINATED_OVERFLOW_COMPONENT,
};

struct EVENTS_BASE_EXPORT LatencyInfo {
  struct LatencyComponent {
    
    
    
    int64 sequence_number;
    
    base::TimeTicks event_time;
    
    uint32 event_count;
  };

  
  
  typedef std::map<std::pair<LatencyComponentType, int64>, LatencyComponent>
      LatencyMap;

  LatencyInfo();

  ~LatencyInfo();

  
  void MergeWith(const LatencyInfo& other);

  
  void AddNewLatencyFrom(const LatencyInfo& other);

  
  
  void AddLatencyNumber(LatencyComponentType component,
                        int64 id,
                        int64 component_sequence_number);

  
  
  
  void AddLatencyNumberWithTimestamp(LatencyComponentType component,
                                     int64 id,
                                     int64 component_sequence_number,
                                     base::TimeTicks time,
                                     uint32 event_count,
                                     bool dump_to_trace);

  
  
  
  bool FindLatency(LatencyComponentType type,
                   int64 id,
                   LatencyComponent* output) const;

  void RemoveLatency(LatencyComponentType type);

  void Clear();

  
  void TraceEventType(const char* event_type);

  LatencyMap latency_components;
  
  int64 trace_id;
  
  bool terminated;
};

}  

#endif  
