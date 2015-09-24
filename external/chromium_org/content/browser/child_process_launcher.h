// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_CHILD_PROCESS_LAUNCHER_H_
#define CONTENT_BROWSER_CHILD_PROCESS_LAUNCHER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/process/kill.h"
#include "base/process/launch.h"
#include "content/common/content_export.h"

class CommandLine;

namespace content {
class SandboxedProcessLauncherDelegate;

class CONTENT_EXPORT ChildProcessLauncher {
 public:
  class CONTENT_EXPORT Client {
   public:
    
    
    virtual void OnProcessLaunched() = 0;

   protected:
    virtual ~Client() {}
  };

  
  
  
  
  
  ChildProcessLauncher(
#if defined(OS_WIN)
      SandboxedProcessLauncherDelegate* delegate,
#elif defined(OS_POSIX)
      bool use_zygote,
      const base::EnvironmentMap& environ,
      int ipcfd,
#endif
      CommandLine* cmd_line,
      int child_process_id,
      Client* client);
  ~ChildProcessLauncher();

  
  bool IsStarting();

  
  base::ProcessHandle GetHandle();

  
  
  
  
  
  
  
  
  
  
  
  base::TerminationStatus GetChildTerminationStatus(bool known_dead,
                                                    int* exit_code);

  
  
  void SetProcessBackgrounded(bool background);

  
  
  void SetTerminateChildOnShutdown(bool terminate_on_shutdown);

 private:
  class Context;

  scoped_refptr<Context> context_;

  DISALLOW_COPY_AND_ASSIGN(ChildProcessLauncher);
};

}  

#endif  
