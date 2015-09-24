// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_THREADING_SIMPLE_THREAD_H_
#define BASE_THREADING_SIMPLE_THREAD_H_

#include <string>
#include <queue>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/threading/platform_thread.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"

namespace base {

class BASE_EXPORT SimpleThread : public PlatformThread::Delegate {
 public:
  class BASE_EXPORT Options {
   public:
    Options() : stack_size_(0) { }
    ~Options() { }

    

    
    void set_stack_size(size_t size) { stack_size_ = size; }
    size_t stack_size() const { return stack_size_; }
   private:
    size_t stack_size_;
  };

  
  
  
  
  explicit SimpleThread(const std::string& name_prefix);
  SimpleThread(const std::string& name_prefix, const Options& options);

  virtual ~SimpleThread();

  virtual void Start();
  virtual void Join();

  
  virtual void Run() = 0;

  
  std::string name_prefix() { return name_prefix_; }

  
  std::string name() { return name_; }

  
  PlatformThreadId tid() { return tid_; }

  
  bool HasBeenStarted();

  
  bool HasBeenJoined() { return joined_; }

  
  virtual void ThreadMain() OVERRIDE;

  
  
  void SetThreadPriority(ThreadPriority priority) {
    PlatformThread::SetThreadPriority(thread_, priority);
  }

 private:
  const std::string name_prefix_;
  std::string name_;
  const Options options_;
  PlatformThreadHandle thread_;  
  WaitableEvent event_;          
  PlatformThreadId tid_;         
  bool joined_;                  
};

class BASE_EXPORT DelegateSimpleThread : public SimpleThread {
 public:
  class BASE_EXPORT Delegate {
   public:
    Delegate() { }
    virtual ~Delegate() { }
    virtual void Run() = 0;
  };

  DelegateSimpleThread(Delegate* delegate,
                       const std::string& name_prefix);
  DelegateSimpleThread(Delegate* delegate,
                       const std::string& name_prefix,
                       const Options& options);

  virtual ~DelegateSimpleThread();
  virtual void Run() OVERRIDE;
 private:
  Delegate* delegate_;
};

class BASE_EXPORT DelegateSimpleThreadPool
    : public DelegateSimpleThread::Delegate {
 public:
  typedef DelegateSimpleThread::Delegate Delegate;

  DelegateSimpleThreadPool(const std::string& name_prefix, int num_threads);
  virtual ~DelegateSimpleThreadPool();

  
  
  void Start();

  
  
  void JoinAll();

  
  
  void AddWork(Delegate* work, int repeat_count);
  void AddWork(Delegate* work) {
    AddWork(work, 1);
  }

  
  virtual void Run() OVERRIDE;

 private:
  const std::string name_prefix_;
  int num_threads_;
  std::vector<DelegateSimpleThread*> threads_;
  std::queue<Delegate*> delegates_;
  base::Lock lock_;            
  WaitableEvent dry_;    
};

}  

#endif  
