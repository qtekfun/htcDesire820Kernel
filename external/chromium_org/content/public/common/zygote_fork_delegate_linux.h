// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_ZYGOTE_FORK_DELEGATE_LINUX_H_
#define CONTENT_PUBLIC_COMMON_ZYGOTE_FORK_DELEGATE_LINUX_H_

#include <unistd.h>

#include <string>
#include <vector>

#include "base/process/kill.h"

namespace content {

class ZygoteForkDelegate {
 public:
  
  
  
  
  virtual ~ZygoteForkDelegate() {}

  
  
  virtual void Init(int sandboxdesc) = 0;

  
  
  virtual void InitialUMA(std::string* uma_name,
                          int* uma_sample,
                          int* uma_boundary_value) = 0;

  
  
  
  virtual bool CanHelp(const std::string& process_type, std::string* uma_name,
                       int* uma_sample, int* uma_boundary_value) = 0;

  
  enum {
    
    kBrowserFDIndex,
    
    
    
    kDummyFDIndex,
    kParentFDIndex,
    kNumPassedFDs  
  };

  
  
  
  
  virtual pid_t Fork(const std::vector<int>& fds) = 0;

  
  
  
  virtual bool AckChild(int fd, const std::string& channel_switch) = 0;

  
  
  
  
  
  
  
  virtual bool GetTerminationStatus(pid_t pid, bool known_dead,
                                    base::TerminationStatus* status,
                                    int* exit_code) = 0;
};

}  

#endif  
