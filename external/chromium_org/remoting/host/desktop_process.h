// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_PROCESS_H_
#define REMOTING_HOST_DESKTOP_PROCESS_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ipc/ipc_listener.h"
#include "remoting/host/desktop_session_agent.h"

namespace IPC {
class ChannelProxy;
}  

namespace remoting {

class AutoThreadTaskRunner;
class DesktopEnvironment;
class DesktopEnvironmentFactory;
class DesktopSessionAgent;

class DesktopProcess : public DesktopSessionAgent::Delegate,
                       public IPC::Listener,
                       public base::SupportsWeakPtr<DesktopProcess> {
 public:
  DesktopProcess(
      scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
      scoped_refptr<AutoThreadTaskRunner> input_task_runner,
      const std::string& daemon_channel_name);
  virtual ~DesktopProcess();

  
  virtual DesktopEnvironmentFactory& desktop_environment_factory() OVERRIDE;
  virtual void OnNetworkProcessDisconnected() OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  void InjectSas();

  
  
  bool Start(scoped_ptr<DesktopEnvironmentFactory> desktop_environment_factory);

 private:
  
  
  
  void OnCrash(const std::string& function_name,
               const std::string& file_name,
               const int& line_number);

  
  scoped_refptr<AutoThreadTaskRunner> caller_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> input_task_runner_;

  
  scoped_ptr<DesktopEnvironmentFactory> desktop_environment_factory_;

  
  
  std::string daemon_channel_name_;

  
  scoped_ptr<IPC::ChannelProxy> daemon_channel_;

  
  
  scoped_refptr<DesktopSessionAgent> desktop_agent_;

  DISALLOW_COPY_AND_ASSIGN(DesktopProcess);
};

}  

#endif  
