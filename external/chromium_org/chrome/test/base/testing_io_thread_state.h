// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_TESTING_IO_THREAD_STATE_H_
#define CHROME_TEST_BASE_TESTING_IO_THREAD_STATE_H_

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"

class IOThread;

namespace chrome {

class TestingIOThreadState {
 public:
  TestingIOThreadState();
  ~TestingIOThreadState();
  IOThread* io_thread_state() { return io_thread_state_.get(); }

 private:
  void Initialize(const base::Closure& done);
  void Shutdown(const base::Closure& done);

  scoped_ptr<IOThread> io_thread_state_;

  DISALLOW_COPY_AND_ASSIGN(TestingIOThreadState);
};

}  

#endif  
