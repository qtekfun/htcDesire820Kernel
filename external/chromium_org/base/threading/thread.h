// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_THREAD_H_
#define BASE_THREADING_THREAD_H_

#include <string>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/threading/platform_thread.h"

namespace base {

class MessagePump;

class BASE_EXPORT Thread : PlatformThread::Delegate {
 public:
  struct BASE_EXPORT Options {
    typedef Callback<scoped_ptr<MessagePump>()> MessagePumpFactory;

    Options();
    Options(MessageLoop::Type type, size_t size);
    ~Options();

    
    
    MessageLoop::Type message_loop_type;

    
    
    
    
    MessagePumpFactory message_pump_factory;

    
    
    
    size_t stack_size;
  };

  
  
  explicit Thread(const char* name);

  
  
  
  
  
  
  
  
  virtual ~Thread();

#if defined(OS_WIN)
  
  
  
  
  
  void init_com_with_mta(bool use_mta) {
    DCHECK(!started_);
    com_status_ = use_mta ? MTA : STA;
  }
#endif

  
  
  
  
  
  
  
  bool Start();

  
  
  
  
  
  
  bool StartWithOptions(const Options& options);

  
  
  
  
  
  
  
  
  
  
  void Stop();

  
  
  
  
  
  
  
  
  
  
  
  void StopSoon();

  
  
  
  
  
  
  
  
  MessageLoop* message_loop() const { return message_loop_; }

  
  
  
  
  
  scoped_refptr<MessageLoopProxy> message_loop_proxy() const {
    return message_loop_ ? message_loop_->message_loop_proxy() : NULL;
  }

  
  const std::string& thread_name() const { return name_; }

  
  PlatformThreadHandle thread_handle() { return thread_; }

  
  PlatformThreadId thread_id() const { return thread_id_; }

  
  bool IsRunning() const;

  
  void SetPriority(ThreadPriority priority);

 protected:
  
  virtual void Init() {}

  
  virtual void Run(MessageLoop* message_loop);

  
  virtual void CleanUp() {}

  static void SetThreadWasQuitProperly(bool flag);
  static bool GetThreadWasQuitProperly();

  void set_message_loop(MessageLoop* message_loop) {
    message_loop_ = message_loop;
  }

 private:
#if defined(OS_WIN)
  enum ComStatus {
    NONE,
    STA,
    MTA,
  };
#endif

  
  virtual void ThreadMain() OVERRIDE;

#if defined(OS_WIN)
  
  ComStatus com_status_;
#endif

  
  bool started_;

  
  
  bool stopping_;

  
  bool running_;

  
  struct StartupData;
  StartupData* startup_data_;

  
  PlatformThreadHandle thread_;

  
  
  MessageLoop* message_loop_;

  
  PlatformThreadId thread_id_;

  
  std::string name_;

  friend void ThreadQuitHelper();

  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  

#endif  
