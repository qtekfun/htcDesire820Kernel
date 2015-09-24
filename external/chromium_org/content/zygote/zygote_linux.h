// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_ZYGOTE_ZYGOTE_H_
#define CONTENT_ZYGOTE_ZYGOTE_H_

#include <string>
#include <vector>

#include "base/containers/small_map.h"
#include "base/posix/global_descriptors.h"
#include "base/process/kill.h"
#include "base/process/process.h"

class Pickle;
class PickleIterator;

namespace content {

class ZygoteForkDelegate;

class Zygote {
 public:
  Zygote(int sandbox_flags,
         ZygoteForkDelegate* helper);
  ~Zygote();

  bool ProcessRequests();

 private:
  struct ZygoteProcessInfo {
    
    base::ProcessHandle internal_pid;
    
    
    bool started_from_helper;
  };
  typedef base::SmallMap< std::map<base::ProcessHandle, ZygoteProcessInfo> >
      ZygoteProcessMap;

  
  
  
  bool GetProcessInfo(base::ProcessHandle pid,
                      ZygoteProcessInfo* process_info);

  
  bool UsingSUIDSandbox() const;

  
  

  
  
  bool HandleRequestFromBrowser(int fd);

  void HandleReapRequest(int fd, const Pickle& pickle, PickleIterator iter);

  
  
  
  
  bool GetTerminationStatus(base::ProcessHandle real_pid, bool known_dead,
                            base::TerminationStatus* status,
                            int* exit_code);

  void HandleGetTerminationStatus(int fd,
                                  const Pickle& pickle,
                                  PickleIterator iter);

  
  
  
  
  
  int ForkWithRealPid(const std::string& process_type,
                      const base::GlobalDescriptors::Mapping& fd_mapping,
                      const std::string& channel_switch,
                      std::string* uma_name,
                      int* uma_sample,
                      int* uma_boundary_value);

  
  
  
  base::ProcessId ReadArgsAndFork(const Pickle& pickle,
                                  PickleIterator iter,
                                  std::vector<int>& fds,
                                  std::string* uma_name,
                                  int* uma_sample,
                                  int* uma_boundary_value);

  
  
  
  
  bool HandleForkRequest(int fd,
                         const Pickle& pickle,
                         PickleIterator iter,
                         std::vector<int>& fds);

  bool HandleGetSandboxStatus(int fd,
                              const Pickle& pickle,
                              PickleIterator iter);

  
  
  
  
  ZygoteProcessMap process_info_map_;

  const int sandbox_flags_;
  ZygoteForkDelegate* helper_;

  
  
  std::string initial_uma_name_;
  int initial_uma_sample_;
  int initial_uma_boundary_value_;
};

}  

#endif  
