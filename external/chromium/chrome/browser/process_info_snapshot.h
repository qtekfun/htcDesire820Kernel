// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROCESS_INFO_SNAPSHOT_H_
#define CHROME_BROWSER_PROCESS_INFO_SNAPSHOT_H_
#pragma once

#include <sys/types.h>

#include <map>
#include <string>
#include <vector>

#include "base/process_util.h"

class ProcessInfoSnapshot {
 public:
  ProcessInfoSnapshot();
  ~ProcessInfoSnapshot();

  
  
  static const size_t kMaxPidListSize;

  
  
  
  
  
  bool Sample(std::vector<base::ProcessId> pid_list);

  
  void Reset();

  
  
  
  
  struct ProcInfoEntry {
    base::ProcessId pid;
    base::ProcessId ppid;
    uid_t uid;
    uid_t euid;
    
    
    uint64_t rss;
    uint64_t rshrd;
    uint64_t rprvt;
    uint64_t vsize;
    std::string command;

    ProcInfoEntry()
        : pid(0),
          ppid(0),
          uid(0),
          euid(0),
          rss(0),
          rshrd(0),
          rprvt(0),
          vsize(0) {
    }
  };

  
  
  
  
  bool GetProcInfo(int pid,
                   ProcInfoEntry* proc_info) const;

  
  
  
  
  
  
  
  
  
  
  
  
  bool GetCommittedKBytesOfPID(int pid,
                               base::CommittedKBytes* usage) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetWorkingSetKBytesOfPID(int pid,
                                base::WorkingSetKBytes* ws_usage) const;

  
  
  
  
  

 private:
  
  std::map<int,ProcInfoEntry> proc_info_entries_;
};

#endif  
