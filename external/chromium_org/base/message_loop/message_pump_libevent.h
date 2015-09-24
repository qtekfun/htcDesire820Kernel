// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_LIBEVENT_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_LIBEVENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_pump.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"

struct event_base;
struct event;

namespace base {

class BASE_EXPORT MessagePumpLibevent : public MessagePump {
 public:
  class IOObserver {
   public:
    IOObserver() {}

    
    
    
    
    virtual void WillProcessIOEvent() = 0;
    virtual void DidProcessIOEvent() = 0;

   protected:
    virtual ~IOObserver() {}
  };

  
  
  class Watcher {
   public:
    // Called from MessageLoop::Run when an FD can be read from/written to
    
    virtual void OnFileCanReadWithoutBlocking(int fd) = 0;
    virtual void OnFileCanWriteWithoutBlocking(int fd) = 0;

   protected:
    virtual ~Watcher() {}
  };

  
  class FileDescriptorWatcher {
   public:
    FileDescriptorWatcher();
    ~FileDescriptorWatcher();  

    
    

    
    
    bool StopWatchingFileDescriptor();

   private:
    friend class MessagePumpLibevent;
    friend class MessagePumpLibeventTest;

    
    
    void Init(event* e);

    
    event* ReleaseEvent();

    void set_pump(MessagePumpLibevent* pump) { pump_ = pump; }
    MessagePumpLibevent* pump() const { return pump_; }

    void set_watcher(Watcher* watcher) { watcher_ = watcher; }

    void OnFileCanReadWithoutBlocking(int fd, MessagePumpLibevent* pump);
    void OnFileCanWriteWithoutBlocking(int fd, MessagePumpLibevent* pump);

    event* event_;
    MessagePumpLibevent* pump_;
    Watcher* watcher_;
    WeakPtrFactory<FileDescriptorWatcher> weak_factory_;

    DISALLOW_COPY_AND_ASSIGN(FileDescriptorWatcher);
  };

  enum Mode {
    WATCH_READ = 1 << 0,
    WATCH_WRITE = 1 << 1,
    WATCH_READ_WRITE = WATCH_READ | WATCH_WRITE
  };

  MessagePumpLibevent();
  virtual ~MessagePumpLibevent();

  
  
  
  
  
  
  
  
  
  
  
  
  bool WatchFileDescriptor(int fd,
                           bool persistent,
                           int mode,
                           FileDescriptorWatcher *controller,
                           Watcher *delegate);

  void AddIOObserver(IOObserver* obs);
  void RemoveIOObserver(IOObserver* obs);

  
  virtual void Run(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;
  virtual void ScheduleWork() OVERRIDE;
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time) OVERRIDE;

 private:
  friend class MessagePumpLibeventTest;

  void WillProcessIOEvent();
  void DidProcessIOEvent();

  
  bool Init();

  // Called by libevent to tell us a registered FD can be read/written to.
  static void OnLibeventNotification(int fd, short flags,
                                     void* context);

  
  
  static void OnWakeup(int socket, short flags, void* context);

  
  bool keep_running_;

  
  bool in_run_;

  
  bool processed_io_events_;

  
  TimeTicks delayed_work_time_;

  
  
  event_base* event_base_;

  
  int wakeup_pipe_in_;
  
  int wakeup_pipe_out_;
  
  event* wakeup_event_;

  ObserverList<IOObserver> io_observers_;
  ThreadChecker watch_file_descriptor_caller_checker_;
  DISALLOW_COPY_AND_ASSIGN(MessagePumpLibevent);
};

}  

#endif  
