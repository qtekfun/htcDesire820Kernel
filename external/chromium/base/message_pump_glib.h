// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_PUMP_GLIB_H_
#define BASE_MESSAGE_PUMP_GLIB_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/message_pump.h"
#include "base/observer_list.h"
#include "base/time.h"

typedef union _GdkEvent GdkEvent;
typedef struct _GMainContext GMainContext;
typedef struct _GPollFD GPollFD;
typedef struct _GSource GSource;

namespace base {

class MessagePumpForUI : public MessagePump {
 public:
  
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void WillProcessEvent(GdkEvent* event) = 0;

    
    virtual void DidProcessEvent(GdkEvent* event) = 0;
  };

  
  
  
  
  
  
  
  
  class Dispatcher {
   public:
    virtual ~Dispatcher() {}
    
    
    virtual bool Dispatch(GdkEvent* event) = 0;
  };

  MessagePumpForUI();
  virtual ~MessagePumpForUI();

  
  virtual void RunWithDispatcher(Delegate* delegate, Dispatcher* dispatcher);

  
  
  
  virtual bool RunOnce(GMainContext* context, bool block);

  
  
  
  
  
  
  int HandlePrepare();
  bool HandleCheck();
  void HandleDispatch();

  
  void AddObserver(Observer* observer);

  
  
  void RemoveObserver(Observer* observer);

  
  
  virtual void DispatchEvents(GdkEvent* event);

  
  virtual void Run(Delegate* delegate);
  virtual void Quit();
  virtual void ScheduleWork();
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time);

 protected:
  
  Dispatcher* GetDispatcher();

 private:
  
  
  struct RunState;

  
  
  void WillProcessEvent(GdkEvent* event);

  
  
  void DidProcessEvent(GdkEvent* event);

  
  static void EventDispatcher(GdkEvent* event, void* data);

  RunState* state_;

  
  
  
  GMainContext* context_;

  
  TimeTicks delayed_work_time_;

  
  
  GSource* work_source_;

  
  
  
  
  int wakeup_pipe_read_;
  int wakeup_pipe_write_;
  
  scoped_ptr<GPollFD> wakeup_gpollfd_;

  
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpForUI);
};

}  

#endif  
