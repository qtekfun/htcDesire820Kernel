// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_WAITER_TEST_UTILS_H_
#define MOJO_SYSTEM_WAITER_TEST_UTILS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/threading/simple_thread.h"
#include "mojo/public/system/core.h"
#include "mojo/system/dispatcher.h"
#include "mojo/system/waiter.h"

namespace mojo {
namespace system {
namespace test {

class SimpleWaiterThread : public base::SimpleThread {
 public:
  
  
  explicit SimpleWaiterThread(MojoResult* result);
  virtual ~SimpleWaiterThread();  

  Waiter* waiter() { return &waiter_; }

 private:
  virtual void Run() OVERRIDE;

  MojoResult* const result_;
  Waiter waiter_;

  DISALLOW_COPY_AND_ASSIGN(SimpleWaiterThread);
};

class WaiterThread : public base::SimpleThread {
 public:
  
  WaiterThread(scoped_refptr<Dispatcher> dispatcher,
               MojoWaitFlags wait_flags,
               MojoDeadline deadline,
               MojoResult success_result,
               bool* did_wait_out,
               MojoResult* result_out);
  virtual ~WaiterThread();

 private:
  virtual void Run() OVERRIDE;

  const scoped_refptr<Dispatcher> dispatcher_;
  const MojoWaitFlags wait_flags_;
  const MojoDeadline deadline_;
  const MojoResult success_result_;
  bool* const did_wait_out_;
  MojoResult* const result_out_;

  Waiter waiter_;

  DISALLOW_COPY_AND_ASSIGN(WaiterThread);
};

}  
}  
}  

#endif  
