// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_PROCESSOR_H_
#define UI_EVENTS_EVENT_PROCESSOR_H_

#include "ui/events/event_dispatcher.h"
#include "ui/events/event_source.h"

namespace ui {

class EVENTS_EXPORT EventProcessor : public EventDispatcherDelegate {
 public:
  virtual ~EventProcessor() {}

  
  virtual EventTarget* GetRootTarget() = 0;

  
  
  virtual EventDispatchDetails OnEventFromSource(Event* event)
      WARN_UNUSED_RESULT;

 protected:
  
  
  
  
  
  virtual void PrepareEventForDispatch(Event* event);
};

}  

#endif  
