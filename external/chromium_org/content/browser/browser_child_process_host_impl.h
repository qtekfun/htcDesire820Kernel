// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_CHILD_PROCESS_HOST_IMPL_H_
#define CONTENT_BROWSER_BROWSER_CHILD_PROCESS_HOST_IMPL_H_

#include <list>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "base/synchronization/waitable_event_watcher.h"
#include "content/browser/child_process_launcher.h"
#include "content/browser/power_monitor_message_broadcaster.h"
#include "content/public/browser/browser_child_process_host.h"
#include "content/public/browser/child_process_data.h"
#include "content/public/common/child_process_host_delegate.h"

namespace content {

class BrowserChildProcessHostIterator;
class BrowserChildProcessObserver;
class BrowserMessageFilter;

class CONTENT_EXPORT BrowserChildProcessHostImpl
    : public BrowserChildProcessHost,
      public NON_EXPORTED_BASE(ChildProcessHostDelegate),
      public ChildProcessLauncher::Client {
 public:
  BrowserChildProcessHostImpl(
      int process_type,
      BrowserChildProcessHostDelegate* delegate);
  virtual ~BrowserChildProcessHostImpl();

  
  
  static void TerminateAll();

  
  virtual bool Send(IPC::Message* message) OVERRIDE;
  virtual void Launch(
#if defined(OS_WIN)
      SandboxedProcessLauncherDelegate* delegate,
#elif defined(OS_POSIX)
      bool use_zygote,
      const base::EnvironmentMap& environ,
#endif
      CommandLine* cmd_line) OVERRIDE;
  virtual const ChildProcessData& GetData() const OVERRIDE;
  virtual ChildProcessHost* GetHost() const OVERRIDE;
  virtual base::TerminationStatus GetTerminationStatus(
      bool known_dead, int* exit_code) OVERRIDE;
  virtual void SetName(const base::string16& name) OVERRIDE;
  virtual void SetHandle(base::ProcessHandle handle) OVERRIDE;

  
  
  base::ProcessHandle GetHandle() const;

  
  void ForceShutdown();

  
  void SetBackgrounded(bool backgrounded);

  
  
  void SetTerminateChildOnShutdown(bool terminate_on_shutdown);

  
  void AddFilter(BrowserMessageFilter* filter);

  
  static void NotifyProcessInstanceCreated(const ChildProcessData& data);

  BrowserChildProcessHostDelegate* delegate() const { return delegate_; }

  typedef std::list<BrowserChildProcessHostImpl*> BrowserChildProcessList;
 private:
  friend class BrowserChildProcessHostIterator;
  friend class BrowserChildProcessObserver;

  static BrowserChildProcessList* GetIterator();

  static void AddObserver(BrowserChildProcessObserver* observer);
  static void RemoveObserver(BrowserChildProcessObserver* observer);

  
  virtual bool CanShutdown() OVERRIDE;
  virtual void OnChildDisconnected() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual void OnProcessLaunched() OVERRIDE;

#if defined(OS_WIN)
  void DeleteProcessWaitableEvent(base::WaitableEvent* event);
  void OnProcessExitedEarly(base::WaitableEvent* event);
#endif

  ChildProcessData data_;
  BrowserChildProcessHostDelegate* delegate_;
  scoped_ptr<ChildProcessHost> child_process_host_;

  scoped_ptr<ChildProcessLauncher> child_process_;

  PowerMonitorMessageBroadcaster power_monitor_message_broadcaster_;

#if defined(OS_WIN)
  
  
  
  base::WaitableEventWatcher early_exit_watcher_;
#endif
};

}  

#endif  
