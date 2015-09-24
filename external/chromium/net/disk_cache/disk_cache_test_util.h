// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_DISK_CACHE_TEST_UTIL_H_
#define NET_DISK_CACHE_DISK_CACHE_TEST_UTIL_H_
#pragma once

#include <string>

#include "base/callback.h"
#include "base/file_path.h"
#include "base/message_loop.h"
#include "base/timer.h"
#include "build/build_config.h"

bool CreateCacheTestFile(const FilePath& name);

bool DeleteCache(const FilePath& path);

bool CopyTestCache(const std::string& name);

FilePath GetCacheFilePath();

void CacheTestFillBuffer(char* buffer, size_t len, bool no_nulls);

std::string GenerateKey(bool same_length);

bool CheckCacheIntegrity(const FilePath& path, bool new_eviction);

class ScopedTestCache {
 public:
  ScopedTestCache();
  
  explicit ScopedTestCache(const std::string& name);
  ~ScopedTestCache();

  FilePath path() const { return path_; }

 private:
  const FilePath path_;  

  DISALLOW_COPY_AND_ASSIGN(ScopedTestCache);
};


class CallbackTest : public CallbackRunner< Tuple1<int> >  {
 public:
  explicit CallbackTest(bool reuse);
  virtual ~CallbackTest();

  int result() const { return result_; }
  virtual void RunWithParams(const Tuple1<int>& params);

 private:
  int result_;
  int reuse_;
  DISALLOW_COPY_AND_ASSIGN(CallbackTest);
};


class MessageLoopHelper {
 public:
  MessageLoopHelper();
  ~MessageLoopHelper();

  
  
  bool WaitUntilCacheIoFinished(int num_callbacks);

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

  DISALLOW_COPY_AND_ASSIGN(MessageLoopHelper);
};

#endif  
