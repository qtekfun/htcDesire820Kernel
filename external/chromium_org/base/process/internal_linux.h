// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_LINUX_INTERNAL_H_
#define BASE_PROCESS_LINUX_INTERNAL_H_

#include <unistd.h>

#include "base/files/file_path.h"

namespace base {

class Time;
class TimeDelta;

namespace internal {

extern const char kProcDir[];

extern const char kStatFile[];

base::FilePath GetProcPidDir(pid_t pid);

pid_t ProcDirSlotToPid(const char* d_name);

bool ReadProcStats(pid_t pid, std::string* buffer);

bool ParseProcStats(const std::string& stats_data,
                    std::vector<std::string>* proc_stats);

enum ProcStatsFields {
  VM_COMM           = 1,   
  VM_STATE          = 2,   
  VM_PPID           = 3,   
  VM_PGRP           = 4,   
  VM_UTIME          = 13,  
  VM_STIME          = 14,  
  VM_NUMTHREADS     = 19,  
  VM_STARTTIME      = 21,  
  VM_VSIZE          = 22,  
  VM_RSS            = 23,  
};

int GetProcStatsFieldAsInt(const std::vector<std::string>& proc_stats,
                           ProcStatsFields field_num);

size_t GetProcStatsFieldAsSizeT(const std::vector<std::string>& proc_stats,
                                ProcStatsFields field_num);

int ReadProcStatsAndGetFieldAsInt(pid_t pid,
                                  ProcStatsFields field_num);

size_t ReadProcStatsAndGetFieldAsSizeT(pid_t pid,
                                       ProcStatsFields field_num);

Time GetBootTime();

TimeDelta ClockTicksToTimeDelta(int clock_ticks);

}  
}  

#endif  
