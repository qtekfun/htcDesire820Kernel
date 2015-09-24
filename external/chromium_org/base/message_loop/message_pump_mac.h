// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_MAC_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_MAC_H_

#include "base/message_loop/message_pump.h"

#include "base/basictypes.h"

#include <CoreFoundation/CoreFoundation.h>

#include "base/memory/weak_ptr.h"

#if !defined(__OBJC__)
class NSAutoreleasePool;
#else  
#if defined(OS_IOS)
#import <Foundation/Foundation.h>
#else
#import <AppKit/AppKit.h>

@protocol CrAppProtocol
- (BOOL)isHandlingSendEvent;
@end
#endif  
#endif  

namespace base {

class MessagePumpInstrumentation;
class RunLoop;
class TimeTicks;

class MessagePumpCFRunLoopBase : public MessagePump {
  
  friend class MessagePumpScopedAutoreleasePool;
 public:
  MessagePumpCFRunLoopBase();
  virtual ~MessagePumpCFRunLoopBase();

  
  
  
  
  virtual void Run(Delegate* delegate) OVERRIDE;
  virtual void DoRun(Delegate* delegate) = 0;

  virtual void ScheduleWork() OVERRIDE;
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time) OVERRIDE;

 protected:
  
  CFRunLoopRef run_loop() const { return run_loop_; }
  int nesting_level() const { return nesting_level_; }
  int run_nesting_level() const { return run_nesting_level_; }

  
  
  void SetDelegate(Delegate* delegate);

  
  
  
  
  virtual NSAutoreleasePool* CreateAutoreleasePool();

  
  
  void EnableInstrumentation();
  WeakPtr<MessagePumpInstrumentation> instrumentation_;

 private:
  
  
  
  static void RunDelayedWorkTimer(CFRunLoopTimerRef timer, void* info);

  
  
  
  
  static void RunWorkSource(void* info);
  bool RunWork();

  
  
  
  
  
  static void RunIdleWorkSource(void* info);
  bool RunIdleWork();

  
  
  
  
  
  
  static void RunNestingDeferredWorkSource(void* info);
  bool RunNestingDeferredWork();

  
  
  
  
  
  void MaybeScheduleNestingDeferredWork();

  
  
  static void StartOrEndWaitObserver(CFRunLoopObserverRef observer,
                                     CFRunLoopActivity activity, void* info);

  
  
  static void PreSourceObserver(CFRunLoopObserverRef observer,
                                CFRunLoopActivity activity, void* info);

  
  
  
  static void EnterExitObserver(CFRunLoopObserverRef observer,
                                CFRunLoopActivity activity, void* info);

  
  
  
  virtual void EnterExitRunLoop(CFRunLoopActivity activity);

  
  CFRunLoopRef run_loop_;

  
  
  CFRunLoopTimerRef delayed_work_timer_;
  CFRunLoopSourceRef work_source_;
  CFRunLoopSourceRef idle_work_source_;
  CFRunLoopSourceRef nesting_deferred_work_source_;
  CFRunLoopObserverRef pre_wait_observer_;
  CFRunLoopObserverRef pre_source_observer_;
  CFRunLoopObserverRef enter_exit_observer_;

  
  Delegate* delegate_;

  
  
  
  
  CFAbsoluteTime delayed_work_fire_time_;

  
  
  
  int nesting_level_;

  
  
  int run_nesting_level_;

  
  
  int deepest_nesting_level_;

  
  
  
  
  
  bool delegateless_work_;
  bool delegateless_idle_work_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpCFRunLoopBase);
};

class MessagePumpCFRunLoop : public MessagePumpCFRunLoopBase {
 public:
  MessagePumpCFRunLoop();
  virtual ~MessagePumpCFRunLoop();

  virtual void DoRun(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;

 private:
  virtual void EnterExitRunLoop(CFRunLoopActivity activity) OVERRIDE;

  
  
  
  bool quit_pending_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpCFRunLoop);
};

class MessagePumpNSRunLoop : public MessagePumpCFRunLoopBase {
 public:
  BASE_EXPORT MessagePumpNSRunLoop();
  virtual ~MessagePumpNSRunLoop();

  virtual void DoRun(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;

 private:
  
  
  
  CFRunLoopSourceRef quit_source_;

  
  bool keep_running_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpNSRunLoop);
};

#if defined(OS_IOS)
class MessagePumpUIApplication : public MessagePumpCFRunLoopBase {
 public:
  MessagePumpUIApplication();
  virtual ~MessagePumpUIApplication();
  virtual void DoRun(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;

  
  
  virtual void Attach(Delegate* delegate);

 private:
  RunLoop* run_loop_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpUIApplication);
};

#else

class MessagePumpNSApplication : public MessagePumpCFRunLoopBase {
 public:
  MessagePumpNSApplication();
  virtual ~MessagePumpNSApplication();

  virtual void DoRun(Delegate* delegate) OVERRIDE;
  virtual void Quit() OVERRIDE;

 private:
  
  bool keep_running_;

  
  
  
  
  bool running_own_loop_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpNSApplication);
};

class MessagePumpCrApplication : public MessagePumpNSApplication {
 public:
  MessagePumpCrApplication();
  virtual ~MessagePumpCrApplication();

 protected:
  
  
  virtual NSAutoreleasePool* CreateAutoreleasePool() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(MessagePumpCrApplication);
};
#endif  

class MessagePumpMac {
 public:
  
  
  
  
  
  
  
  
  static MessagePump* Create();

#if !defined(OS_IOS)
  
  
  
  
  
  BASE_EXPORT static bool UsingCrApp();

  
  
  BASE_EXPORT static bool IsHandlingSendEvent();
#endif  

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(MessagePumpMac);
};

}  

#endif  
