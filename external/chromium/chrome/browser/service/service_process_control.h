// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SERVICE_SERVICE_PROCESS_CONTROL_H_
#define CHROME_BROWSER_SERVICE_SERVICE_PROCESS_CONTROL_H_

#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/process.h"
#include "base/task.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ipc/ipc_sync_channel.h"

class Profile;
class CommandLine;

namespace remoting {
struct ChromotingHostInfo;
}  

class ServiceProcessControl : public IPC::Channel::Sender,
                              public IPC::Channel::Listener,
                              public NotificationObserver {
 public:
  typedef IDMap<ServiceProcessControl>::iterator iterator;
  typedef std::queue<IPC::Message> MessageQueue;
  typedef Callback1<const remoting::ChromotingHostInfo&>::Type
      RemotingHostStatusHandler;

  
  class MessageHandler {
   public:
    virtual ~MessageHandler() {}

    
    virtual void OnRemotingHostInfo(
        const remoting::ChromotingHostInfo& host_info) = 0;
  };

  
  explicit ServiceProcessControl(Profile* profile);
  virtual ~ServiceProcessControl();

  
  Profile* profile() const { return profile_; }

  
  bool is_connected() const { return channel_.get() != NULL; }

  
  
  
  
  
  
  
  
  
  
  
  
  void Launch(Task* success_task, Task* failure_task);

  
  virtual bool OnMessageReceived(const IPC::Message& message);
  virtual void OnChannelConnected(int32 peer_pid);
  virtual void OnChannelError();

  
  virtual bool Send(IPC::Message* message);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void OnCloudPrintProxyIsEnabled(bool enabled, std::string email);
  void OnRemotingHostInfo(const remoting::ChromotingHostInfo& host_info);

  
  
  
  bool Shutdown();

  
  
  bool GetCloudPrintProxyStatus(
      Callback2<bool, std::string>::Type* cloud_print_status_callback);

  
  
  bool SetRemotingHostCredentials(const std::string& user,
                                  const std::string& auth_token);

  bool EnableRemotingHost();
  bool DisableRemotingHost();

  
  
  
  bool RequestRemotingHostStatus();

  
  
  void AddMessageHandler(MessageHandler* message_handler);

  
  
  
  void RemoveMessageHandler(MessageHandler* message_handler);

 private:
  
  
  class Launcher
      : public base::RefCountedThreadSafe<ServiceProcessControl::Launcher> {
   public:
    Launcher(ServiceProcessControl* process, CommandLine* cmd_line);
    
    
    
    void Run(Task* task);

    bool launched() const { return launched_; }

   private:
    friend class base::RefCountedThreadSafe<ServiceProcessControl::Launcher>;
    virtual ~Launcher();

#if !defined(OS_MACOSX)
    void DoDetectLaunched();
#endif  

    void DoRun();
    void Notify();
    ServiceProcessControl* process_;
    scoped_ptr<CommandLine> cmd_line_;
    scoped_ptr<Task> notify_task_;
    bool launched_;
    uint32 retry_count_;
  };

  typedef std::vector<Task*> TaskList;

  
  void RunConnectDoneTasks();

  
  void OnProcessLaunched();

  
  void ConnectInternal();

  static void RunAllTasksHelper(TaskList* task_list);

  Profile* profile_;

  
  scoped_ptr<IPC::SyncChannel> channel_;

  
  scoped_refptr<Launcher> launcher_;

  
  
  TaskList connect_done_tasks_;
  
  TaskList connect_success_tasks_;
  
  TaskList connect_failure_tasks_;

  
  
  scoped_ptr<Callback2<bool, std::string>::Type> cloud_print_status_callback_;

  
  std::set<MessageHandler*> message_handlers_;

  NotificationRegistrar registrar_;
};

#endif  
