// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_SERVICE_PROCESS_H_
#define CHROME_SERVICE_SERVICE_PROCESS_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/threading/thread.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/service/cloud_print/cloud_print_proxy.h"

class ServiceProcessPrefs;
class ServiceIPCServer;
class CommandLine;
class ServiceURLRequestContextGetter;
class ServiceProcessState;

namespace net {
class NetworkChangeNotifier;
}

class CommandLine;

class ServiceProcess : public cloud_print::CloudPrintProxy::Client {
 public:
  ServiceProcess();
  virtual ~ServiceProcess();

  
  
  bool Initialize(base::MessageLoopForUI* message_loop,
                  const CommandLine& command_line,
                  ServiceProcessState* state);

  bool Teardown();
  
  
  
  
  

  
  
  
  
  
  
  
  base::Thread* io_thread() const {
    return io_thread_.get();
  }
  
  
  
  base::Thread* file_thread() const {
    return file_thread_.get();
  }

  
  
  
  base::WaitableEvent* shutdown_event() {
    return &shutdown_event_;
  }

  
  void Shutdown();

  void SetUpdateAvailable() {
    update_available_ = true;
  }
  bool update_available() const { return update_available_; }

  
  
  
  bool HandleClientDisconnect();

  cloud_print::CloudPrintProxy* GetCloudPrintProxy();

  
  virtual void OnCloudPrintProxyEnabled(bool persist_state) OVERRIDE;
  virtual void OnCloudPrintProxyDisabled(bool persist_state) OVERRIDE;

  ServiceURLRequestContextGetter* GetServiceURLRequestContextGetter();

 private:
  friend class TestServiceProcess;

  
  void ScheduleShutdownCheck();

  
  
  void ShutdownIfNeeded();

  
  void ScheduleCloudPrintPolicyCheck();

  
  void CloudPrintPolicyCheckIfNeeded();

  
  
  void OnServiceEnabled();

  
  
  void OnServiceDisabled();

  
  void Terminate();

  scoped_ptr<net::NetworkChangeNotifier> network_change_notifier_;
  scoped_ptr<base::Thread> io_thread_;
  scoped_ptr<base::Thread> file_thread_;
  scoped_refptr<base::SequencedWorkerPool> blocking_pool_;
  scoped_ptr<cloud_print::CloudPrintProxy> cloud_print_proxy_;
  scoped_ptr<ServiceProcessPrefs> service_prefs_;
  scoped_ptr<ServiceIPCServer> ipc_server_;
  scoped_ptr<ServiceProcessState> service_process_state_;

  
  base::WaitableEvent shutdown_event_;

  
  base::MessageLoop* main_message_loop_;

  
  int enabled_services_;

  
  bool update_available_;

  scoped_refptr<ServiceURLRequestContextGetter> request_context_getter_;

  DISALLOW_COPY_AND_ASSIGN(ServiceProcess);
};

extern ServiceProcess* g_service_process;

#endif  
