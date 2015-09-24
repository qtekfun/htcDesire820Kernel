// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SERVICES_BROKER_PROCESS_H_
#define SANDBOX_LINUX_SERVICES_BROKER_PROCESS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/pickle.h"
#include "base/process/process.h"

namespace sandbox {

class BrokerProcess {
 public:
  
  
  
  
  
  
  
  
  explicit BrokerProcess(int denied_errno,
                         const std::vector<std::string>& allowed_r_files,
                         const std::vector<std::string>& allowed_w_files,
                         bool fast_check_in_client = true,
                         bool quiet_failures_for_tests = false);
  ~BrokerProcess();
  
  
  
  
  bool Init(bool (*sandbox_callback)(void));

  
  
  
  
  int Access(const char* pathname, int mode) const;
  
  
  
  
  int Open(const char* pathname, int flags) const;

  int broker_pid() const { return broker_pid_; }

 private:
  enum IPCCommands {
    kCommandInvalid = 0,
    kCommandOpen,
    kCommandAccess,
  };
  int PathAndFlagsSyscall(enum IPCCommands command_type,
                          const char* pathname,
                          int flags) const;
  bool HandleRequest() const;
  bool HandleRemoteCommand(IPCCommands command_type,
                           int reply_ipc,
                           const Pickle& read_pickle,
                           PickleIterator iter) const;

  void AccessFileForIPC(const std::string& requested_filename,
                        int mode,
                        Pickle* write_pickle) const;
  void OpenFileForIPC(const std::string& requested_filename,
                      int flags,
                      Pickle* write_pickle,
                      std::vector<int>* opened_files) const;
  bool GetFileNameIfAllowedToAccess(const char*, int, const char**) const;
  bool GetFileNameIfAllowedToOpen(const char*, int, const char**) const;
  const int denied_errno_;
  bool initialized_;               
  bool is_child_;                  
  bool fast_check_in_client_;      
                                   
  bool quiet_failures_for_tests_;  
                                   
  pid_t broker_pid_;               
  const std::vector<std::string> allowed_r_files_;  
  const std::vector<std::string> allowed_w_files_;  
  int ipc_socketpair_;  
  DISALLOW_IMPLICIT_CONSTRUCTORS(BrokerProcess);
};

}  

#endif  
