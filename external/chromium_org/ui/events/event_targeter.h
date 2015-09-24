// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_TARGETER_H_
#define UI_EVENTS_EVENT_TARGETER_H_

#include "base/compiler_specific.h"
#include "ui/events/event.h"
#include "ui/events/events_export.h"

namespace ui {

class Event;
class EventTarget;
class LocatedEvent;

class EVENTS_EXPORT EventTargeter {
 public:
  virtual ~EventTargeter();

  
  
  virtual EventTarget* FindTargetForEvent(EventTarget* root,
                                          Event* event);

  
  
  
  
  
  virtual EventTarget* FindTargetForLocatedEvent(EventTarget* root,
                                                 LocatedEvent* event);

 protected:
  
  
  
  virtual bool SubtreeShouldBeExploredForEvent(EventTarget* target,
                                               const LocatedEvent& event);
};

}  

#endif  
