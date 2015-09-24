// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_EVENT_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_EVENT_H_

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/values.h"

#include "chrome/browser/performance_monitor/event_type.h"

namespace performance_monitor {

const char* EventTypeToString(EventType event_type);

class Event {
 public:
  Event(const EventType& type,
        const base::Time& time,
        scoped_ptr<base::DictionaryValue> data);
  virtual ~Event();

  
  
  static scoped_ptr<Event> FromValue(scoped_ptr<base::DictionaryValue> data);

  
  EventType type() const { return type_; }
  base::Time time() const { return time_; }
  base::DictionaryValue* data() const { return data_.get(); }

 private:

  
  EventType type_;
  
  base::Time time_;
  
  
  scoped_ptr<base::DictionaryValue> data_;
};

}  

#endif  
