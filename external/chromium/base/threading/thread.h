// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/message_loop.h"
#include "base/message_loop_proxy.h"
#include "base/threading/platform_thread.h"

namespace base {

class BASE_API Thread : PlatformThread::Delegate {
 public:
  struct Options {
    Options() : message_loop_type(MessageLoop::TYPE_DEFAULT), stack_size(0) {}
    Options(MessageLoop::Type type, size_t size)
        : message_loop_type(type), stack_size(size) {}

    
    MessageLoop::Type message_loop_type;

    
    
    
    size_t stack_size;
  };

  
  
  explicit Thread(const char* name);

  
  
  
  
  
  virtual ~Thread();

  
  
  
  
  
  
  
  bool Start();

  
  
  
  
  
  
  bool StartWithOptions(const Options& options);

  
  
  
  
  
  
  
  
  
  
  
  void Stop();

  
  
  
  
  
  
  
  
  
  
  
  void StopSoon();

  
  
  
  
  
  
  
  
  MessageLoop* message_loop() const { return message_loop_; }

  
  
  
  
  
  
  scoped_refptr<MessageLoopProxy> message_loop_proxy() {
    return message_loop_proxy_;
  }

  
  const std::string &thread_name() { return name_; }

  
  PlatformThreadHandle thread_handle() { return thread_; }

  
  PlatformThreadId thread_id() const { return thread_id_; }

  
  
  bool IsRunning() const { return thread_id_ != kInvalidThreadId; }

 protected:
  
  virtual void Init() {}

  
  virtual void Run(MessageLoop* message_loop);

  
  virtual void CleanUp() {}

  
  
  
  virtual void CleanUpAfterMessageLoopDestruction() {}

  static void SetThreadWasQuitProperly(bool flag);
  static bool GetThreadWasQuitProperly();

  void set_message_loop(MessageLoop* message_loop) {
    message_loop_ = message_loop;
  }

 private:
  bool thread_was_started() const { return started_; }

  
  virtual void ThreadMain();

  
  bool started_;

  
  
  bool stopping_;

  
  struct StartupData;
  StartupData* startup_data_;

  
  PlatformThreadHandle thread_;

  
  
  MessageLoop* message_loop_;

  
  
  scoped_refptr<MessageLoopProxy> message_loop_proxy_;

  
  PlatformThreadId thread_id_;

  
  std::string name_;

  friend class ThreadQuitTask;

  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  

#endif  
