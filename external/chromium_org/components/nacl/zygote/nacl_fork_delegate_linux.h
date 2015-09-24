// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_ZYGOTE_NACL_FORK_DELEGATE_LINUX_H_
#define COMPONENTS_NACL_ZYGOTE_NACL_FORK_DELEGATE_LINUX_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/common/zygote_fork_delegate_linux.h"

class NaClForkDelegate : public content::ZygoteForkDelegate {
 public:
  NaClForkDelegate();
  virtual ~NaClForkDelegate();

  virtual void Init(int sandboxdesc) OVERRIDE;
  virtual void InitialUMA(std::string* uma_name,
                          int* uma_sample,
                          int* uma_boundary_value) OVERRIDE;
  virtual bool CanHelp(const std::string& process_type, std::string* uma_name,
                          int* uma_sample, int* uma_boundary_value) OVERRIDE;
  virtual pid_t Fork(const std::vector<int>& fds) OVERRIDE;
  virtual bool AckChild(int fd,
                        const std::string& channel_switch) OVERRIDE;
  virtual bool GetTerminationStatus(pid_t pid, bool known_dead,
                                    base::TerminationStatus* status,
                                    int* exit_code) OVERRIDE;

 private:
  
  
  enum NaClHelperStatus {
    kNaClHelperUnused = 0,
    kNaClHelperMissing = 1,
    kNaClHelperBootstrapMissing = 2,
    kNaClHelperValgrind = 3,
    kNaClHelperLaunchFailed = 4,
    kNaClHelperAckFailed = 5,
    kNaClHelperSuccess = 6,
    kNaClHelperStatusBoundary  
  };

  NaClHelperStatus status_;
  int fd_;
};

#endif  
