// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_PROCESS_PROXY_PROCESS_PROXY_H_
#define CHROMEOS_PROCESS_PROXY_PROCESS_PROXY_H_

#include <fcntl.h>
#include <signal.h>

#include <cstdio>
#include <string>

#include "base/memory/ref_counted.h"
#include "chromeos/process_proxy/process_output_watcher.h"

namespace base {
class TaskRunner;
class Thread;
}  

namespace chromeos {

class ProcessProxy : public base::RefCountedThreadSafe<ProcessProxy> {
 public:
  ProcessProxy();

  
  bool Open(const std::string& command, pid_t* pid);

  
  
  
  bool StartWatchingOnThread(base::Thread* watch_thread,
                             const ProcessOutputCallback& callback);

  
  bool Write(const std::string& text);

  
  
  void Close();

  
  bool OnTerminalResize(int width, int height);

 private:
  friend class base::RefCountedThreadSafe<ProcessProxy>;
  
  ~ProcessProxy();

  
  
  
  
  bool CreatePseudoTerminalPair(int *pt_pair);

  bool LaunchProcess(const std::string& command, int slave_fd, pid_t* pid);

  
  
  void OnProcessOutput(ProcessOutputType type, const std::string& output);
  void CallOnProcessOutputCallback(ProcessOutputType type,
                                   const std::string& output);

  bool StopWatching();

  
  void CloseAllFdPairs();
  
  void CloseFdPair(int* pipe);
  
  void CloseFd(int* fd);
  void ClearAllFdPairs();
  
  void ClearFdPair(int* pipe);

  bool process_launched_;
  pid_t pid_;

  bool callback_set_;
  ProcessOutputCallback callback_;
  scoped_refptr<base::TaskRunner> callback_runner_;

  bool watcher_started_;

  int pt_pair_[2];
  int shutdown_pipe_[2];

  DISALLOW_COPY_AND_ASSIGN(ProcessProxy);
};

}  

#endif  
