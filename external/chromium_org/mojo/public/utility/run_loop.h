// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_UTILITY_RUN_LOOP_H_
#define MOJO_PUBLIC_UTILITY_RUN_LOOP_H_

#include <map>

#include "mojo/public/system/core_cpp.h"

namespace mojo {
namespace utility {

class RunLoopHandler;

class RunLoop {
 public:
  RunLoop();
  ~RunLoop();

  
  
  static void SetUp();

  
  static void TearDown();

  
  
  static RunLoop* current();

  
  
  void AddHandler(RunLoopHandler* handler,
                  const Handle& handle,
                  MojoWaitFlags wait_flags,
                  MojoDeadline deadline);
  void RemoveHandler(const Handle& handle);

  
  
  void Run();
  void Quit();

 private:
  struct RunState;
  struct WaitState;

  
  struct HandlerData {
    HandlerData()
        : handler(NULL),
          wait_flags(MOJO_WAIT_FLAG_NONE),
          deadline(0),
          id(0) {}

    RunLoopHandler* handler;
    MojoWaitFlags wait_flags;
    MojoTimeTicks deadline;
    
    int id;
  };

  typedef std::map<Handle, HandlerData> HandleToHandlerData;

  
  
  void Wait();

  
  void NotifyDeadlineExceeded();

  
  
  void RemoveFirstInvalidHandle(const WaitState& wait_state);

  
  WaitState GetWaitState() const;

  HandleToHandlerData handler_data_;

  
  
  RunState* run_state_;

  
  
  
  
  
  int next_handler_id_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(RunLoop);
};

}  
}  

#endif  
