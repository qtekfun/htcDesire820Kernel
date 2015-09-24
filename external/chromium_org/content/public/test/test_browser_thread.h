// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_BROWSER_THREAD_H_
#define CONTENT_PUBLIC_TEST_TEST_BROWSER_THREAD_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_thread.h"

namespace base {
class MessageLoop;
class Thread;
}

namespace content {

class TestBrowserThreadImpl;

class TestBrowserThread {
 public:
  explicit TestBrowserThread(BrowserThread::ID identifier);
  TestBrowserThread(BrowserThread::ID identifier,
                    base::MessageLoop* message_loop);
  ~TestBrowserThread();

  
  
  
  

  
  bool Start();

  
  bool StartIOThread();

  
  void Stop();

  
  bool IsRunning();

  
  
  base::Thread* DeprecatedGetThreadObject();

 private:
  scoped_ptr<TestBrowserThreadImpl> impl_;

  DISALLOW_COPY_AND_ASSIGN(TestBrowserThread);
};

}  

#endif  
