// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_THREAD_IMPL_H_
#define CONTENT_BROWSER_BROWSER_THREAD_IMPL_H_

#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_thread.h"

namespace content {

class CONTENT_EXPORT BrowserThreadImpl : public BrowserThread,
                                         public base::Thread {
 public:
  
  
  explicit BrowserThreadImpl(BrowserThread::ID identifier);

  
  
  
  BrowserThreadImpl(BrowserThread::ID identifier,
                    base::MessageLoop* message_loop);
  virtual ~BrowserThreadImpl();

  static void ShutdownThreadPool();

 protected:
  virtual void Init() OVERRIDE;
  virtual void Run(base::MessageLoop* message_loop) OVERRIDE;
  virtual void CleanUp() OVERRIDE;

 private:
  
  
  
  friend class BrowserThread;

  
  
  void UIThreadRun(base::MessageLoop* message_loop);
  void DBThreadRun(base::MessageLoop* message_loop);
  void FileThreadRun(base::MessageLoop* message_loop);
  void FileUserBlockingThreadRun(base::MessageLoop* message_loop);
  void ProcessLauncherThreadRun(base::MessageLoop* message_loop);
  void CacheThreadRun(base::MessageLoop* message_loop);
  void IOThreadRun(base::MessageLoop* message_loop);

  static bool PostTaskHelper(
      BrowserThread::ID identifier,
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay,
      bool nestable);

  
  void Initialize();

  
  friend class ContentTestSuiteBaseListener;
  friend class TestBrowserThreadBundle;
  static void FlushThreadPoolHelper();

  
  
  ID identifier_;
};

}  

#endif  
