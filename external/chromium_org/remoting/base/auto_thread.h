// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_AUTO_THREAD_H_
#define REMOTING_BASE_AUTO_THREAD_H_

#include <string>

#include "base/message_loop/message_loop.h"
#include "base/threading/platform_thread.h"
#include "remoting/base/auto_thread_task_runner.h"

namespace remoting {

class AutoThread : base::PlatformThread::Delegate {
 public:
  
  
  
  static scoped_refptr<AutoThreadTaskRunner> CreateWithType(
      const char* name,
      scoped_refptr<AutoThreadTaskRunner> joiner,
      base::MessageLoop::Type type);
  static scoped_refptr<AutoThreadTaskRunner> Create(
      const char* name,
      scoped_refptr<AutoThreadTaskRunner> joiner);

#if defined(OS_WIN)
  
  
  enum ComInitType { COM_INIT_NONE, COM_INIT_STA, COM_INIT_MTA };
  static scoped_refptr<AutoThreadTaskRunner> CreateWithLoopAndComInitTypes(
      const char* name,
      scoped_refptr<AutoThreadTaskRunner> joiner,
      base::MessageLoop::Type loop_type,
      ComInitType com_init_type);
#endif

  
  explicit AutoThread(const char* name);

  
  virtual ~AutoThread();

  
  
  
  
  
  
  
  
  
  
  scoped_refptr<AutoThreadTaskRunner> StartWithType(
      base::MessageLoop::Type type);

#if defined(OS_WIN)
  
  
  void SetComInitType(ComInitType com_init_type);
#endif

 private:
  AutoThread(const char* name, AutoThreadTaskRunner* joiner);

  void QuitThread(scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  void JoinAndDeleteThread();

  
  virtual void ThreadMain() OVERRIDE;

  
  struct StartupData;
  StartupData* startup_data_;

#if defined(OS_WIN)
  
  ComInitType com_init_type_;
#endif

  
  base::PlatformThreadHandle thread_;

  
  std::string name_;

  
  
  bool was_quit_properly_;

  
  scoped_refptr<AutoThreadTaskRunner> joiner_;

  DISALLOW_COPY_AND_ASSIGN(AutoThread);
};

}  

#endif  
