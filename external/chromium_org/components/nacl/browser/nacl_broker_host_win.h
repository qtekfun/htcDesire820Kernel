// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_BROKER_HOST_WIN_H_
#define COMPONENTS_NACL_BROWSER_NACL_BROKER_HOST_WIN_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "content/public/browser/browser_child_process_host_delegate.h"

namespace content {
class BrowserChildProcessHost;
}

namespace nacl {

class NaClBrokerHost : public content::BrowserChildProcessHostDelegate {
 public:
  NaClBrokerHost();
  ~NaClBrokerHost();

  
  
  bool Init();

  
  
  bool LaunchLoader(const std::string& loader_channel_id);

  bool LaunchDebugExceptionHandler(int32 pid,
                                   base::ProcessHandle process_handle,
                                   const std::string& startup_info);

  
  void StopBroker();

  
  
  
  bool IsTerminating() { return is_terminating_; }

 private:
  
  void OnLoaderLaunched(const std::string& loader_channel_id,
                        base::ProcessHandle handle);
  
  void OnDebugExceptionHandlerLaunched(int32 pid, bool success);

  
  virtual bool OnMessageReceived(const IPC::Message& msg);

  scoped_ptr<content::BrowserChildProcessHost> process_;
  bool is_terminating_;

  DISALLOW_COPY_AND_ASSIGN(NaClBrokerHost);
};

}  

#endif  
