// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DRIVE_EVENT_LOGGER_H_
#define CHROME_BROWSER_DRIVE_EVENT_LOGGER_H_

#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"

namespace drive {

const int kDefaultHistorySize = 1000;

class EventLogger {
 public:
  
  struct Event {
    Event(int id, logging::LogSeverity severity, const std::string& what);
    int id;  
    logging::LogSeverity severity;  
    base::Time when;  
    std::string what;  
  };

  
  EventLogger();
  ~EventLogger();

  
  
  void Log(logging::LogSeverity severity, const std::string& what);

  
  
  void SetHistorySize(size_t history_size);

  
  
  std::vector<Event> GetHistory();

 private:
  std::deque<Event> history_;  
  size_t history_size_;  
  int next_event_id_;  
  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(EventLogger);
};

}  

#endif  
