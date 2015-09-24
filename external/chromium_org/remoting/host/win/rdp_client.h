// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_RDP_CLIENT_H_
#define REMOTING_HOST_WIN_RDP_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace webrtc {
class DesktopSize;
}  

namespace remoting {

class RdpClient : public base::NonThreadSafe {
 public:
  class EventHandler {
   public:
    virtual ~EventHandler() {}

    
    
    virtual void OnRdpConnected() = 0;

    
    virtual void OnRdpClosed() = 0;
  };

  RdpClient(
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
      const webrtc::DesktopSize& screen_size,
      const std::string& terminal_id,
      EventHandler* event_handler);
  virtual ~RdpClient();

  
  void InjectSas();

 private:
  
  class Core;
  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(RdpClient);
};

}  

#endif  
