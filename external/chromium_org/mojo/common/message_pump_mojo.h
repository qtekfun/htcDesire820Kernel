// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_COMMON_MESSAGE_PUMP_MOJO_H_
#define MOJO_COMMON_MESSAGE_PUMP_MOJO_H_

#include <map>

#include "base/message_loop/message_pump.h"
#include "base/time/time.h"
#include "mojo/common/mojo_common_export.h"
#include "mojo/public/system/core_cpp.h"

namespace mojo {
namespace common {

class MessagePumpMojoHandler;

class MOJO_COMMON_EXPORT MessagePumpMojo : public base::MessagePump {
 public:
  MessagePumpMojo();
  virtual ~MessagePumpMojo();

  
  
  void AddHandler(MessagePumpMojoHandler* handler,
                  const Handle& handle,
                  MojoWaitFlags wait_flags,
                  base::TimeTicks deadline);

  void RemoveHandler(const Handle& handle);

  
  virtual void Run(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;
  virtual void ScheduleWork() OVERRIDE;
  virtual void ScheduleDelayedWork(
      const base::TimeTicks& delayed_work_time) OVERRIDE;

 private:
  struct RunState;
  struct WaitState;

  
  struct Handler {
    Handler() : handler(NULL), wait_flags(MOJO_WAIT_FLAG_NONE), id(0) {}

    MessagePumpMojoHandler* handler;
    MojoWaitFlags wait_flags;
    base::TimeTicks deadline;
    
    int id;
  };

  typedef std::map<Handle, Handler> HandleToHandler;

  
  
  void DoInternalWork(bool block);

  
  
  void RemoveFirstInvalidHandle(const WaitState& wait_state);

  void SignalControlPipe();

  WaitState GetWaitState() const;

  
  MojoDeadline GetDeadlineForWait() const;

  
  
  RunState* run_state_;

  HandleToHandler handlers_;

  
  
  
  
  
  int next_handler_id_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpMojo);
};

}  
}  

#endif  
