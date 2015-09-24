// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_HOST_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_HOST_H_

#include "base/environment.h"
#include "base/process/kill.h"
#include "base/process/process_handle.h"
#include "base/process/process_metrics.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "content/common/content_export.h"
#include "content/public/common/process_type.h"
#include "ipc/ipc_sender.h"

class CommandLine;

namespace base {
class FilePath;
}

namespace content {

class BrowserChildProcessHostDelegate;
class ChildProcessHost;
class SandboxedProcessLauncherDelegate;
struct ChildProcessData;

class CONTENT_EXPORT BrowserChildProcessHost : public IPC::Sender {
 public:
  
  
  
  static BrowserChildProcessHost* Create(
      int process_type,
      BrowserChildProcessHostDelegate* delegate);

  virtual ~BrowserChildProcessHost() {}

  
  
  virtual void Launch(
#if defined(OS_WIN)
      SandboxedProcessLauncherDelegate* delegate,
#elif defined(OS_POSIX)
      bool use_zygote,
      const base::EnvironmentMap& environ,
#endif
      CommandLine* cmd_line) = 0;

  virtual const ChildProcessData& GetData() const = 0;

  
  virtual ChildProcessHost* GetHost() const = 0;

  
  
  
  
  
  
  
  virtual base::TerminationStatus GetTerminationStatus(
      bool known_dead, int* exit_code) = 0;

  
  virtual void SetName(const base::string16& name) = 0;

  
  
  
  
  
  virtual void SetHandle(base::ProcessHandle handle) = 0;

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  static base::ProcessMetrics::PortProvider* GetPortProvider();
#endif
};

};  

#endif  
