// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_

#include "base/message_loop/message_pump.h"
#include "base/synchronization/waitable_event.h"
#include "base/time/time.h"

namespace base {

class MessagePumpDefault : public MessagePump {
 public:
  MessagePumpDefault();
  virtual ~MessagePumpDefault();

  
  virtual void Run(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;
  virtual void ScheduleWork() OVERRIDE;
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time) OVERRIDE;

 private:
  
  bool keep_running_;

  
  WaitableEvent event_;

  
  TimeTicks delayed_work_time_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpDefault);
};

}  

#endif  
