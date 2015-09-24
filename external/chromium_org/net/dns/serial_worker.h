// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_SERIAL_WORKER_H_
#define NET_DNS_SERIAL_WORKER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"

namespace base {
class MessageLoopProxy;
}

namespace net {

class NET_EXPORT_PRIVATE SerialWorker
  : NON_EXPORTED_BASE(public base::RefCountedThreadSafe<SerialWorker>) {
 public:
  SerialWorker();

  
  
  virtual void WorkNow();

  
  void Cancel();

  bool IsCancelled() const { return state_ == CANCELLED; }

 protected:
  friend class base::RefCountedThreadSafe<SerialWorker>;
  
  virtual ~SerialWorker();

  
  virtual void DoWork() = 0;

  
  virtual void OnWorkFinished() = 0;

  base::MessageLoopProxy* loop() { return message_loop_.get(); }

 private:
  enum State {
    CANCELLED = -1,
    IDLE = 0,
    WORKING,  
    PENDING,  
    WAITING,  
  };

  
  
  void DoWorkJob();

  
  void OnWorkJobFinished();

  
  void RetryWork();

  
  scoped_refptr<base::MessageLoopProxy> message_loop_;

  State state_;

  DISALLOW_COPY_AND_ASSIGN(SerialWorker);
};

}  

#endif  

