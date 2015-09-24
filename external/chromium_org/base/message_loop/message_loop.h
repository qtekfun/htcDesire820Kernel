// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_
#define BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_

#include <queue>
#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/incoming_task_queue.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/message_loop/message_loop_proxy_impl.h"
#include "base/message_loop/message_pump.h"
#include "base/observer_list.h"
#include "base/pending_task.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "base/tracking_info.h"

#if defined(OS_WIN)
#include "base/message_loop/message_pump_win.h"
#elif defined(OS_IOS)
#include "base/message_loop/message_pump_io_ios.h"
#elif defined(OS_POSIX)
#include "base/message_loop/message_pump_libevent.h"
#if !defined(OS_MACOSX) && !defined(OS_ANDROID)

#if defined(USE_AURA) && defined(USE_X11) && !defined(OS_NACL)
#include "base/message_loop/message_pump_x11.h"
#elif defined(USE_OZONE) && !defined(OS_NACL)
#include "base/message_loop/message_pump_ozone.h"
#else
#define USE_GTK_MESSAGE_PUMP
#include "base/message_loop/message_pump_gtk.h"
#if defined(TOOLKIT_GTK)
#include "base/message_loop/message_pump_x11.h"
#endif
#endif

#endif
#endif

namespace base {

class HistogramBase;
class MessagePumpDispatcher;
class MessagePumpObserver;
class RunLoop;
class ThreadTaskRunnerHandle;
#if defined(OS_ANDROID)
class MessagePumpForUI;
#endif
class WaitableEvent;

class BASE_EXPORT MessageLoop : public MessagePump::Delegate {
 public:

#if defined(USE_GTK_MESSAGE_PUMP)
  typedef MessagePumpGdkObserver Observer;
#elif !defined(OS_MACOSX) && !defined(OS_ANDROID)
  typedef MessagePumpDispatcher Dispatcher;
  typedef MessagePumpObserver Observer;
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum Type {
    TYPE_DEFAULT,
    TYPE_UI,
    TYPE_CUSTOM,
#if defined(TOOLKIT_GTK)
    TYPE_GPU,
#endif
    TYPE_IO,
#if defined(OS_ANDROID)
    TYPE_JAVA,
#endif 
  };

  
  
  explicit MessageLoop(Type type = TYPE_DEFAULT);
  
  
  explicit MessageLoop(scoped_ptr<base::MessagePump> pump);
  virtual ~MessageLoop();

  
  static MessageLoop* current();

  static void EnableHistogrammer(bool enable_histogrammer);

  typedef MessagePump* (MessagePumpFactory)();
  
  
  
  static bool InitMessagePumpForUIFactory(MessagePumpFactory* factory);

  
  
  
  
  static MessagePump* CreateMessagePumpForType(Type type);

  
  
  
  
  
  
  
  
  class BASE_EXPORT DestructionObserver {
   public:
    virtual void WillDestroyCurrentMessageLoop() = 0;

   protected:
    virtual ~DestructionObserver();
  };

  
  
  void AddDestructionObserver(DestructionObserver* destruction_observer);

  
  
  void RemoveDestructionObserver(DestructionObserver* destruction_observer);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PostTask(const tracked_objects::Location& from_here,
                const Closure& task);

  bool TryPostTask(const tracked_objects::Location& from_here,
                   const Closure& task);

  void PostDelayedTask(const tracked_objects::Location& from_here,
                       const Closure& task,
                       TimeDelta delay);

  void PostNonNestableTask(const tracked_objects::Location& from_here,
                           const Closure& task);

  void PostNonNestableDelayedTask(const tracked_objects::Location& from_here,
                                  const Closure& task,
                                  TimeDelta delay);

  
  
  
  
  
  
  
  
  
  template <class T>
  void DeleteSoon(const tracked_objects::Location& from_here, const T* object) {
    base::subtle::DeleteHelperInternal<T, void>::DeleteViaSequencedTaskRunner(
        this, from_here, object);
  }

  
  
  
  
  
  
  
  
  
  
  template <class T>
  void ReleaseSoon(const tracked_objects::Location& from_here,
                   const T* object) {
    base::subtle::ReleaseHelperInternal<T, void>::ReleaseViaSequencedTaskRunner(
        this, from_here, object);
  }

  
  
  void Run();

  
  
  
  void RunUntilIdle();

  
  void Quit() { QuitWhenIdle(); }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void QuitWhenIdle();

  
  
  
  
  void QuitNow();

  
  static Closure QuitClosure() { return QuitWhenIdleClosure(); }

  
  
  
  static Closure QuitWhenIdleClosure();

  
  
  virtual bool IsType(Type type) const;

  
  Type type() const { return type_; }

  
  void set_thread_name(const std::string& thread_name) {
    DCHECK(thread_name_.empty()) << "Should not rename this thread!";
    thread_name_ = thread_name;
  }
  const std::string& thread_name() const { return thread_name_; }

  
  scoped_refptr<MessageLoopProxy> message_loop_proxy() {
    return message_loop_proxy_;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
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

  
  
  
  
  class BASE_EXPORT TaskObserver {
   public:
    TaskObserver();

    
    virtual void WillProcessTask(const PendingTask& pending_task) = 0;

    
    virtual void DidProcessTask(const PendingTask& pending_task) = 0;

   protected:
    virtual ~TaskObserver();
  };

  
  
  void AddTaskObserver(TaskObserver* task_observer);
  void RemoveTaskObserver(TaskObserver* task_observer);

  
  
  static const int kHighResolutionTimerModeLeaseTimeMs = 1000;

#if defined(OS_WIN)
  void set_os_modal_loop(bool os_modal_loop) {
    os_modal_loop_ = os_modal_loop;
  }

  bool os_modal_loop() const {
    return os_modal_loop_;
  }
#endif  

  
  bool is_running() const;

  
  
  bool IsHighResolutionTimerEnabledForTesting();

  
  bool IsIdleForTesting();

  
  
  void LockWaitUnLockForTesting(WaitableEvent* caller_wait,
                                WaitableEvent* caller_signal);

  
 protected:

#if defined(OS_WIN)
  MessagePumpWin* pump_win() {
    return static_cast<MessagePumpWin*>(pump_.get());
  }
#elif defined(OS_POSIX) && !defined(OS_IOS)
  MessagePumpLibevent* pump_libevent() {
    return static_cast<MessagePumpLibevent*>(pump_.get());
  }
#if defined(TOOLKIT_GTK)
  friend class MessagePumpX11;
  MessagePumpX11* pump_gpu() {
    DCHECK_EQ(TYPE_GPU, type());
    return static_cast<MessagePumpX11*>(pump_.get());
  }
#endif
#endif

  scoped_ptr<MessagePump> pump_;

 private:
  friend class internal::IncomingTaskQueue;
  friend class RunLoop;

  
  void Init();

  
  
  
  
  void RunHandler();

#if defined(OS_WIN)
  __declspec(noinline) void RunInternalInSEHFrame();
#endif

  
  
  
  void RunInternal();

  
  bool ProcessNextDelayedNonNestableTask();

  
  void RunTask(const PendingTask& pending_task);

  
  
  bool DeferOrRunPendingTask(const PendingTask& pending_task);

  
  void AddToDelayedWorkQueue(const PendingTask& pending_task);

  
  
  
  bool DeletePendingTasks();

  
  
  
  uint64 GetTaskTraceID(const PendingTask& task);

  
  
  void ReloadWorkQueue();

  
  
  void ScheduleWork(bool was_empty);

  
  
  void StartHistogrammer();

  
  
  
  void HistogramEvent(int event);

  
  virtual bool DoWork() OVERRIDE;
  virtual bool DoDelayedWork(TimeTicks* next_delayed_work_time) OVERRIDE;
  virtual bool DoIdleWork() OVERRIDE;
  virtual void GetQueueingInformation(size_t* queue_size,
                                      TimeDelta* queueing_delay) OVERRIDE;

  const Type type_;

  
  
  TaskQueue work_queue_;

  
  DelayedTaskQueue delayed_work_queue_;

  
  TimeTicks recent_time_;

  
  
  
  TaskQueue deferred_non_nestable_work_queue_;

  ObserverList<DestructionObserver> destruction_observers_;

  bool exception_restoration_;

  
  
  bool nestable_tasks_allowed_;

#if defined(OS_WIN)
  
  
  bool os_modal_loop_;
#endif

  std::string thread_name_;
  
  HistogramBase* message_histogram_;

  RunLoop* run_loop_;

  ObserverList<TaskObserver> task_observers_;

  scoped_refptr<internal::IncomingTaskQueue> incoming_task_queue_;

  
  scoped_refptr<internal::MessageLoopProxyImpl> message_loop_proxy_;
  scoped_ptr<ThreadTaskRunnerHandle> thread_task_runner_handle_;

  template <class T, class R> friend class base::subtle::DeleteHelperInternal;
  template <class T, class R> friend class base::subtle::ReleaseHelperInternal;

  void DeleteSoonInternal(const tracked_objects::Location& from_here,
                          void(*deleter)(const void*),
                          const void* object);
  void ReleaseSoonInternal(const tracked_objects::Location& from_here,
                           void(*releaser)(const void*),
                           const void* object);

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

class BASE_EXPORT MessageLoopForUI : public MessageLoop {
 public:
#if defined(OS_WIN)
  typedef MessagePumpForUI::MessageFilter MessageFilter;
#endif

  MessageLoopForUI() : MessageLoop(TYPE_UI) {
  }

  
  static MessageLoopForUI* current() {
    MessageLoop* loop = MessageLoop::current();
    DCHECK(loop);
    DCHECK_EQ(MessageLoop::TYPE_UI, loop->type());
    return static_cast<MessageLoopForUI*>(loop);
  }

#if defined(OS_IOS)
  
  
  
  void Attach();
#endif

#if defined(OS_ANDROID)
  
  
  
  void Start();
#elif !defined(OS_MACOSX)

  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

#if defined(OS_WIN)
  
  void SetMessageFilter(scoped_ptr<MessageFilter> message_filter) {
    pump_ui()->SetMessageFilter(message_filter.Pass());
  }
#endif

 protected:
#if defined(USE_X11)
  friend class MessagePumpX11;
#endif
#if defined(USE_OZONE) && !defined(OS_NACL)
  friend class MessagePumpOzone;
#endif

  
  MessagePumpForUI* pump_ui() {
    return static_cast<MessagePumpForUI*>(pump_.get());
  }
#endif  
};

COMPILE_ASSERT(sizeof(MessageLoop) == sizeof(MessageLoopForUI),
               MessageLoopForUI_should_not_have_extra_member_variables);

class BASE_EXPORT MessageLoopForIO : public MessageLoop {
 public:
#if defined(OS_WIN)
  typedef MessagePumpForIO::IOHandler IOHandler;
  typedef MessagePumpForIO::IOContext IOContext;
  typedef MessagePumpForIO::IOObserver IOObserver;
#elif defined(OS_IOS)
  typedef MessagePumpIOSForIO::Watcher Watcher;
  typedef MessagePumpIOSForIO::FileDescriptorWatcher
      FileDescriptorWatcher;
  typedef MessagePumpIOSForIO::IOObserver IOObserver;

  enum Mode {
    WATCH_READ = MessagePumpIOSForIO::WATCH_READ,
    WATCH_WRITE = MessagePumpIOSForIO::WATCH_WRITE,
    WATCH_READ_WRITE = MessagePumpIOSForIO::WATCH_READ_WRITE
  };
#elif defined(OS_POSIX)
  typedef MessagePumpLibevent::Watcher Watcher;
  typedef MessagePumpLibevent::FileDescriptorWatcher
      FileDescriptorWatcher;
  typedef MessagePumpLibevent::IOObserver IOObserver;

  enum Mode {
    WATCH_READ = MessagePumpLibevent::WATCH_READ,
    WATCH_WRITE = MessagePumpLibevent::WATCH_WRITE,
    WATCH_READ_WRITE = MessagePumpLibevent::WATCH_READ_WRITE
  };

#endif

  MessageLoopForIO() : MessageLoop(TYPE_IO) {
  }

  
  static MessageLoopForIO* current() {
    MessageLoop* loop = MessageLoop::current();
    DCHECK_EQ(MessageLoop::TYPE_IO, loop->type());
    return static_cast<MessageLoopForIO*>(loop);
  }

  void AddIOObserver(IOObserver* io_observer) {
    pump_io()->AddIOObserver(io_observer);
  }

  void RemoveIOObserver(IOObserver* io_observer) {
    pump_io()->RemoveIOObserver(io_observer);
  }

#if defined(OS_WIN)
  
  void RegisterIOHandler(HANDLE file, IOHandler* handler);
  bool RegisterJobObject(HANDLE job, IOHandler* handler);
  bool WaitForIOCompletion(DWORD timeout, IOHandler* filter);

 protected:
  
  MessagePumpForIO* pump_io() {
    return static_cast<MessagePumpForIO*>(pump_.get());
  }

#elif defined(OS_IOS)
  
  bool WatchFileDescriptor(int fd,
                           bool persistent,
                           Mode mode,
                           FileDescriptorWatcher *controller,
                           Watcher *delegate);

 private:
  MessagePumpIOSForIO* pump_io() {
    return static_cast<MessagePumpIOSForIO*>(pump_.get());
  }

#elif defined(OS_POSIX)
  
  bool WatchFileDescriptor(int fd,
                           bool persistent,
                           Mode mode,
                           FileDescriptorWatcher* controller,
                           Watcher* delegate);

 private:
  MessagePumpLibevent* pump_io() {
    return static_cast<MessagePumpLibevent*>(pump_.get());
  }
#endif  
};

COMPILE_ASSERT(sizeof(MessageLoop) == sizeof(MessageLoopForIO),
               MessageLoopForIO_should_not_have_extra_member_variables);

}  

#endif  
