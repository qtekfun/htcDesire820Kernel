// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_SCHEDULE_TIMER_TEST_H
#define CONTENT_TEST_PLUGIN_PLUGIN_SCHEDULE_TIMER_TEST_H

#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "content/test/plugin/plugin_test.h"

namespace NPAPIClient {

class ScheduleTimerTest : public PluginTest {
 public:
  ScheduleTimerTest(NPP id, NPNetscapeFuncs *host_functions);

  virtual NPError New(uint16 mode, int16 argc, const char* argn[],
                      const char* argv[], NPSavedData* saved) OVERRIDE;

  void OnTimer(uint32 timer_id);

 private:
  
  static const int kNumTimers = 3;
  uint32 timer_ids_[kNumTimers];

  
  static const int kNumEvents = 11;
  struct Event {
    int time;

    
    
    int received_index;

    
    int scheduled_index;

    
    uint32 scheduled_interval;
    bool schedule_repeated;

    
    int unscheduled_index;
  };
  static Event schedule_[kNumEvents];
  int num_received_events_;

  
  bool received_events_[kNumEvents];

  
  base::Time start_time_;

  
  int FindUnreceivedEvent(int time, uint32 timer_id);
  void HandleEventIndex(int event_index);
};

}  

#endif  
