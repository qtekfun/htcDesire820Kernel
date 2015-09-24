// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_H_
#define BASE_MESSAGE_LOOP_H_
#pragma once

#include <queue>
#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/message_pump.h"
#include "base/observer_list.h"
#include "base/synchronization/lock.h"
#include "base/task.h"

#if defined(OS_WIN)
#include "base/message_pump_win.h"
#elif defined(OS_POSIX)
#include "base/message_pump_libevent.h"
#if !defined(OS_MACOSX)
#include "base/message_pump_glib.h"
typedef struct _XDisplay Display;
#endif
#endif
#if defined(TOUCH_UI)
#include "base/message_pump_glib_x_dispatch.h"
#endif

namespace base {
class Histogram;
}

class BASE_API MessageLoop : public base::MessagePump::Delegate {
 public:
#if defined(OS_WIN)
  typedef base::MessagePumpWin::Dispatcher Dispatcher;
  typedef base::MessagePumpForUI::Observer Observer;
#elif !defined(OS_MACOSX)
#if defined(TOUCH_UI)
  typedef base::MessagePumpGlibXDispatcher Dispatcher;
#else
  typedef base::MessagePumpForUI::Dispatcher Dispatcher;
#endif
  typedef base::MessagePumpForUI::Observer Observer;
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum Type {
    TYPE_DEFAULT,
    TYPE_UI,
    TYPE_IO
  };

  
  
  explicit MessageLoop(Type type = TYPE_DEFAULT);
  ~MessageLoop();

  
  static MessageLoop* current();

  static void EnableHistogrammer(bool enable_histogrammer);

  
  
  
  
  
  
  
  
  class BASE_API DestructionObserver {
   public:
    virtual void WillDestroyCurrentMessageLoop() = 0;

   protected:
    virtual ~DestructionObserver();
  };

  
  
  void AddDestructionObserver(DestructionObserver* destruction_observer);

  
  
  void RemoveDestructionObserver(DestructionObserver* destruction_observer);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  void PostTask(
      const tracked_objects::Location& from_here, Task* task);

  void PostDelayedTask(
      const tracked_objects::Location& from_here, Task* task, int64 delay_ms);

  void PostNonNestableTask(
      const tracked_objects::Location& from_here, Task* task);

  void PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here, Task* task, int64 delay_ms);

  
  
  
  
  
  
  
  
  
  template <class T>
  void DeleteSoon(const tracked_objects::Location& from_here, const T* object) {
    PostNonNestableTask(from_here, new DeleteTask<T>(object));
  }

  
  
  
  
  
  
  
  
  
  
  template <class T>
  void ReleaseSoon(const tracked_objects::Location& from_here,
                   const T* object) {
    PostNonNestableTask(from_here, new ReleaseTask<T>(object));
  }

  
  void Run();

  
  
  void RunAllPending();

  
  
  
  
  
  
  
  
  
  void Quit();

  
  
  void QuitNow();

  
  
  class QuitTask : public Task {
   public:
    virtual void Run() {
      MessageLoop::current()->Quit();
    }
  };

  
  Type type() const { return type_; }

  
  void set_thread_name(const std::string& thread_name) {
    DCHECK(thread_name_.empty()) << "Should not rename this thread!";
    thread_name_ = thread_name;
  }
  const std::string& thread_name() const { return thread_name_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetNestableTasksAllowed(bool allowed);
  bool NestableTasksAllowed() const;

  
  class ScopedNestableTaskAllower {
   public:
    explicit ScopedNestableTaskAllower(MessageLoop* loop)
        : loop_(loop),
          old_state_(loop_->NestableTasksAllowed()) {
      loop_->SetNestableTasksAllowed(true);
    }
    ~ScopedNestableTaskAllower() {
      loop_->SetNestableTasksAllowed(old_state_);
    }

   private:
    MessageLoop* loop_;
    bool old_state_;
  };

  
  
  
  
  void set_exception_restoration(bool restore) {
    exception_restoration_ = restore;
  }

  
  bool IsNested();

  
  
  
  
  class BASE_API TaskObserver {
   public:
    TaskObserver();

    
    virtual void WillProcessTask(const Task* task) = 0;

    
    virtual void DidProcessTask(const Task* task) = 0;

   protected:
    virtual ~TaskObserver();
  };

  
  
  void AddTaskObserver(TaskObserver* task_observer);
  void RemoveTaskObserver(TaskObserver* task_observer);

  
  
  bool high_resolution_timers_enabled() {
#if defined(OS_WIN)
    return !high_resolution_timer_expiration_.is_null();
#else
    return true;
#endif
  }

  
  
  static const int kHighResolutionTimerModeLeaseTimeMs = 1000;

  
  void AssertIdle() const;

#if defined(OS_WIN)
  void set_os_modal_loop(bool os_modal_loop) {
    os_modal_loop_ = os_modal_loop;
  }

  bool os_modal_loop() const {
    return os_modal_loop_;
  }
#endif  

  
 protected:
  struct RunState {
    
    int run_depth;

    
    
    bool quit_received;

#if !defined(OS_MACOSX)
    Dispatcher* dispatcher;
#endif
  };

  class AutoRunState : RunState {
   public:
    explicit AutoRunState(MessageLoop* loop);
    ~AutoRunState();
   private:
    MessageLoop* loop_;
    RunState* previous_state_;
  };

  
  struct PendingTask {
    PendingTask(Task* task, bool nestable)
        : task(task), sequence_num(0), nestable(nestable) {
    }

    
    bool operator<(const PendingTask& other) const;

    Task* task;                        
    base::TimeTicks delayed_run_time;  
    int sequence_num;                  
    bool nestable;                     
  };

  class TaskQueue : public std::queue<PendingTask> {
   public:
    void Swap(TaskQueue* queue) {
      c.swap(queue->c);  
    }
  };

  typedef std::priority_queue<PendingTask> DelayedTaskQueue;

#if defined(OS_WIN)
  base::MessagePumpWin* pump_win() {
    return static_cast<base::MessagePumpWin*>(pump_.get());
  }
#elif defined(OS_POSIX)
  base::MessagePumpLibevent* pump_libevent() {
    return static_cast<base::MessagePumpLibevent*>(pump_.get());
  }
#endif

  
  
  
  
  void RunHandler();

#if defined(OS_WIN)
  __declspec(noinline) void RunInternalInSEHFrame();
#endif

  
  
  
  void RunInternal();

  
  bool ProcessNextDelayedNonNestableTask();

  
  void RunTask(Task* task);

  
  
  bool DeferOrRunPendingTask(const PendingTask& pending_task);

  
  void AddToDelayedWorkQueue(const PendingTask& pending_task);

  
  
  
  void ReloadWorkQueue();

  
  
  
  bool DeletePendingTasks();

  
  void PostTask_Helper(const tracked_objects::Location& from_here, Task* task,
                       int64 delay_ms, bool nestable);

  
  
  void StartHistogrammer();

  
  
  
  void HistogramEvent(int event);

  
  virtual bool DoWork();
  virtual bool DoDelayedWork(base::TimeTicks* next_delayed_work_time);
  virtual bool DoIdleWork();

  Type type_;

  
  
  TaskQueue work_queue_;

  
  DelayedTaskQueue delayed_work_queue_;

  
  base::TimeTicks recent_time_;

  
  
  
  TaskQueue deferred_non_nestable_work_queue_;

  scoped_refptr<base::MessagePump> pump_;

  ObserverList<DestructionObserver> destruction_observers_;

  
  
  bool nestable_tasks_allowed_;

  bool exception_restoration_;

  std::string thread_name_;
  
  base::Histogram* message_histogram_;

  
  
  
  
  TaskQueue incoming_queue_;
  
  mutable base::Lock incoming_queue_lock_;

  RunState* state_;

#if defined(OS_WIN)
  base::TimeTicks high_resolution_timer_expiration_;
  
  
  bool os_modal_loop_;
#endif

  
  int next_sequence_num_;

  ObserverList<TaskObserver> task_observers_;

 private:
  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

class BASE_API MessageLoopForUI : public MessageLoop {
 public:
  MessageLoopForUI() : MessageLoop(TYPE_UI) {
  }

  
  static MessageLoopForUI* current() {
    MessageLoop* loop = MessageLoop::current();
#ifdef ANDROID
    DCHECK_EQ(static_cast<int>(MessageLoop::TYPE_UI),
              static_cast<int>(loop->type()));
#else
    DCHECK_EQ(MessageLoop::TYPE_UI, loop->type());
#endif
    return static_cast<MessageLoopForUI*>(loop);
  }

#if defined(OS_WIN)
  void DidProcessMessage(const MSG& message);
#endif  

#if defined(USE_X11)
  
  
  
  
  
  
  
  
  Display* GetDisplay();
#endif  

#if !defined(OS_MACOSX)
  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void Run(Dispatcher* dispatcher);

 protected:
  
  base::MessagePumpForUI* pump_ui() {
    return static_cast<base::MessagePumpForUI*>(pump_.get());
  }
#endif  
};

COMPILE_ASSERT(sizeof(MessageLoop) == sizeof(MessageLoopForUI),
               MessageLoopForUI_should_not_have_extra_member_variables);

class BASE_API MessageLoopForIO : public MessageLoop {
 public:
#if defined(OS_WIN)
  typedef base::MessagePumpForIO::IOHandler IOHandler;
  typedef base::MessagePumpForIO::IOContext IOContext;
  typedef base::MessagePumpForIO::IOObserver IOObserver;
#elif defined(OS_POSIX)
  typedef base::MessagePumpLibevent::Watcher Watcher;
  typedef base::MessagePumpLibevent::FileDescriptorWatcher
      FileDescriptorWatcher;
  typedef base::MessagePumpLibevent::IOObserver IOObserver;

  enum Mode {
    WATCH_READ = base::MessagePumpLibevent::WATCH_READ,
    WATCH_WRITE = base::MessagePumpLibevent::WATCH_WRITE,
    WATCH_READ_WRITE = base::MessagePumpLibevent::WATCH_READ_WRITE
  };

#endif

  MessageLoopForIO() : MessageLoop(TYPE_IO) {
  }

  
  static MessageLoopForIO* current() {
    MessageLoop* loop = MessageLoop::current();
#ifdef ANDROID
    DCHECK_EQ(static_cast<int>(MessageLoop::TYPE_IO),
              static_cast<int>(loop->type()));
#else
    DCHECK_EQ(MessageLoop::TYPE_IO, loop->type());
#endif
    return static_cast<MessageLoopForIO*>(loop);
  }

  void AddIOObserver(IOObserver* io_observer) {
    pump_io()->AddIOObserver(io_observer);
  }

  void RemoveIOObserver(IOObserver* io_observer) {
    pump_io()->RemoveIOObserver(io_observer);
  }

#if defined(OS_WIN)
  
  void RegisterIOHandler(HANDLE file_handle, IOHandler* handler);
  bool WaitForIOCompletion(DWORD timeout, IOHandler* filter);

 protected:
  
  base::MessagePumpForIO* pump_io() {
    return static_cast<base::MessagePumpForIO*>(pump_.get());
  }

#elif defined(OS_POSIX)
  
  bool WatchFileDescriptor(int fd,
                           bool persistent,
                           Mode mode,
                           FileDescriptorWatcher *controller,
                           Watcher *delegate);

 private:
  base::MessagePumpLibevent* pump_io() {
    return static_cast<base::MessagePumpLibevent*>(pump_.get());
  }
#endif  
};

COMPILE_ASSERT(sizeof(MessageLoop) == sizeof(MessageLoopForIO),
               MessageLoopForIO_should_not_have_extra_member_variables);

#endif  
