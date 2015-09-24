// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SERVICE_PROCESS_SERVICE_PROCESS_CONTROL_H_
#define CHROME_BROWSER_SERVICE_PROCESS_SERVICE_PROCESS_CONTROL_H_

#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/cancelable_callback.h"
#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/process/process.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"

class CommandLine;

namespace cloud_print {
struct CloudPrintProxyInfo;
}  

class ServiceProcessControl : public IPC::Sender,
                              public IPC::Listener,
                              public content::NotificationObserver {
 public:
  enum ServiceProcessEvent {
    SERVICE_EVENT_INITIALIZE,
    SERVICE_EVENT_ENABLED_ON_LAUNCH,
    SERVICE_EVENT_ENABLE,
    SERVICE_EVENT_DISABLE,
    SERVICE_EVENT_DISABLE_BY_POLICY,
    SERVICE_EVENT_LAUNCH,
    SERVICE_EVENT_LAUNCHED,
    SERVICE_EVENT_LAUNCH_FAILED,
    SERVICE_EVENT_CHANNEL_CONNECTED,
    SERVICE_EVENT_CHANNEL_ERROR,
    SERVICE_EVENT_INFO_REQUEST,
    SERVICE_EVENT_INFO_REPLY,
    SERVICE_EVENT_HISTOGRAMS_REQUEST,
    SERVICE_EVENT_HISTOGRAMS_REPLY,
    SERVICE_EVENT_MAX,
  };

  typedef IDMap<ServiceProcessControl>::iterator iterator;
  typedef std::queue<IPC::Message> MessageQueue;
  typedef base::Callback<void(const cloud_print::CloudPrintProxyInfo&)>
      CloudPrintProxyInfoHandler;

  
  static ServiceProcessControl* GetInstance();

  
  
  virtual bool IsConnected() const;

  
  
  
  
  
  
  
  
  
  
  
  virtual void Launch(const base::Closure& success_task,
                      const base::Closure& failure_task);

  
  
  virtual void Disconnect();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  virtual bool Shutdown();

  
  
  
  
  
  bool GetCloudPrintProxyInfo(
      const CloudPrintProxyInfoHandler& cloud_print_status_callback);

  
  
  
  
  
  bool GetHistograms(const base::Closure& cloud_print_status_callback,
                     const base::TimeDelta& timeout);

 private:
  
  
  class Launcher
      : public base::RefCountedThreadSafe<ServiceProcessControl::Launcher> {
   public:
    Launcher(ServiceProcessControl* process, CommandLine* cmd_line);
    
    
    
    void Run(const base::Closure& task);

    bool launched() const { return launched_; }

   private:
    friend class base::RefCountedThreadSafe<ServiceProcessControl::Launcher>;
    virtual ~Launcher();

#if !defined(OS_MACOSX)
    void DoDetectLaunched();
#endif  

    void DoRun();
    void Notify();
    void CloseProcessHandle();
    ServiceProcessControl* process_;
    scoped_ptr<CommandLine> cmd_line_;
    base::Closure notify_task_;
    bool launched_;
    uint32 retry_count_;
    base::ProcessHandle process_handle_;
  };

  friend class MockServiceProcessControl;
  friend class CloudPrintProxyPolicyStartupTest;

  ServiceProcessControl();
  virtual ~ServiceProcessControl();

  friend struct DefaultSingletonTraits<ServiceProcessControl>;

  typedef std::vector<base::Closure> TaskList;

  
  void OnCloudPrintProxyInfo(
      const cloud_print::CloudPrintProxyInfo& proxy_info);
  void OnHistograms(const std::vector<std::string>& pickled_histograms);

  
  void RunHistogramsCallback();

  
  void RunConnectDoneTasks();

  
  void OnProcessLaunched();

  
  void ConnectInternal();

  
  void SetChannel(IPC::ChannelProxy* channel);

  static void RunAllTasksHelper(TaskList* task_list);

  
  scoped_ptr<IPC::ChannelProxy> channel_;

  
  scoped_refptr<Launcher> launcher_;

  
  TaskList connect_success_tasks_;
  
  TaskList connect_failure_tasks_;

  
  
  CloudPrintProxyInfoHandler cloud_print_info_callback_;

  
  
  base::Closure histograms_callback_;

  content::NotificationRegistrar registrar_;

  
  base::CancelableClosure histograms_timeout_callback_;
};

#endif  
