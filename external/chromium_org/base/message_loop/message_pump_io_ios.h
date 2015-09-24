// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_IO_IOS_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_IO_IOS_H_

#include "base/base_export.h"
#include "base/mac/scoped_cffiledescriptorref.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_pump_mac.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"

namespace base {

class BASE_EXPORT MessagePumpIOSForIO : public MessagePumpNSRunLoop {
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
    friend class MessagePumpIOSForIO;
    friend class MessagePumpIOSForIOTest;

    
    
    void Init(CFFileDescriptorRef fdref,
              CFOptionFlags callback_types,
              CFRunLoopSourceRef fd_source,
              bool is_persistent);

    void set_pump(base::WeakPtr<MessagePumpIOSForIO> pump) { pump_ = pump; }
    const base::WeakPtr<MessagePumpIOSForIO>& pump() const { return pump_; }

    void set_watcher(Watcher* watcher) { watcher_ = watcher; }

    void OnFileCanReadWithoutBlocking(int fd, MessagePumpIOSForIO* pump);
    void OnFileCanWriteWithoutBlocking(int fd, MessagePumpIOSForIO* pump);

    bool is_persistent_;  
    base::mac::ScopedCFFileDescriptorRef fdref_;
    CFOptionFlags callback_types_;
    base::ScopedCFTypeRef<CFRunLoopSourceRef> fd_source_;
    base::WeakPtr<MessagePumpIOSForIO> pump_;
    Watcher* watcher_;

    DISALLOW_COPY_AND_ASSIGN(FileDescriptorWatcher);
  };

  enum Mode {
    WATCH_READ = 1 << 0,
    WATCH_WRITE = 1 << 1,
    WATCH_READ_WRITE = WATCH_READ | WATCH_WRITE
  };

  MessagePumpIOSForIO();
  virtual ~MessagePumpIOSForIO();

  
  
  
  
  
  
  
  
  
  
  
  bool WatchFileDescriptor(int fd,
                           bool persistent,
                           int mode,
                           FileDescriptorWatcher *controller,
                           Watcher *delegate);

  void RemoveRunLoopSource(CFRunLoopSourceRef source);

  void AddIOObserver(IOObserver* obs);
  void RemoveIOObserver(IOObserver* obs);

 private:
  friend class MessagePumpIOSForIOTest;

  void WillProcessIOEvent();
  void DidProcessIOEvent();

  static void HandleFdIOEvent(CFFileDescriptorRef fdref,
                              CFOptionFlags callback_types,
                              void* context);

  ObserverList<IOObserver> io_observers_;
  ThreadChecker watch_file_descriptor_caller_checker_;

  base::WeakPtrFactory<MessagePumpIOSForIO> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpIOSForIO);
};

}  

#endif  
