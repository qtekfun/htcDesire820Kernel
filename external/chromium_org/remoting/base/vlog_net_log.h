// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_VLOG_NET_LOG_H_
#define REMOTING_BASE_VLOG_NET_LOG_H_

#include "base/memory/scoped_handle.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_log.h"

namespace remoting {

class VlogNetLog : public net::NetLog {
 public:
  VlogNetLog();
  virtual ~VlogNetLog();

 private:
  class Observer;
  scoped_ptr<Observer> observer_;

  DISALLOW_COPY_AND_ASSIGN(VlogNetLog);
};

}  

#endif  
