// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_LOCAL_INPUT_MONITOR_H_
#define REMOTING_HOST_LOCAL_INPUT_MONITOR_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class ClientSessionControl;

class LocalInputMonitor {
 public:
  virtual ~LocalInputMonitor() {}

  
  
  static scoped_ptr<LocalInputMonitor> Create(
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
      base::WeakPtr<ClientSessionControl> client_session_control);

 protected:
  LocalInputMonitor() {}
};

}  

#endif  
