// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SESSION_TEST_UTIL_H_
#define NET_SPDY_SPDY_SESSION_TEST_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/message_loop/message_loop.h"
#include "base/pending_task.h"

namespace net {

class SpdySessionTestTaskObserver : public base::MessageLoop::TaskObserver {
 public:
  
  
  
  
  
  
  SpdySessionTestTaskObserver(const std::string& file_name,
                              const std::string& function_name);
  virtual ~SpdySessionTestTaskObserver();

  
  virtual void WillProcessTask(const base::PendingTask& pending_task) OVERRIDE;
  virtual void DidProcessTask(const base::PendingTask& pending_task) OVERRIDE;

  
  uint16 executed_count() const { return executed_count_; }

 private:
  uint16 executed_count_;
  std::string file_name_;
  std::string function_name_;
};

}  

#endif  
