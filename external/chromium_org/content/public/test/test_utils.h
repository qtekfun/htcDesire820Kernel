// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_UTILS_H_
#define CONTENT_PUBLIC_TEST_TEST_UTILS_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/run_loop.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_source.h"

namespace base {
class Value;
}


namespace content {

class RenderViewHost;

void RunMessageLoop();

void RunThisRunLoop(base::RunLoop* run_loop);

void RunAllPendingInMessageLoop();

void RunAllPendingInMessageLoop(BrowserThread::ID thread_id);

base::Closure GetQuitTaskForRunLoop(base::RunLoop* run_loop);

scoped_ptr<base::Value> ExecuteScriptAndGetValue(
    RenderViewHost* render_view_host,
    const std::string& script);

class MessageLoopRunner : public base::RefCounted<MessageLoopRunner> {
 public:
  MessageLoopRunner();

  
  
  void Run();

  
  void Quit();

  
  
  
  
  
  base::Closure QuitClosure();

 private:
  friend class base::RefCounted<MessageLoopRunner>;
  ~MessageLoopRunner();

  
  bool loop_running_;

  
  bool quit_closure_called_;

  base::RunLoop run_loop_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopRunner);
};

class WindowedNotificationObserver : public NotificationObserver {
 public:
  
  
  
  
  typedef base::Callback<bool(const NotificationSource&,
                              const NotificationDetails&)>
      ConditionTestCallback;
  typedef base::Callback<bool(void)>
      ConditionTestCallbackWithoutSourceAndDetails;

  
  
  
  
  WindowedNotificationObserver(int notification_type,
                               const NotificationSource& source);

  
  
  
  WindowedNotificationObserver(int notification_type,
                               const ConditionTestCallback& callback);
  WindowedNotificationObserver(
      int notification_type,
      const ConditionTestCallbackWithoutSourceAndDetails& callback);

  virtual ~WindowedNotificationObserver();

  
  
  
  void AddNotificationType(int notification_type,
                           const NotificationSource& source);

  
  
  
  void Wait();

  
  
  const NotificationSource& source() const {
    return source_;
  }

  const NotificationDetails& details() const {
    return details_;
  }

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

 private:
  bool seen_;
  bool running_;
  NotificationRegistrar registrar_;

  ConditionTestCallback callback_;

  NotificationSource source_;
  NotificationDetails details_;
  scoped_refptr<MessageLoopRunner> message_loop_runner_;

  DISALLOW_COPY_AND_ASSIGN(WindowedNotificationObserver);
};

}  

#endif  
