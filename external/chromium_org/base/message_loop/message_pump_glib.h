// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_GLIB_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_GLIB_H_

#include "base/base_export.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_pump.h"
#include "base/observer_list.h"
#include "base/time/time.h"

typedef struct _GMainContext GMainContext;
typedef struct _GPollFD GPollFD;
typedef struct _GSource GSource;

namespace base {

class MessagePumpObserver;

class MessagePumpDispatcher;

class BASE_EXPORT MessagePumpGlib : public MessagePump {
 public:
  MessagePumpGlib();
  virtual ~MessagePumpGlib();

  
  virtual void RunWithDispatcher(Delegate* delegate,
                                 MessagePumpDispatcher* dispatcher);

  
  
  
  
  
  
  int HandlePrepare();
  bool HandleCheck();
  void HandleDispatch();

  
  virtual void Run(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;
  virtual void ScheduleWork() OVERRIDE;
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time) OVERRIDE;

 protected:
  
  MessagePumpDispatcher* GetDispatcher();

 private:
  
  
  struct RunState;

  RunState* state_;

  
  
  
  GMainContext* context_;

  
  TimeTicks delayed_work_time_;

  
  
  GSource* work_source_;

  
  
  
  
  int wakeup_pipe_read_;
  int wakeup_pipe_write_;
  
  scoped_ptr<GPollFD> wakeup_gpollfd_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpGlib);
};

}  

#endif  
