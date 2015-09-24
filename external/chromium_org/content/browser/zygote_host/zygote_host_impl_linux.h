// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ZYGOTE_HOST_ZYGOTE_HOST_IMPL_LINUX_H_
#define CONTENT_BROWSER_ZYGOTE_HOST_ZYGOTE_HOST_IMPL_LINUX_H_

#include <set>
#include <string>
#include <vector>

#include "base/pickle.h"
#include "base/process/kill.h"
#include "base/synchronization/lock.h"
#include "content/public/browser/file_descriptor_info.h"
#include "content/public/browser/zygote_host_linux.h"

template<typename Type>
struct DefaultSingletonTraits;

namespace content {

class CONTENT_EXPORT ZygoteHostImpl : public ZygoteHost {
 public:
  
  static ZygoteHostImpl* GetInstance();

  void Init(const std::string& sandbox_cmd);

  
  void TearDownAfterLastChild();

  
  
  
  pid_t ForkRequest(const std::vector<std::string>& command_line,
                    const std::vector<FileDescriptorInfo>& mapping,
                    const std::string& process_type);
  void EnsureProcessTerminated(pid_t process);

  
  
  
  
  
  
  
  
  
  
  
  base::TerminationStatus GetTerminationStatus(base::ProcessHandle handle,
                                               bool known_dead,
                                               int* exit_code);

  
  virtual pid_t GetPid() const OVERRIDE;
  virtual pid_t GetSandboxHelperPid() const OVERRIDE;
  virtual int GetSandboxStatus() const OVERRIDE;
  virtual void AdjustRendererOOMScore(base::ProcessHandle process_handle,
                                      int score) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<ZygoteHostImpl>;

  ZygoteHostImpl();
  virtual ~ZygoteHostImpl();

  
  
  void TearDown();

  
  void ZygoteChildBorn(pid_t process);

  
  void ZygoteChildDied(pid_t process);

  
  
  
  bool SendMessage(const Pickle& data, const std::vector<int>* fds);

  ssize_t ReadReply(void* buf, size_t buflen);

  int control_fd_;  
  
  
  
  base::Lock control_lock_;
  pid_t pid_;
  bool init_;
  bool using_suid_sandbox_;
  std::string sandbox_binary_;
  bool have_read_sandbox_status_word_;
  int sandbox_status_;
  
  
  base::Lock child_tracking_lock_;
  std::set<pid_t> list_of_running_zygote_children_;
  bool should_teardown_after_last_child_exits_;
};

}  

#endif  
