// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_THREAD_TEST_HELPER_H_
#define BASE_TEST_THREAD_TEST_HELPER_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/waitable_event.h"

namespace base {

class ThreadTestHelper : public RefCountedThreadSafe<ThreadTestHelper> {
 public:
  explicit ThreadTestHelper(MessageLoopProxy* target_thread);

  
  bool Run() WARN_UNUSED_RESULT;

  virtual void RunTest();

 protected:
  friend class RefCountedThreadSafe<ThreadTestHelper>;

  virtual ~ThreadTestHelper();

  
  void set_test_result(bool test_result) { test_result_ = test_result; }

 private:
  void RunInThread();

  bool test_result_;
  scoped_refptr<MessageLoopProxy> target_thread_;
  WaitableEvent done_event_;

  DISALLOW_COPY_AND_ASSIGN(ThreadTestHelper);
};

}  

#endif  
