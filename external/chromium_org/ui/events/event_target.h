// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_TARGET_H_
#define UI_EVENTS_EVENT_TARGET_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/events/event_handler.h"
#include "ui/events/events_export.h"

namespace ui {

class EventDispatcher;
class EventTargeter;
class EventTargetIterator;
class LocatedEvent;

class EVENTS_EXPORT EventTarget : public EventHandler {
 public:
  class DispatcherApi {
   public:
    explicit DispatcherApi(EventTarget* target) : target_(target) {}

    const EventHandlerList& pre_target_list() const {
      return target_->pre_target_list_;
    }

   private:
    DispatcherApi();
    EventTarget* target_;

    DISALLOW_COPY_AND_ASSIGN(DispatcherApi);
  };

  EventTarget();
  virtual ~EventTarget();

  virtual bool CanAcceptEvent(const Event& event) = 0;

  
  virtual EventTarget* GetParentTarget() = 0;

  
  
  virtual scoped_ptr<EventTargetIterator> GetChildIterator() const = 0;

  
  
  virtual EventTargeter* GetEventTargeter() = 0;

  
  
  virtual void ConvertEventToTarget(EventTarget* target,
                                    LocatedEvent* event);

  
  
  
  void AddPreTargetHandler(EventHandler* handler);

  
  
  void PrependPreTargetHandler(EventHandler* handler);
  void RemovePreTargetHandler(EventHandler* handler);

  
  
  
  void AddPostTargetHandler(EventHandler* handler);
  void RemovePostTargetHandler(EventHandler* handler);

  
  bool IsPreTargetListEmpty() const;

 protected:
  void set_target_handler(EventHandler* handler) {
    target_handler_ = handler;
  }

  
  virtual void OnEvent(Event* event) OVERRIDE;
  virtual void OnKeyEvent(KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(GestureEvent* event) OVERRIDE;

 private:
  friend class EventDispatcher;
  friend class EventTargetTestApi;

  
  
  
  void GetPreTargetHandlers(EventHandlerList* list);

  
  
  
  void GetPostTargetHandlers(EventHandlerList* list);

  EventHandlerList pre_target_list_;
  EventHandlerList post_target_list_;
  EventHandler* target_handler_;

  DISALLOW_COPY_AND_ASSIGN(EventTarget);
};

}  

#endif  
