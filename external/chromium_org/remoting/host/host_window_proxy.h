// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_WINDOW_PROXY_H_
#define REMOTING_HOST_HOST_WINDOW_PROXY_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/host/host_window.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace remoting {

class HostWindowProxy : public HostWindow {
 public:
  HostWindowProxy(
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
      scoped_ptr<HostWindow> host_window);
  virtual ~HostWindowProxy();

  
  virtual void Start(
      const base::WeakPtr<ClientSessionControl>& client_session_control)
      OVERRIDE;

 private:
  
  class Core;
  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(HostWindowProxy);
};

}  

#endif  
