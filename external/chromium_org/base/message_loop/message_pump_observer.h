// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_OBSERVER_H
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_OBSERVER_H

#include "base/base_export.h"
#include "base/event_types.h"

namespace base {

enum EventStatus {
  EVENT_CONTINUE,    
#if defined(USE_X11)
  EVENT_HANDLED      
#endif
};

class BASE_EXPORT MessagePumpObserver {
 public:
  
  
  
  
  
  virtual EventStatus WillProcessEvent(const NativeEvent& event) = 0;

  
  
  virtual void DidProcessEvent(const NativeEvent& event) = 0;

 protected:
  virtual ~MessagePumpObserver() {}
};

}  

#endif  
