// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_DISK_CACHE_TEST_UTIL_H_
#define NET_DISK_CACHE_DISK_CACHE_TEST_UTIL_H_

#include <string>

#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "base/timer/timer.h"
#include "base/tuple.h"
#include "build/build_config.h"

bool CreateCacheTestFile(const base::FilePath& name);

bool DeleteCache(const base::FilePath& path);

void CacheTestFillBuffer(char* buffer, size_t len, bool no_nulls);

std::string GenerateKey(bool same_length);

bool CheckCacheIntegrity(const base::FilePath& path, bool new_eviction,
                         uint32 mask);


class MessageLoopHelper {
 public:
  MessageLoopHelper();
  ~MessageLoopHelper();

  
  
  
  bool WaitUntilCacheIoFinished(int num_callbacks);

  
  bool callback_reused_error() const { return callback_reused_error_; }
  void set_callback_reused_error(bool error) {
    callback_reused_error_ = error;
  }

  int callbacks_called() const { return callbacks_called_; }
  
  
  
  void CallbackWasCalled() { ++callbacks_called_; }

 private:
  
  void ExpectCallbacks(int num_callbacks) {
    num_callbacks_ = num_callbacks;
    num_iterations_ = last_ = 0;
    completed_ = false;
  }

  
  void TimerExpired();

  base::RepeatingTimer<MessageLoopHelper> timer_;
  int num_callbacks_;
  int num_iterations_;
  int last_;
  bool completed_;

  
  bool callback_reused_error_;
  int callbacks_called_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopHelper);
};


class CallbackTest {
 public:
  
  
  
  
  CallbackTest(MessageLoopHelper* helper, bool reuse);
  ~CallbackTest();

  void Run(int result);

  int last_result() const { return last_result_; }

 private:
  MessageLoopHelper* helper_;
  int reuse_;
  int last_result_;
  DISALLOW_COPY_AND_ASSIGN(CallbackTest);
};

#endif  
