// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_REMOTING_WAITER_H_
#define CHROME_TEST_REMOTING_WAITER_H_

#include "base/timer/timer.h"

namespace content {
class MessageLoopRunner;
}

namespace remoting {

class TimeoutWaiter {
 public:
  explicit TimeoutWaiter(base::TimeDelta timeout);
  virtual ~TimeoutWaiter();

  
  
  virtual bool Wait();

 protected:
  virtual void CancelWait();

 private:
  
  void CancelWaitCallback();

  base::OneShotTimer<TimeoutWaiter> timeout_timer_;
  base::TimeDelta timeout_;
  scoped_refptr<content::MessageLoopRunner> message_loop_runner_;

  DISALLOW_COPY_AND_ASSIGN(TimeoutWaiter);
};

class ConditionalTimeoutWaiter : public TimeoutWaiter {
 public:
  typedef base::Callback<bool(void)> Predicate;

  ConditionalTimeoutWaiter(base::TimeDelta timeout,
                           base::TimeDelta interval,
                           const Predicate& callback);
  virtual ~ConditionalTimeoutWaiter();

  
  virtual bool Wait() OVERRIDE;

 protected:
  virtual void CancelWait() OVERRIDE;

 private:
  
  void CancelWaitCallback();

  base::TimeDelta interval_;
  Predicate callback_;
  base::RepeatingTimer<ConditionalTimeoutWaiter> condition_timer_;
  bool success_;

  DISALLOW_COPY_AND_ASSIGN(ConditionalTimeoutWaiter);
};

}  

#endif  
