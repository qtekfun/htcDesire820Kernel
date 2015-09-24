// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_KILL_H_
#define BASE_PROCESS_KILL_H_

#include "base/files/file_path.h"
#include "base/process/process_handle.h"
#include "base/time/time.h"

namespace base {

class ProcessFilter;

enum TerminationStatus {
  TERMINATION_STATUS_NORMAL_TERMINATION,   
  TERMINATION_STATUS_ABNORMAL_TERMINATION, 
  TERMINATION_STATUS_PROCESS_WAS_KILLED,   
  TERMINATION_STATUS_PROCESS_CRASHED,      
  TERMINATION_STATUS_STILL_RUNNING,        
#if defined(OS_ANDROID)
  
  
  
  
  TERMINATION_STATUS_OOM_PROTECTED,        
#endif
  TERMINATION_STATUS_MAX_ENUM
};

BASE_EXPORT bool KillProcesses(const FilePath::StringType& executable_name,
                               int exit_code,
                               const ProcessFilter* filter);

BASE_EXPORT bool KillProcess(ProcessHandle process, int exit_code, bool wait);

#if defined(OS_POSIX)
BASE_EXPORT bool KillProcessGroup(ProcessHandle process_group_id);
#endif  

#if defined(OS_WIN)
BASE_EXPORT bool KillProcessById(ProcessId process_id,
                                 int exit_code,
                                 bool wait);
#endif  

BASE_EXPORT TerminationStatus GetTerminationStatus(ProcessHandle handle,
                                                   int* exit_code);

#if defined(OS_POSIX)
BASE_EXPORT TerminationStatus GetKnownDeadTerminationStatus(
    ProcessHandle handle, int* exit_code);
#endif  

BASE_EXPORT bool WaitForExitCode(ProcessHandle handle, int* exit_code);

BASE_EXPORT bool WaitForExitCodeWithTimeout(ProcessHandle handle,
                                            int* exit_code,
                                            base::TimeDelta timeout);

BASE_EXPORT bool WaitForProcessesToExit(
    const FilePath::StringType& executable_name,
    base::TimeDelta wait,
    const ProcessFilter* filter);

BASE_EXPORT bool WaitForSingleProcess(ProcessHandle handle,
                                      base::TimeDelta wait);

BASE_EXPORT bool CleanupProcesses(const FilePath::StringType& executable_name,
                                  base::TimeDelta wait,
                                  int exit_code,
                                  const ProcessFilter* filter);

BASE_EXPORT void EnsureProcessTerminated(ProcessHandle process_handle);

#if defined(OS_POSIX) && !defined(OS_MACOSX)
BASE_EXPORT void EnsureProcessGetsReaped(ProcessHandle process_handle);
#endif

}  

#endif  
