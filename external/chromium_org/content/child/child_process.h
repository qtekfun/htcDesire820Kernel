// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_CHILD_PROCESS_H_
#define CONTENT_CHILD_CHILD_PROCESS_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"

namespace content {
class ChildThread;

class CONTENT_EXPORT ChildProcess {
 public:
  
  
  ChildProcess();
  virtual ~ChildProcess();

  
  ChildThread* main_thread();

  
  
  void set_main_thread(ChildThread* thread);

  base::MessageLoop* io_message_loop() { return io_thread_.message_loop(); }
  base::MessageLoopProxy* io_message_loop_proxy() {
    return io_thread_.message_loop_proxy().get();
  }

  
  
  
  
  
  
  
  
  
  base::WaitableEvent* GetShutDownEvent();

  
  
  
  
  void AddRefProcess();
  void ReleaseProcess();

  
  
  static ChildProcess* current();

  static void WaitForDebugger(const std::string& label);
 private:
  int ref_count_;

  
  base::WaitableEvent shutdown_event_;

  
  base::Thread io_thread_;

  
  
  
  scoped_ptr<ChildThread> main_thread_;

  DISALLOW_COPY_AND_ASSIGN(ChildProcess);
};

}  

#endif  
