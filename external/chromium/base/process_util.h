// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_UTIL_H_
#define BASE_PROCESS_UTIL_H_
#pragma once

#include "base/basictypes.h"

#if defined(OS_WIN)
#include <windows.h>
#include <tlhelp32.h>
#elif defined(OS_MACOSX)
struct kinfo_proc;
typedef struct _malloc_zone_t malloc_zone_t;
#include <mach/mach.h>
#elif defined(OS_POSIX)
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#endif

#include <list>
#include <string>
#include <utility>
#include <vector>

#include "base/base_api.h"
#include "base/file_descriptor_shuffle.h"
#include "base/file_path.h"
#include "base/process.h"

class CommandLine;

namespace base {

#if defined(OS_WIN)
struct ProcessEntry : public PROCESSENTRY32 {
  ProcessId pid() const { return th32ProcessID; }
  ProcessId parent_pid() const { return th32ParentProcessID; }
  const wchar_t* exe_file() const { return szExeFile; }
};

struct IoCounters : public IO_COUNTERS {
};

const uint32 kProcessAccessTerminate              = PROCESS_TERMINATE;
const uint32 kProcessAccessCreateThread           = PROCESS_CREATE_THREAD;
const uint32 kProcessAccessSetSessionId           = PROCESS_SET_SESSIONID;
const uint32 kProcessAccessVMOperation            = PROCESS_VM_OPERATION;
const uint32 kProcessAccessVMRead                 = PROCESS_VM_READ;
const uint32 kProcessAccessVMWrite                = PROCESS_VM_WRITE;
const uint32 kProcessAccessDuplicateHandle        = PROCESS_DUP_HANDLE;
const uint32 kProcessAccessCreateProcess          = PROCESS_CREATE_PROCESS;
const uint32 kProcessAccessSetQuota               = PROCESS_SET_QUOTA;
const uint32 kProcessAccessSetInformation         = PROCESS_SET_INFORMATION;
const uint32 kProcessAccessQueryInformation       = PROCESS_QUERY_INFORMATION;
const uint32 kProcessAccessSuspendResume          = PROCESS_SUSPEND_RESUME;
const uint32 kProcessAccessQueryLimitedInfomation =
    PROCESS_QUERY_LIMITED_INFORMATION;
const uint32 kProcessAccessWaitForTermination     = SYNCHRONIZE;
#elif defined(OS_POSIX)

struct ProcessEntry {
  ProcessEntry();
  ~ProcessEntry();

  ProcessId pid() const { return pid_; }
  ProcessId parent_pid() const { return ppid_; }
  ProcessId gid() const { return gid_; }
  const char* exe_file() const { return exe_file_.c_str(); }
  const std::vector<std::string>& cmd_line_args() const {
    return cmd_line_args_;
  }

  ProcessId pid_;
  ProcessId ppid_;
  ProcessId gid_;
  std::string exe_file_;
  std::vector<std::string> cmd_line_args_;
};

struct IoCounters {
  uint64_t ReadOperationCount;
  uint64_t WriteOperationCount;
  uint64_t OtherOperationCount;
  uint64_t ReadTransferCount;
  uint64_t WriteTransferCount;
  uint64_t OtherTransferCount;
};

const uint32 kProcessAccessTerminate              = 0;
const uint32 kProcessAccessCreateThread           = 0;
const uint32 kProcessAccessSetSessionId           = 0;
const uint32 kProcessAccessVMOperation            = 0;
const uint32 kProcessAccessVMRead                 = 0;
const uint32 kProcessAccessVMWrite                = 0;
const uint32 kProcessAccessDuplicateHandle        = 0;
const uint32 kProcessAccessCreateProcess          = 0;
const uint32 kProcessAccessSetQuota               = 0;
const uint32 kProcessAccessSetInformation         = 0;
const uint32 kProcessAccessQueryInformation       = 0;
const uint32 kProcessAccessSuspendResume          = 0;
const uint32 kProcessAccessQueryLimitedInfomation = 0;
const uint32 kProcessAccessWaitForTermination     = 0;
#endif  

enum TerminationStatus {
  TERMINATION_STATUS_NORMAL_TERMINATION,   
  TERMINATION_STATUS_ABNORMAL_TERMINATION, 
  TERMINATION_STATUS_PROCESS_WAS_KILLED,   
  TERMINATION_STATUS_PROCESS_CRASHED,      
  TERMINATION_STATUS_STILL_RUNNING,        
  TERMINATION_STATUS_MAX_ENUM
};

BASE_API ProcessId GetCurrentProcId();

BASE_API ProcessHandle GetCurrentProcessHandle();

BASE_API bool OpenProcessHandle(ProcessId pid, ProcessHandle* handle);

BASE_API bool OpenPrivilegedProcessHandle(ProcessId pid, ProcessHandle* handle);

BASE_API bool OpenProcessHandleWithAccess(ProcessId pid,
                                          uint32 access_flags,
                                          ProcessHandle* handle);

BASE_API void CloseProcessHandle(ProcessHandle process);

BASE_API ProcessId GetProcId(ProcessHandle process);

#if defined(OS_LINUX)
FilePath GetProcessExecutablePath(ProcessHandle process);

int ParseProcStatCPU(const std::string& input);

static const char kAdjustOOMScoreSwitch[] = "--adjust-oom-score";

bool AdjustOOMScore(ProcessId process, int score);
#endif

#if defined(OS_POSIX)
ProcessId GetParentProcessId(ProcessHandle process);

void CloseSuperfluousFds(const InjectiveMultimap& saved_map);
#endif

#if defined(OS_WIN)

enum IntegrityLevel {
  INTEGRITY_UNKNOWN,
  LOW_INTEGRITY,
  MEDIUM_INTEGRITY,
  HIGH_INTEGRITY,
};
BASE_API bool GetProcessIntegrityLevel(ProcessHandle process,
                                       IntegrityLevel *level);

BASE_API bool LaunchApp(const std::wstring& cmdline,
                        bool wait, bool start_hidden,
                        ProcessHandle* process_handle);

BASE_API bool LaunchAppWithHandleInheritance(const std::wstring& cmdline,
                                             bool wait, bool start_hidden,
                                             ProcessHandle* process_handle);

BASE_API bool LaunchAppAsUser(UserTokenHandle token,
                              const std::wstring& cmdline,
                              bool start_hidden,
                              ProcessHandle* process_handle);

BASE_API bool LaunchAppAsUser(UserTokenHandle token,
                              const std::wstring& cmdline,
                              bool start_hidden, ProcessHandle* process_handle,
                              bool empty_desktop_name, bool inherit_handles);


#elif defined(OS_POSIX)
typedef std::vector<std::pair<int, int> > file_handle_mapping_vector;
bool LaunchApp(const std::vector<std::string>& argv,
               const file_handle_mapping_vector& fds_to_remap,
               bool wait, ProcessHandle* process_handle);

typedef std::vector<std::pair<std::string, std::string> > environment_vector;
bool LaunchApp(const std::vector<std::string>& argv,
               const environment_vector& environ,
               const file_handle_mapping_vector& fds_to_remap,
               bool wait, ProcessHandle* process_handle);

bool LaunchAppInNewProcessGroup(const std::vector<std::string>& argv,
                                const environment_vector& environ,
                                const file_handle_mapping_vector& fds_to_remap,
                                bool wait, ProcessHandle* process_handle);

char** AlterEnvironment(const environment_vector& changes,
                        const char* const* const env);
#endif  

BASE_API bool LaunchApp(const CommandLine& cl, bool wait, bool start_hidden,
                        ProcessHandle* process_handle);

BASE_API bool GetAppOutput(const CommandLine& cl, std::string* output);

#if defined(OS_POSIX)
bool GetAppOutputRestricted(const CommandLine& cl,
                            std::string* output, size_t max_output);
#endif

class ProcessFilter {
 public:
  
  
  virtual bool Includes(const ProcessEntry& entry) const = 0;

 protected:
  virtual ~ProcessFilter() {}
};

BASE_API int GetProcessCount(const FilePath::StringType& executable_name,
                             const ProcessFilter* filter);

BASE_API bool KillProcesses(const FilePath::StringType& executable_name,
                            int exit_code, const ProcessFilter* filter);

BASE_API bool KillProcess(ProcessHandle process, int exit_code, bool wait);

#if defined(OS_POSIX)
bool KillProcessGroup(ProcessHandle process_group_id);
#endif

#if defined(OS_WIN)
BASE_API bool KillProcessById(ProcessId process_id, int exit_code, bool wait);
#endif

BASE_API TerminationStatus GetTerminationStatus(ProcessHandle handle,
                                                int* exit_code);

BASE_API bool WaitForExitCode(ProcessHandle handle, int* exit_code);

BASE_API bool WaitForExitCodeWithTimeout(ProcessHandle handle, int* exit_code,
                                         int64 timeout_milliseconds);

BASE_API bool WaitForProcessesToExit(
    const FilePath::StringType& executable_name,
    int64 wait_milliseconds,
    const ProcessFilter* filter);

BASE_API bool WaitForSingleProcess(ProcessHandle handle,
                                   int64 wait_milliseconds);

BASE_API bool CleanupProcesses(const FilePath::StringType& executable_name,
                               int64 wait_milliseconds,
                               int exit_code,
                               const ProcessFilter* filter);

class BASE_API ProcessIterator {
 public:
  typedef std::list<ProcessEntry> ProcessEntries;

  explicit ProcessIterator(const ProcessFilter* filter);
  virtual ~ProcessIterator();

  
  
  
  
  
  const ProcessEntry* NextProcessEntry();

  
  ProcessEntries Snapshot();

 protected:
  virtual bool IncludeEntry();
  const ProcessEntry& entry() { return entry_; }

 private:
  
  
  
  bool CheckForNextProcess();

  
  
  void InitProcessEntry(ProcessEntry* entry);

#if defined(OS_WIN)
  HANDLE snapshot_;
  bool started_iteration_;
#elif defined(OS_MACOSX)
  std::vector<kinfo_proc> kinfo_procs_;
  size_t index_of_kinfo_proc_;
#elif defined(OS_POSIX)
  DIR *procfs_dir_;
#endif
  ProcessEntry entry_;
  const ProcessFilter* filter_;

  DISALLOW_COPY_AND_ASSIGN(ProcessIterator);
};

class BASE_API NamedProcessIterator : public ProcessIterator {
 public:
  NamedProcessIterator(const FilePath::StringType& executable_name,
                       const ProcessFilter* filter);
  virtual ~NamedProcessIterator();

 protected:
  virtual bool IncludeEntry();

 private:
  FilePath::StringType executable_name_;

  DISALLOW_COPY_AND_ASSIGN(NamedProcessIterator);
};

struct WorkingSetKBytes {
  WorkingSetKBytes() : priv(0), shareable(0), shared(0) {}
  size_t priv;
  size_t shareable;
  size_t shared;
};

struct CommittedKBytes {
  CommittedKBytes() : priv(0), mapped(0), image(0) {}
  size_t priv;
  size_t mapped;
  size_t image;
};

struct FreeMBytes {
  size_t total;
  size_t largest;
  void* largest_ptr;
};

BASE_API int64 TimeValToMicroseconds(const struct timeval& tv);

class BASE_API ProcessMetrics {
 public:
  ~ProcessMetrics();

  
  
#if !defined(OS_MACOSX)
  static ProcessMetrics* CreateProcessMetrics(ProcessHandle process);
#else
  class PortProvider {
   public:
    
    
    
    
    virtual mach_port_t TaskForPid(ProcessHandle process) const = 0;
  };

  
  
  
  static ProcessMetrics* CreateProcessMetrics(ProcessHandle process,
                                              PortProvider* port_provider);
#endif  

  
  
  
  size_t GetPagefileUsage() const;
  
  size_t GetPeakPagefileUsage() const;
  
  
  size_t GetWorkingSetSize() const;
  
  size_t GetPeakWorkingSetSize() const;
  
  
  
  
  bool GetMemoryBytes(size_t* private_bytes,
                      size_t* shared_bytes);
  
  
  void GetCommittedKBytes(CommittedKBytes* usage) const;
  
  
  bool GetWorkingSetKBytes(WorkingSetKBytes* ws_usage) const;

  
  
  
  
  bool CalculateFreeMemory(FreeMBytes* free) const;

  
  
  
  
  
  
  double GetCPUUsage();

  
  
  
  
  
  bool GetIOCounters(IoCounters* io_counters) const;

 private:
#if !defined(OS_MACOSX)
  explicit ProcessMetrics(ProcessHandle process);
#else
  ProcessMetrics(ProcessHandle process, PortProvider* port_provider);
#endif  

  ProcessHandle process_;

  int processor_count_;

  
  
  int64 last_time_;
  int64 last_system_time_;

#if defined(OS_MACOSX)
  
  mach_port_t TaskForPid(ProcessHandle process) const;

  PortProvider* port_provider_;
#elif defined(OS_POSIX)
  
  int last_cpu_;
#endif  

  DISALLOW_COPY_AND_ASSIGN(ProcessMetrics);
};

BASE_API size_t GetSystemCommitCharge();

BASE_API bool EnableLowFragmentationHeap();

BASE_API void EnableTerminationOnHeapCorruption();

#if !defined(OS_WIN)
void EnableTerminationOnOutOfMemory();
#if defined(OS_MACOSX)
malloc_zone_t* GetPurgeableZone();
#endif
#endif

BASE_API bool EnableInProcessStackDumping();

BASE_API void RaiseProcessToHighPriority();

#if defined(OS_MACOSX)
void RestoreDefaultExceptionHandler();
#endif  

}  

#endif  
