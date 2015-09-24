// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_DISPATCHER_H_
#define UI_EVENTS_EVENT_DISPATCHER_H_

#include "base/auto_reset.h"
#include "ui/events/event.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_handler.h"
#include "ui/events/events_export.h"

namespace ui {

class EventDispatcher;
class EventTarget;
class EventTargeter;

struct EventDispatchDetails {
  EventDispatchDetails()
      : dispatcher_destroyed(false),
        target_destroyed(false) {}
  bool dispatcher_destroyed;
  bool target_destroyed;
};

class EVENTS_EXPORT EventDispatcherDelegate {
 public:
  EventDispatcherDelegate();
  virtual ~EventDispatcherDelegate();

  
  
  
  virtual bool CanDispatchToTarget(EventTarget* target) = 0;

  
  
  Event* current_event();

  
  
  
  EventDispatchDetails DispatchEvent(EventTarget* target, Event* event)
      WARN_UNUSED_RESULT;

 protected:
  
  
  
  virtual EventDispatchDetails PreDispatchEvent(
      EventTarget* target,
      Event* event) WARN_UNUSED_RESULT;

  
  virtual EventDispatchDetails PostDispatchEvent(
      EventTarget* target,
      const Event& event) WARN_UNUSED_RESULT;

 private:
  
  EventDispatchDetails DispatchEventToTarget(EventTarget* target,
                                             Event* event) WARN_UNUSED_RESULT;

  EventDispatcher* dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(EventDispatcherDelegate);
};

class EVENTS_EXPORT EventDispatcher {
 public:
  explicit EventDispatcher(EventDispatcherDelegate* delegate);
  virtual ~EventDispatcher();

  void ProcessEvent(EventTarget* target, Event* event);

  const Event* current_event() const { return current_event_; }
  Event* current_event() { return current_event_; }

  bool delegate_destroyed() const { return !delegate_; }
  const EventDispatchDetails& details() const { return details_; }

  void OnHandlerDestroyed(EventHandler* handler);
  void OnDispatcherDelegateDestroyed();

 private:
  void DispatchEventToEventHandlers(EventHandlerList* list, Event* event);

  
  
  
  void DispatchEvent(EventHandler* handler, Event* event);

  EventDispatcherDelegate* delegate_;

  Event* current_event_;

  EventHandlerList handler_list_;

  EventDispatchDetails details_;

  DISALLOW_COPY_AND_ASSIGN(EventDispatcher);
};

}  

#endif  
