// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_SERVICE_PROCESS_UTIL_H_
#define CHROME_COMMON_SERVICE_PROCESS_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/process.h"
#include "base/shared_memory.h"
#include "ipc/ipc_channel_handle.h"

class Task;
class CommandLine;

namespace base {
  class MessageLoopProxy;
}

IPC::ChannelHandle GetServiceProcessChannel();

#if !defined(OS_MACOSX)
std::string GetServiceProcessScopedName(const std::string& append_str);

std::string GetServiceProcessScopedVersionedName(const std::string& append_str);
#endif  

bool CheckServiceProcessReady();

bool GetServiceProcessData(std::string* version, base::ProcessId* pid);

bool ForceServiceProcessShutdown(const std::string& version,
                                 base::ProcessId process_id);

class ServiceProcessState {
 public:
  ServiceProcessState();
  ~ServiceProcessState();

  
  
  bool Initialize();

  
  
  
  
  
  
  bool SignalReady(
      base::MessageLoopProxy* message_loop_proxy, Task* shutdown_task);

  
  void SignalStopped();

  
  bool AddToAutoRun();

  
  bool RemoveFromAutoRun();

  
  IPC::ChannelHandle GetServiceProcessChannel();

 private:

#if !defined(OS_MACOSX)
  
  bool CreateSharedData();

  
  
  bool HandleOtherVersion();

  
  
  bool TakeSingletonLock();
#endif  

  
  void CreateState();

  
  void TearDownState();

  
  
  void CreateAutoRunCommandLine();

  
  
  struct StateData;
  StateData* state_;
  scoped_ptr<base::SharedMemory> shared_mem_service_data_;
  scoped_ptr<CommandLine> autorun_command_line_;
};

#endif  
