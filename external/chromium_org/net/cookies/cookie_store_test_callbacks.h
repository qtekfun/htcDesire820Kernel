// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_COOKIES_COOKIE_STORE_TEST_CALLBACKS_H_
#define NET_COOKIES_COOKIE_STORE_TEST_CALLBACKS_H_

#include <string>
#include <vector>

#include "net/cookies/cookie_store.h"

namespace base {
class MessageLoop;
class Thread;
}

namespace net {

class CookieCallback {
 public:
  
  bool did_run() { return did_run_; }

 protected:
  
  
  explicit CookieCallback(base::Thread* run_in_thread);

  
  
  CookieCallback();

  
  
  void CallbackEpilogue();

 private:
  bool did_run_;
  base::Thread* run_in_thread_;
  base::MessageLoop* run_in_loop_;
  base::MessageLoop* parent_loop_;
  base::MessageLoop* loop_to_quit_;
};


class BoolResultCookieCallback : public CookieCallback {
 public:
  BoolResultCookieCallback();
  explicit BoolResultCookieCallback(base::Thread* run_in_thread);

  void Run(bool result) {
    result_ = result;
    CallbackEpilogue();
  }

  bool result() { return result_; }

 private:
  bool result_;
};

class StringResultCookieCallback : public CookieCallback {
 public:
  StringResultCookieCallback();
  explicit StringResultCookieCallback(base::Thread* run_in_thread);

  void Run(const std::string& result) {
    result_ = result;
    CallbackEpilogue();
  }

  const std::string& result() { return result_; }

 private:
  std::string result_;
};

class IntResultCookieCallback : public CookieCallback {
 public:
  IntResultCookieCallback();
  explicit IntResultCookieCallback(base::Thread* run_in_thread);

  void Run(int result) {
    result_ = result;
    CallbackEpilogue();
  }

  int result() { return result_; }

 private:
  int result_;
};

class NoResultCookieCallback : public CookieCallback {
 public:
  NoResultCookieCallback();
  explicit NoResultCookieCallback(base::Thread* run_in_thread);

  void Run() {
    CallbackEpilogue();
  }
};

}  

#endif  